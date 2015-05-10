//
// 七並べ
//

#pragma once
#include "f_arrange.hpp"   // 各ゲーム専用の関数ファイルの読み込み


// 七並べ リザルト
void result_arrange(AppEnv& app_env, Texture& sys, Texture& frm, short& rank) {
  while (1) {
    if (!app_env.isOpen()) { return; }

    // 左クリックでゲーム終了
    if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); break; }

    app_env.setupDraw();

    // システムメッセージの表示
    AS_disp_message(sys, frm, true, rank);

    app_env.update();
  }
}

// 七並べ 本編
void game_arrange(AppEnv& app_env) {
  // 乱数の基準値設定
  Random random; random.setSeed((u_int)time(nullptr));

  // 画像の読み込み
  Texture t_card("res/png/c01_standard.png");   // カードの画像
  Texture system("res/png/m02_as.png");         // システムメッセージ
  Texture wframe("res/png/sys04_frame.png");    // テロップ用フレーム

  // BGM と SE の読み込み
  Media thema(M_THEMA); thema.gain(0.5); thema.looping(true);
  Media intro(M_INTRO); intro.gain(0.5);   // イントロ

  Media click(SE_CLICK); click.gain(0.25);         // クリックしたとき
  Media finish(SE_FINISH); finish.gain(0.25);      // ゲーム終了

  // このゲーム内専用の変数
  short i, j;              // 繰り返し処理用
  short current_mark;      // 現在参照しているカードの記号
  short game_rank = 1;     // 現在の順位

  float card_x, card_y;    // カードの表示位置（始点）
  float tex_x, tex_y;      // カードの切り取り位置（始点）
  float angle = 0.0;       // カード画像の回転エフェクト用：変化量
  float scale = 1.0;       // カード画像の回転エフェクト用：実際の変化量
  float sys_x, sys_y;      // システムメッセージの表示位置

  bool isLow = false;      // 今より小さい数字が出せるか
  bool isHigh = false;     // 今より大きい数字が出せるか
  bool isEnable = false;   // 選択した行動が実行できるか
  bool isCheck = false;    // ゲーム終了判定を実行したか
  bool isAct = false;      // 対戦相手のAI用：行動したかの判定

  // このゲーム内専用の変数：マウス判定用
  Vec2f mouse_pos;   // マウスカーソルの座標
  bool on_mouse;     // マウスカーソルの判定

  // このゲーム内専用の変数：プレイヤー関係
  short player_turn = 0;           // 誰のターンか
  short card_count[PLAYER] = {};   // カードの残り枚数
  short pass_count[PLAYER] = {};   // パスした回数

  // 手札の情報
  struct Card {
    short number;
    short mark;
    bool used;
  };

  // カード情報の生成
  Card player[PLAYER][CARD_NUM] = {};   // 各プレイヤーの手札
  short num_left[CARD_MARK] = {};       // 場のカードの左端情報
  short num_right[CARD_MARK] = {};      // 場のカードの右端情報

  // ゲームの初期化
  {
    bool isSwap;   // ソート確認用

    // カードのシャッフル：一時的な配列を作成、データを格納してから乱数を使って入れ替える
    Card cardinfo[JOKER_NG] = {};
    for (i = 0; i < JOKER_NG; i++) { cardinfo[i] = {i % CARD_NUM, i / CARD_NUM, false}; }
    for (i = 0; i < JOKER_NG; i++) { std::swap(cardinfo[i], cardinfo[random.fromZeroToLast(JOKER_NG)]); }

    // 各プレイヤーの手札情報の初期化：乱数で用意した一時配列の内容を各プレイヤーの手札配列に入れ替える
    for (i = 0; i < PLAYER; i++) {
      for (j = 0; j < CARD_NUM; j++) { std::swap(player[i][j], cardinfo[j + (i * CARD_NUM)]); }
    }

    // プレイヤーの手札をソート
    {
      // 記号順に揃える
      while (1) {
        isSwap = false;
        for (i = 0; i < CARD_NUM - 1; i++) {
          if (player[DEF_PLAYER][i].mark > player[DEF_PLAYER][i + 1].mark) {
            std::swap(player[DEF_PLAYER][i], player[DEF_PLAYER][i + 1]);
            isSwap = true;
          }
        }
        if (!isSwap) { break; }   // 入れ替える必要がなくなったらループ終了
      }

      // 記号ごとに、数字順に揃える
      while (1) {
        isSwap = false;
        for (i = 0; i < CARD_NUM - 1; i++) {
          if (player[DEF_PLAYER][i].number > player[DEF_PLAYER][i + 1].number) {
            // 入れ替えるカードの記号が同じなら入れ替えを実行
            if (player[DEF_PLAYER][i].mark == player[DEF_PLAYER][i + 1].mark) {
              std::swap(player[DEF_PLAYER][i], player[DEF_PLAYER][i + 1]);
              isSwap = true;
            }
          }
        }
        if (!isSwap) { break; }   // 入れ替える必要がなくなったらループ終了
      }
    }

    // 場のカード情報の初期化
    for (i = 0; i < CARD_MARK; i++) { num_left[i] = 6; num_right[i] = 6; }

    // パス回数の初期化
    for (i = 0; i < PLAYER; i++) { pass_count[i] = 0; }

    // 手札に「７」があるか確認
    for (i = 0; i < PLAYER; i++) {
      for (j = 0; j < CARD_NUM; j++) {
        // 「７」があったら場に出した状態にする
        if (player[i][j].number == 6) { player[i][j].used = true; }
      }
    }

    intro.play();   // イントロ再生
  }


  /* 本編 */
  ////////////////////////////////////////////////////////////////////////////////
  while (1) {
    if (!app_env.isOpen()) { return; }

    // BGM再生：イントロが終わったらループ部分の再生
    if (!intro.isPlaying()) { if (!thema.isPlaying()) { thema.play(); } }

    // ゲームの終了条件
    if (!isCheck) {
      // プレイヤーがリタイアした
      if (pass_count[DEF_PLAYER] == RETIRE_P) { break; }

      // プレイヤーの手札がなくなった
      for (i = 0; i < CARD_NUM; i++) { if (!player[DEF_PLAYER][i].used) { break; } }
      if (i == CARD_NUM) { break; }

      isCheck = true;   // ゲーム終了しないとき、次のターンまで判定をスキップする
    }

    app_env.setupDraw();

    // システムメッセージの表示
    {
      // メッセージ全体
      AS_disp_message(system, wframe, false, game_rank);

      // プレイヤー情報の表示
      for (i = 0; i < PLAYER; i++) {
        sys_x = (-Window::WIDTH / 2) + Icon_W * 2;
        sys_y = (-Window::HEIGHT / 2) + Telop_H * 8 - (Telop_H + 2);

        // リタイアしていなければ、手札の枚数とパス回数を表示
        if (pass_count[i] < RETIRE_P) {
          // 手札の残り枚数を取得して、その値を表示
          card_count[i] = 0;
          for (j = 0; j < CARD_NUM; j++) { if (!player[i][j].used) { card_count[i]++; } }
          CO_disp_number(system, card_count[i], sys_x, sys_y - (Telop_H + 2) * i);

          // パスした回数を表示
          CO_disp_number(system, pass_count[i], sys_x + (Icon_W * 2), sys_y - (Telop_H + 2) * i);

          // 「枚数」
          drawTextureBox(sys_x + Card_W, sys_y - (Telop_H + 2) * i, Icon_W, Telop_H,
            Icon_W * 2, Telop_H * 2, Icon_W, Telop_H,
            system, Color(1, 1, 1));

          // 「回数」
          drawTextureBox(sys_x + Card_W + (Icon_W * 2), sys_y - (Telop_H + 2) * i, Icon_W, Telop_H,
            Icon_W * 3, Telop_H * 2, Icon_W, Telop_H,
            system, Color(1, 1, 1));
        }

        // リタイアしているとき
        else {
          // 「リタイア」
          drawTextureBox(sys_x + Icon_W, sys_y - (Telop_H + 2) * i, Icon_W, Telop_H,
            Icon_W * 3, Telop_H * 1, Icon_W, Telop_H,
            system, Color(1, 1, 1));
        }
      }
    }

    // 場に出ているカードの表示
    for (i = 0; i < CARD_MARK; i++) {
      for (j = num_left[i]; j <= num_right[i]; j++) {
        // 表示位置の設定
        card_x = (Card_W + 2) * (j - 6.5);
        card_y = (Card_H + 2) * (2 - i);

        // カードの表示
        drawTextureBox(card_x, card_y, Card_W, Card_H,
          Card_W * j, Card_H * i, Card_W, Card_H,
          t_card, Color(1, 1, 1));
      }
    }

    // リタイアしてなければ処理を実行
    if (pass_count[player_turn] < RETIRE_P) {

      // プレイヤーと対戦相手で処理を分岐
      switch (player_turn) {

          // プレイヤーの処理
        case DEF_PLAYER:
          mouse_pos = app_env.mousePosition();   // マウス座標の取得

          // 手札の処理
          for (i = 0; i < CARD_NUM; i++) {
            // 手札の表示位置を設定
            card_x = (-Window::WIDTH / 2) + 2 + (Card_W + 2) * i;
            card_y = (-Window::HEIGHT / 2) + 2;

            // フラグのリセット
            isLow = false; isHigh = false; isEnable = false;

            // カードの上にマウスカーソルがあるか判定
            on_mouse = CO_on_mouse(mouse_pos, card_x, card_y, Card_W, Card_H);
            if (on_mouse) {
              // まだ場に出していないカードだったとき
              if (!player[DEF_PLAYER][i].used) {
                // 選択しようとしているカードの記号を取得
                current_mark = player[DEF_PLAYER][i].mark;

                // 場のカードと手札のカードを比較、場に出せるか判定する
                AS_enable_check(isLow, isHigh, isEnable,
                  player[DEF_PLAYER][i].number, num_left[current_mark], num_right[current_mark]);

                // カードが使えたら、カードを回転させる
                if (isEnable) {
                  scale = fabs(sin(angle));
                  angle = fmod(angle + 0.05, M_PI * 2);
                }
              }
            }

            // カードの表示
            if (isEnable) {
              // 回転角で画像を切り替える
              tex_x = (angle < M_PI) ? Card_W * player[DEF_PLAYER][i].number : Card_W * 1;
              tex_y = (angle < M_PI) ? Card_H * player[DEF_PLAYER][i].mark : Card_H * 4;

              // 使用可能なカードの表示
              drawTextureBox(card_x + (Card_W / 2), card_y, Card_W, Card_H,
                tex_x, tex_y, Card_W, Card_H,
                t_card, AS_set_cardcolor(player[DEF_PLAYER][i].used, on_mouse, isEnable),
                0, Vec2f(scale, 1.2), Vec2f(Card_W / 2, 0));
            }
            else {
              // 使用できないカードの表示
              drawTextureBox(card_x, card_y, Card_W, Card_H,
                Card_W * player[DEF_PLAYER][i].number, Card_H * player[DEF_PLAYER][i].mark, Card_W, Card_H,
                t_card, AS_set_cardcolor(player[DEF_PLAYER][i].used, on_mouse, isEnable));
            }

            // カードが使える状態で、左クリックした時の処理
            if (isEnable && app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click.play();   // SE再生

              // 手札から出したことにして、場と手札に結果を反映させる
              if (isLow) { num_left[current_mark]--; }     // 左側に出せるカードの処理
              if (isHigh) { num_right[current_mark]++; }   // 右側に出せるカードの処理
              player[DEF_PLAYER][i].used = true;           // 手札からカードを出した状態にする

              player_turn++;                // 対戦相手のターンにする
              pass_count[DEF_PLAYER] = 0;   // パス回数をリセット
              isCheck = false;              // ゲーム終了判定フラグをリセット
            }

            // 選択できるカードがないときの「パス」を表示
            AS_disp_pass(app_env, t_card, mouse_pos, player_turn, pass_count[DEF_PLAYER], game_rank);
          }

          break;   // プレイヤーの処理 終了

          // 対戦相手の処理
        default:
          // フラグをリセット
          isAct = false; isLow = false; isHigh = false; isCheck = false;

          // 手札の枚数が０でなければ処理を実行
          if (card_count[player_turn] != 0) {
            // 手札から場に出せるカードが見つかった時点で場に出して次のターンにする
            for (i = 0; i < CARD_NUM; i++) {
              // カードの記号を取得
              current_mark = player[player_turn][i].mark;

              // 左右どちらに出せるか判定
              isLow = (player[player_turn][i].number == num_left[current_mark] - 1);
              isHigh = (player[player_turn][i].number == num_right[current_mark] + 1);

              // 場に出せるカードだったら、場に反映させて処理を終了
              if (isLow || isHigh) {
                if (isLow) { num_left[current_mark]--; }     // 左側を更新
                if (isHigh) { num_right[current_mark]++; }   // 右側を更新
                player[player_turn][i].used = true;          // 場に出したことにする
                pass_count[player_turn] = 0;                 // 行動したのでパス回数をリセット

                isAct = true;   // 場にカードを出したので、パスしないようにする
                break;
              }
            }

            // 手札の残り枚数を再確認して、０になったら順位をカウント
            for (j = 0; j < CARD_NUM; j++) { if (!player[player_turn][j].used) { break; } }
            if (j == CARD_NUM) { game_rank++; }

            // 場に出せるカードがなかったらパス回数をカウント
            if (!isAct) { pass_count[player_turn]++; }
          }

          // 行動したら次のターンに進めて、全員が行動したらプレイヤーのターンに戻す
          player_turn++;
          if (player_turn >= PLAYER) { player_turn = 0; }

          break;   // 対戦相手の処理 終了
      }
    }

    // リタイアしたプレイヤーの処理
    else {
      // フラグをリセット
      isAct = false; isLow = false; isHigh = false; isCheck = false;

      // カードを出せなくなるまで出し切る
      for (i = 0; i < CARD_NUM; i++) {
        // 参照しているカードの記号を取得
        current_mark = player[player_turn][i].mark;

        // 左右どちらに出せるか判定
        isLow = (player[player_turn][i].number == num_left[current_mark] - 1);
        isHigh = (player[player_turn][i].number == num_right[current_mark] + 1);

        // 場に出せるカードだったら、場に反映させて処理を終了
        if (isLow || isHigh) {
          if (isLow) { num_left[current_mark]--; }     // 左側を更新
          if (isHigh) { num_right[current_mark]++; }   // 右側を更新
          player[player_turn][i].used = true;          // 場に出したことにする
        }
      }

      // 次のターンにする
      player_turn++;
      if (player_turn >= PLAYER) { player_turn = 0; }
    }

    app_env.update();
  } // 本編　終了

  // リザルト画面
  thema.stop();
  finish.play();
  result_arrange(app_env, system, wframe, game_rank);
}
