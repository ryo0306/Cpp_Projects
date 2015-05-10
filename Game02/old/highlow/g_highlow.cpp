//
// High & Low
//

#pragma once
#include "f_highlow.hpp"   // 各ゲーム専用の関数ファイルの読み込み
#include <fstream>


// High & Low リザルト
void result_highlow(AppEnv& app_env, Texture& sys, Texture& frm,
  bool& upd, short& chain, int& new_sc, int& old_sc) {

  // システムメッセージの表示位置（始点）
  float sys_x, sys_y;

  // クリックを受け付けない時間：１秒
  short stop_time = TIME_FPS;

  while (1) {
    if (!app_env.isOpen()) { return; }

    // １秒間はクリック操作を受け付けない
    if (stop_time != 0) { --stop_time; }
    else {
      // 左クリックでゲーム終了
      if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); break; }
    }

    app_env.setupDraw();

    // システムメッセージの表示
    {
      // メッセージ全体
      HL_disp_message(sys, frm, upd, true, chain);

      // 表示位置の設定
      sys_x = (-Window::WIDTH / 2) + Icon_W * 3;
      sys_y = (-Window::HEIGHT / 2) + Telop_H * 4.5;

      // スコアとハイスコアの表示
      CO_disp_number(sys, new_sc, sys_x, sys_y);
      CO_disp_number(sys, old_sc, sys_x, sys_y - (Telop_H + 2));
    }

    app_env.update();
  }
}

// High & Low 本編
void game_highlow(AppEnv& app_env) {
  // 乱数の基準値設定
  Random r; r.setSeed((u_int)time(nullptr));

  // 画像の読み込み
  Texture t_card("res/png/c01_standard.png");   // カードの画像
  Texture system("res/png/m03_hl.png");         // システムメッセージ
  Texture wframe("res/png/sys04_frame.png");    // テロップ用フレーム

  // BGM と SE の読み込み
  Media thema(M_THEMA); thema.gain(0.5); thema.looping(true);
  Media intro(M_INTRO); intro.gain(0.5);   // イントロ

  Media success(SE_SUCCESS); success.gain(0.25);   // クリック成功
  Media failure(SE_FAILURE); failure.gain(0.25);   // クリック失敗
  Media finish(SE_FINISH); finish.gain(0.25);      // ゲーム終了

  // このゲーム内専用の変数
  short i;                  // 繰り返し処理用
  short card_count = 0;     // 現在参照しているカード配列の場所
  short click_chain = 0;    // カードを選択したときの連続成功回数
  short time_bonus = 0;     // クリック成功したときの残り時間回復量
  short isAct, reAct;       // クリックしたときの成否判定
  short c_left, c_right;    // カードの数字

  int game_score = 0;       // 現在のスコア
  int hi_score = 0;         // これまでの最高得点（データがあれば開始時に読み込み）

  float sys_x, sys_y;       // システムメッセージの表示位置（始点）

  bool isReset = true;      // カードを全て使い切ったときのリセット判定
  bool update = false;      // ハイスコア更新フラグ

  // このゲーム内専用の変数：マウス判定用
  Vec2f mouse_pos;   // マウスカーソルの座標

  // このゲーム内専用の変数：エフェクト処理用
  short isClick;         // クリック判定の保存用
  short fade_time = 0;   // フェードアウト処理を続ける時間

  float fade_out;        // テロップのフェードアウト
  float efc_x, efc_y;    // エフェクト用テロップの切り取り位置
  float efc_scale;       // エフェクト用テロップの縮尺

  Color efc_color;       // エフェクト用カラー

  // ゲーム終了までの制限時間
  short time_count = TIME_FPS * TIME_LIM;

  // 場のカード情報
  short cardinfo[JOKER_NG] = {};

  // ゲームの初期化
  {
    // ハイスコアの読み込み
    std::ifstream fstr("res/score.txt");
    if (fstr) { fstr >> hi_score; }
    else { hi_score = 0; }

    intro.play();   // イントロ再生
  }


  /* 本編 */
  ////////////////////////////////////////////////////////////////////////////////
  while (1) {
    if (!app_env.isOpen()) { return; }

    // BGM再生：イントロが終わったらループ部分の再生
    if (!intro.isPlaying()) { if (!thema.isPlaying()) { thema.play(); } }

    // カードを使い切ったらもう一度シャッフルして最初のカードから始める
    if (isReset) {
      for (i = 0; i < JOKER_NG; i++) { cardinfo[i] = i; }
      for (i = 0; i < JOKER_NG; i++) { std::swap(cardinfo[i], cardinfo[r.fromZeroToLast(JOKER_NG)]); }
      isReset = false;
      card_count = 0;
    }

    // 制限時間の処理
    {
      if (time_count <= 0) { thema.stop(); break; }   // 時間切れならゲーム終了
      time_count--;
    }

    mouse_pos = app_env.mousePosition();   // マウス座標の取得

    app_env.setupDraw();

    // システムメッセージの表示
    {
      // メッセージ全体
      HL_disp_message(system, wframe, update, false, click_chain);

      // 表示位置の設定
      sys_x = (-Window::WIDTH / 2) + Icon_W * 3;
      sys_y = (-Window::HEIGHT / 2) + Telop_H * 4.5;

      // スコアとハイスコアの表示
      CO_disp_number(system, game_score, sys_x, sys_y);
      CO_disp_number(system, hi_score, sys_x, sys_y - (Telop_H + 2));
    }

    // 残り時間の表示
    {
      // 表示位置の設定
      sys_x = (-Window::WIDTH / 2) + Card_W;
      sys_y = (-Window::HEIGHT / 2) + Telop_H * 1.5;

      // 「残り時間」
      drawTextureBox(sys_x, sys_y, Icon_W, Telop_H,
        Icon_W * 3, Telop_H * 2, Icon_W, Telop_H,
        system, Color(1, 1, 1));

      // 残り時間の表示
      drawFillBox(sys_x + Icon_W, sys_y, (TIME_LIM * 40), Telop_H,
        Color(0.5, 0.5, 0.5));
      drawFillBox(sys_x + Icon_W, sys_y, ((float)time_count / TIME_FPS) * 40, Telop_H,
        HL_set_timecolor(time_count));
    }

    // 左右それぞれの数字を取得
    if (card_count < (JOKER_NG - 1)) {
      c_left = cardinfo[card_count];
      c_right = cardinfo[card_count + 1];
    }

    // カードの処理
    {
      isAct = 0; reAct = 0;
      if (HL_disp_card(t_card, mouse_pos, c_left, -(Card_W * 3))) { isAct = DEF_LEFT; }
      if (HL_disp_card(t_card, mouse_pos, c_right, (Card_W * 3))) { isAct = DEF_RIGHT; }

      // カードの上にマウスカーソルがあれば、クリックできるようにする
      if (isAct) {
        // 左クリックした時の処理
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();

          // クリック判定：成功だったらスコア獲得
          reAct = HL_click_check(isAct, c_left % CARD_NUM, c_right % CARD_NUM);
          if (reAct == SUCCESS) {
            // クリック成功回数をカウント、スコアを加点する
            click_chain++;
            game_score += 1 + (click_chain / 10);

            // 成功回数に応じた、残り時間を回復
            time_bonus = click_chain;
            if (time_bonus > TIME_REP) { time_bonus = TIME_REP; }
            time_count += time_bonus;

            success.play();   // SE再生
          }

          // 小さいカードだったとき
          else {
            click_chain = 0;          // 成功回数のカウントをリセット
            time_count -= TIME_REP;   // 残り時間を減らす

            failure.play();   // SE再生
          }

          fade_time = 2 * TIME_FPS;   // エフェクトを２秒間表示
          isClick = reAct;            // 判定結果を次のクリックまで保存
          card_count += HIGHLOW;      // 次のカードを表示する
        }
      }
    }

    // パスの処理
    {
      // パスをクリックしたとき、処理を実行
      reAct = HL_disp_pass(app_env, t_card, mouse_pos, c_left % CARD_NUM, c_right % CARD_NUM);
      if (reAct) {
        switch (reAct) {
            // パス成立
          case SUCCESS:
            // クリック成功回数をカウント、スコアを加点する
            click_chain++;
            game_score += 1 + (click_chain / 10);

            // 成功回数に応じた、残り時間を回復
            time_bonus = click_chain;
            if (time_bonus > TIME_REP) { time_bonus = TIME_REP; }
            time_count += time_bonus;

            success.play();   // SE再生
            break;

            // パス失敗
          case FAILURE:
            click_chain = 0;          // 成功回数のカウントをリセット
            time_count -= TIME_REP;   // 残り時間を減らす

            failure.play();   // SE再生
            break;
        }

        fade_time = 2 * TIME_FPS;   // エフェクトを２秒間表示
        isClick = reAct;            // 判定結果を次のクリックまで保存
        card_count += HIGHLOW;      // 次のカードを表示する
      }
    }

    // カードを使い切ったらシャッフルできるようにする
    if (card_count >= (JOKER_NG - HIGHLOW)) { isReset = true; }

    // カードをクリックしたときにエフェクト処理開始
    if (fade_time != 0) {
      --fade_time;
      fade_out = ((float)fade_time / TIME_FPS) / 2;

      // エフェクトの設定
      switch (isClick) {
          // 成功
        case SUCCESS:
          efc_x = Icon_W; efc_y = 0;

          if (click_chain < EFC_GOOD) {
            efc_color = Color(1, 1, 1, fade_out);
            efc_scale = 1.0;
          }
          else if (click_chain < EFC_GREAT) {
            efc_color = Color(1, 1, 0.5, fade_out);
            efc_scale = 1.25;
          }
          else {
            efc_x = Icon_W * 2;
            efc_color = Color(1, 0.5, 0.5, fade_out);
            efc_scale = 1.5;
          }
          break;

          // 失敗
        case FAILURE:
          efc_x = Icon_W * 3; efc_y = Telop_H;
          efc_color = Color(0.8, 0.8, 1, fade_out);
          efc_scale = 1;
          break;
      }

      // エフェクト表示
      drawTextureBox(Number_W, (Telop_H * 4) - (Telop_H * fade_out), Icon_W, Telop_H,
        efc_x, efc_y, Icon_W, Telop_H,
        system, efc_color,
        0, Vec2f(efc_scale, efc_scale), Vec2f(Icon_W / 2, Telop_H / 2));
    }

    app_env.update();
  } // 本編 終了

  // ハイスコアの保存
  update = false;
  if (game_score > hi_score) {
    hi_score = game_score;
    update = true;
    std::ofstream fstr("res/score.txt");
    if (fstr) { fstr << hi_score; }
  }

  // リザルト画面の処理
  finish.play();
  result_highlow(app_env, system, wframe, update, click_chain, game_score, hi_score);
}
