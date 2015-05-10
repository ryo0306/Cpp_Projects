//
// 神経衰弱
//

#pragma once
#include "f_nervous.hpp"   // 各ゲーム専用の関数ファイルの読み込み


// 神経衰弱 リザルト
void result_nervous(AppEnv& app_env, Texture& sys, Texture& frm,
  bool& isCont, bool& g_set, short& chain, short p_score, short r_score) {

  // システムメッセージの表示位置
  float sys_x, sys_y;

  // スコアを一時的に保存
  short score[] = {p_score, r_score};

  while (1) {
    if (!app_env.isOpen()) { return; }

    // 左クリックで終了
    if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); break; }

    app_env.setupDraw();

    // システムメッセージの表示
    NV_disp_message(sys, frm, isCont, g_set, chain, score);

    // スコアの表示
    {
      sys_x = (-Window::WIDTH / 2) + Card_W + Icon_W * 2;
      sys_y = (-Window::HEIGHT / 2) + (Telop_H * 4) - 2;

      CO_disp_number(sys, p_score, sys_x, sys_y);                   // プレイヤーの点数
      CO_disp_number(sys, r_score, sys_x, sys_y - (Telop_H + 2));   // 対戦相手の点数
    }

    app_env.update();
  }
}

// 神経衰弱 本編
void game_nervous(AppEnv& app_env) {
  // 乱数の基準値設定
  Random random; random.setSeed((u_int)time(nullptr));

  // 画像の読み込み
  Texture t_card("res/png/c01_standard.png");   // カードの画像
  Texture system("res/png/m01_nv.png");         // システムメッセージ
  Texture wframe("res/png/sys04_frame.png");    // テロップ用フレーム

  // BGM と SE の読み込み
  Media thema(M_THEMA); thema.gain(0.5); thema.looping(true);
  Media intro(M_INTRO); intro.gain(0.5);   // イントロ

  Media click(SE_CLICK); click.gain(0.5);         // クリックしたとき
  Media success(SE_SUCCESS); success.gain(0.25);   // カードのペアを作れた
  Media failure(SE_FAILURE); failure.gain(0.25);   // カードのペアを作れなかった
  Media finish(SE_FINISH); finish.gain(0.25);      // ゲーム終了

  // このゲーム内専用の変数
  short i;                  // 繰り返し処理用
  short open_count = 0;     // カードをめくった回数
  short open_chain = 0;     // 連続でペアを見つけた回数
  short hist_count = 0;     // 履歴に情報を保存した回数
  short field_count = 0;    // 場に残っているカードの枚数

  float card_x, card_y;     // カードの表示位置（始点）
  float sys_x, sys_y;       // システムメッセージの表示位置

  bool isContinue = true;   // 続けてカードをめくれるか
  bool isCheck = false;     // 対戦相手のAI用：履歴の確認
  bool isRandom = false;    // 対戦相手のAI用：検索で見つからないとき、ランダムで選択
  bool game_turn = true;    // 誰のターンか：true = プレイヤー、false = 対戦相手
  bool game_set = true;     // ゲーム終了判定

  // このゲーム内専用の変数：マウス判定用
  Vec2f mouse_pos;   // マウスカーソルの座標
  bool on_mouse;     // マウスカーソルの判定

  // 場に配置されるカードの情報
  struct Card {
    short number;   // カードの数字
    short mark;     // カードの記号
    bool isOpen;    // 表向きになっているか
  };

  // カード情報の生成
  Card field[JOKER_NG] = {};        // 場のカード情報
  short open_hist[MAX_HIST] = {};   // 表にしたカードの履歴
  short prev_1, prev_2;             // 表にしたカードの比較用
  short game_score[] = {0, 0};      // プレイヤーと対戦相手の点数

  // ゲームの初期化
  {
    // 履歴の初期化
    for (i = 0; i < MAX_HIST; i++) { open_hist[i] = -1; }

    // カード情報の初期化
    for (i = 0; i < JOKER_NG; i++) { field[i] = {i % CARD_NUM, i / CARD_NUM, false}; }

    // カードのシャッフル
    for (i = 0; i < JOKER_NG; i++) { std::swap(field[i], field[random.fromZeroToLast(JOKER_NG)]); }

    intro.play();   // イントロ再生
  }


  /* 本編 */
  ////////////////////////////////////////////////////////////////////////////////
  while (1) {
    if (!app_env.isOpen()) { return; }

    // BGM再生：イントロが終わったらループ部分の再生
    if (!intro.isPlaying()) { if (!thema.isPlaying()) { thema.play(); } }

    // カードの残り枚数を確認、２枚未満なら強制的に終了する
    if (game_set) {
      open_count = 0;    // めくった回数をリセット
      field_count = 0;   // カードの残り枚数をリセット

      // 残り枚数をカウント
      for (i = 0; i < JOKER_NG; i++) { if (!field[i].isOpen) { field_count++; } }

      // ２枚未満なら強制終了
      if (field_count < OPEN_ACT) { thema.stop(); break; }
      game_set = false;
    }

    app_env.setupDraw();

    // システムメッセージの表示
    {
      // メッセージ全体
      NV_disp_message(system, wframe, isContinue, game_set, open_chain, game_score);

      // スコアの表示
      {
        sys_x = (-Window::WIDTH / 2) + Card_W + Icon_W * 2;
        sys_y = (-Window::HEIGHT / 2) + (Telop_H * 4) - 2;

        CO_disp_number(system, game_score[DEF_PLAYER], sys_x, sys_y);                  // プレイヤーの点数
        CO_disp_number(system, game_score[DEF_RIVAL], sys_x, sys_y - (Telop_H + 2));   // 対戦相手の点数
      }
    }

    switch (game_turn) {
        // プレイヤーのターン
      case true:
        // マウス座標の取得
        mouse_pos = app_env.mousePosition();

        // カードごとの処理
        for (i = 0; i < JOKER_NG; i++) {
          // カードの表示位置設定
          card_x = (Card_W + 8) * ((i % CARD_NUM) - 6.5);
          card_y = (Card_H + 8) * ((i / CARD_NUM) - 1);

          // カードの上にマウスカーソルがあるか判定
          on_mouse = CO_on_mouse(mouse_pos, card_x, card_y, Card_W, Card_H);
          if (on_mouse && open_count < OPEN_ACT) {
            // まだ表向きになってないカードだったとき
            if (!field[i].isOpen) {
              // 左クリックしたときの処理
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                if (open_count == 0) { click.play(); }   // １枚目をめくったときだけ SE再生

                field[i].isOpen = true;      // 選んだカードを表にする
                open_count++;                // めくった回数をカウント
                open_hist[hist_count] = i;   // めくったカードの場所を履歴に保存
                hist_count++;                // 履歴を保存した回数をカウント
              }
            }
          }
        }

        // カードを２枚めくったとき
        if (open_count == OPEN_ACT) {
          // カードを続けて選択できるとき
          if (isContinue) {
            // 表にしたカードの情報を取得
            prev_2 = open_hist[hist_count - OPEN_ACT];       // 最初にめくったカード
            prev_1 = open_hist[hist_count - OPEN_ACT + 1];   // 次にめくったカード

            // 表にしたカードの数字が同じだったとき
            if (field[prev_2].number == field[prev_1].number) {
              success.play();   // SE再生

              game_score[DEF_PLAYER]++;   // 点数を加算
              open_chain++;               // ペアを見つけた回数をカウント
              hist_count -= OPEN_ACT;     // 履歴に保存した回数をめくった回数だけ戻す
              game_set = true;            // 残り枚数が変化するので、もう一度確認する
            }

            // 違う数字だったとき、カードを選択できないようにする
            else {
              failure.play();   // SE再生
              isContinue = false;

              // 履歴が埋まったらカウンタをリセット
              if (hist_count >= MAX_HIST) { hist_count = 0; }
            }
          }

          // カードが選択できなくなったとき
          else {
            // 左クリックで相手のターンにする
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click.play();
              game_turn = false;   // 対戦相手のターンに設定

              // 数字が違うので、表にしたカードを元に戻す
              field[prev_2].isOpen = false;
              field[prev_1].isOpen = false;

              // 各種カウンタのリセット
              open_chain = 0;
              isContinue = true;
              game_set = true;
            }
          }
        }

        break;   // プレイヤーのターン 終了

        // 対戦相手のターン
      case false:
        // カードを選択できなくなるまで処理を実行
        if (isContinue) {
          switch (open_count) {
              // １枚目：裏向きのカードをランダムに選択
            case 0:
              prev_2 = random.fromZeroToLast(JOKER_NG);

              // ランダムに選択されたカードが裏向きなら決定
              if (!field[prev_2].isOpen) {
                open_count++;   // ２枚目の選択を開始

                // フラグをリセット
                isCheck = false;
                isRandom = true;
              }
              break;

              // ２枚目：履歴を検索、同じ数字が見つからなければランダムに選択
            case 1:
              // １回だけ履歴を検索する
              if (!isCheck) {
                for (i = 0; i < MAX_HIST; i++) {
                  // 履歴に１枚目と同じ数字があれば「とりあえず」決定、ランダム選択を回避
                  if (field[open_hist[i]].number == field[prev_2].number) {
                    prev_1 = open_hist[i];
                    isRandom = false;
                    break;
                  }
                }

                isCheck = true;   // 履歴を確認したので、次の自分のターンまで確認しない
              }

              // 履歴検索に失敗していたら、ランダムにカードを選択
              if (isRandom) { prev_1 = random.fromZeroToLast(JOKER_NG); }

              // 選択されたカードが裏向き、かつ１枚目と違うカードなら確定、そうでなければランダム選択開始
              if (!field[prev_1].isOpen && prev_1 != prev_2) { open_count++; }
              else { isRandom = true; }
              break;

              // カードを２枚選択したとき
            case 2:
              // 選択されたカードを表向きにする
              field[prev_2].isOpen = true;
              field[prev_1].isOpen = true;

              // 選択された２枚が同じ数字だったら点数を加算して、もう一度選択する
              if (field[prev_1].number == field[prev_2].number) {
                game_score[DEF_RIVAL]++;   // 点数を加算
                game_set = true;           // 残り枚数が変化するので、もう一度確認する
              }

              // 同じ数字ではなかったとき、カードを選択できないように設定
              else { isContinue = false; }
              break;
          }
        }

        // カードを選択できなくなったとき
        else {
          // 左クリックでプレイヤーのターンにする
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            click.play();

            // 選択されたカードを元に戻す
            field[prev_2].isOpen = false;
            field[prev_1].isOpen = false;

            // 各種フラグのリセット
            game_turn = true;
            game_set = true;
            isContinue = true;
            isRandom = false;
          }
        }

        break;   // 対戦相手のターン 終了
    }

    // カード画像の表示
    for (i = 0; i < JOKER_NG; i++) {
      // カードの表示位置設定
      card_x = (Card_W + 8) * ((i % CARD_NUM) - 6.5);
      card_y = (Card_H + 8) * ((i / CARD_NUM) - 1);

      // カードの上にマウスカーソルがあるか判定
      on_mouse = CO_on_mouse(mouse_pos, card_x, card_y, Card_W, Card_H);
      if (field[i].isOpen || (app_env.isPressKey('O') && app_env.isPressKey('P'))) {
        // カードが表側のとき
        drawTextureBox(card_x, card_y, Card_W, Card_H,
          Card_W * field[i].number, Card_H * field[i].mark, Card_W, Card_H,
          t_card, Color(1, 1, 1));
      }
      else {
        // カードが裏側のとき
        drawTextureBox(card_x, card_y, Card_W, Card_H,
          Card_W * 1, Card_H * 4, Card_W, Card_H,
          t_card, NV_set_cardcolor(on_mouse));
      }
    }

    app_env.update();
  } // 本編 終了

  // リザルト画面
  finish.play();
  result_nervous(app_env, system, wframe,
    isContinue, game_set, open_chain,
    game_score[DEF_PLAYER], game_score[DEF_RIVAL]);
}
