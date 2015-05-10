//
// 七並べ専用 関数リスト
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../system/system.hpp"


/* マクロ */
////////////////////////////////////////////////////////////////////////////////

#define DEF_PLAYER   0   // プレイヤー情報


/* ゲーム専用関数 */
////////////////////////////////////////////////////////////////////////////////

// システムメッセージの表示
void AS_disp_message(Texture& sys, Texture& frm, bool game_set, short& rank) {
  short i;      // 繰り返し処理用
  float x, y;   // システムメッセージの表示位置（始点）

  static float angle1, angle2;   // メッセージエフェクト用

  // ゲームモードの表示
  {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (Window::HEIGHT / 2) - Telop_H * 2;
    angle1 += 0.5;
    if (angle1 > Telop_W) { angle1 = 0; }

    // フレーム表示
    drawTextureBox(x - Number_W, y - (Telop_H / 2), Logo_W + (Number_W * 2), Logo_Hb,
      0, 0, Logo_W, Logo_Hb,
      frm, Color(1, 1, 1));

    // テロップ表示
    drawTextureBox(x, y, Telop_W, Telop_H,
      angle1, Telop_H * 3, Telop_W, Telop_H,
      sys, Color(1, 1, 1));
  }

  // 操作方法の表示
  {
    x = (-Window::WIDTH / 2) + Card_W + Telop_W + Icon_W;

    if (!game_set) {
      // カードの選択
      drawTextureBox(x, y, Telop_W, Telop_H,
        0, Telop_H * 4, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
    else {
      if (angle2 == 180) { angle2 = 0; }
      else { angle2 += 0.1; }

      // クリックで終了
      drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
        0, Telop_H * 6, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // スコアの表示：数値そのものは別の関数で表示
  // 参照：g_common.cpp -> CO_disp_number();
  if (!game_set) {
    x = (-Window::WIDTH / 2) + Icon_W * 0.5;
    y = (-Window::HEIGHT / 2) + Telop_H * 8;

    // 「手札」
    drawTextureBox(x + Icon_W, y, Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // 「パス回数」
    drawTextureBox(x + Icon_W * 3, y, Icon_W, Telop_H,
      Icon_W * 1, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // 情報の表示
    for (i = 0; i < PLAYER; i++) {
      if (i == DEF_PLAYER) {
        // 「プレイヤー」
        drawTextureBox(x, y - (Telop_H + 2), Icon_W, Telop_H,
          Icon_W * 2, Telop_H * 0, Icon_W, Telop_H,
          sys, Color(1, 1, 1));
      }
      else {
        // 「相手」
        drawTextureBox(x, y - (Telop_H + 2) * (1 + i), Icon_W, Telop_H,
          Icon_W * 3, Telop_H * 0, Icon_W, Telop_H,
          sys, Color(1, 1, 1));

        // 「対戦相手の誰か」を表示
        drawTextureBox(x + (Number_W * 5), y - (Telop_H + 2) * (1 + i), Number_W, Telop_H,
          Number_W * (10 + i), Telop_H * 2, Number_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }
  }

  // リザルト画面の表示
  if (game_set) {
    // ゲーム終了の表示
    {
      x = -(Icon_W + 2);
      y = 0;

      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 2, Telop_H * 1, Icon_W, Telop_H,
        sys, Color(1, 1, 1));
    }

    // 順位の表示
    {
      x = -(Icon_W * 1.5);
      y = -(Telop_H + 2);

      // 「順位」
      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 0, Telop_H * 1, Icon_W * 2, Telop_H,
        sys, Color(1, 1, 1));

      // 値に対応した順位の表示
      if (rank != 5) {
        drawTextureBox(x + (Icon_W * 3), y, Number_W * 2, Telop_H,
          Number_W * rank, Telop_H * 2, Number_W, Telop_H,
          sys, Color(1, 1, 1));
      }

      // リタイアしたとき
      else {
        drawTextureBox(x + (Icon_W * 2), y, Icon_W, Telop_H,
          Icon_W * 3, Telop_H * 1, Icon_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }
  }
}

// パスの表示と選択
void AS_disp_pass(AppEnv& app_env, Texture& card, Vec2f& m_pos, short& turn, short& pass, short& rank) {

  float x, y;        // 画像を表示する座標
  Color pass_card;   // 色情報

  x = (Window::WIDTH / 2) - (Card_W + 2);
  y = (-Window::HEIGHT / 2) + 2;

  // コマンドが選択出来る状態
  if (CO_on_mouse(m_pos, x, y, Card_W, Card_H)) {

    // パス回数が０
    if (pass == 0) { pass_card = Color(1, 1, 0.75); }
    // パス回数が０ではない
    else { pass_card = Color(1, 0.75, 0.75); }

    // 左クリックしたらパス回数を増やして次のターンにする
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      turn += 1;
      pass += 1;

      // リタイアになるまでパスしたとき
      if (pass == RETIRE_P) { rank = RETIRE_G; }
    }
  }
  else { pass_card = Color(0.5, 0.5, 0.5); }

  // 「パス」の画像を表示
  drawTextureBox(x, y, Card_W, Card_H,
    Card_W * 2, Card_H * 4, Card_W, Card_H,
    card, pass_card);
}

// カードのカラー変更
Color AS_set_cardcolor(bool used, bool& on_mouse, bool& is_E) {
  // カードが使用済みではないとき
  if (!used) {
    // カードの上にマウスカーソルがあるか判定
    if (!on_mouse) { return Color(1, 1, 1); }   // カードの上にない
    else {
      if (is_E) { return Color(1, 1, 0.5); }    // カードが使える
      return Color(1, 0.5, 0.5);                // 使えない
    }
  }

  // 使用済み
  else { return Color(1, 1, 1, 0.5); }
}

// カードのフラグ判定
void AS_enable_check(bool& is_L, bool& is_H, bool& is_E, short num, short left, short right) {
  // 手札のカードが左端、右端のどちらに出せるか判定
  is_L = (num == left - 1);    // 左端に出せれば true
  is_H = (num == right + 1);   // 右端に出せれば true

  // どちらかに出せれば、使用可能にする
  is_E = (is_L || is_H);
}


////////////////////////////////////////////////////////////////////////////////
