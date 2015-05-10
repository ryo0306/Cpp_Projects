//
// High & Low専用 関数リスト
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../system/system.hpp"


/* マクロ */
////////////////////////////////////////////////////////////////////////////////

enum HL {
  HIGHLOW = 2,      // 表示させるカードの枚数
  SUCCESS = 1,      // カード選択の成功
  FAILURE = 2,      // カード選択の失敗
  DEF_LEFT = 1,     // 左のカードを選択したとき
  DEF_RIGHT = 2,    // 右のカードを選択したとき

  EFC_GOOD = 10,    // エフェクト表示：高評価
  EFC_GREAT = 20,   // エフェクト表示：さらに高評価

  TIME_FPS = 60,    // １秒あたりの処理回数（Frame Per Second）
  TIME_LIM = 20,    // 制限時間（秒）：TIME_FPS * TIME_LIM
  TIME_REP = 20     // 残り時間回復量の上限
};


/* ゲーム専用関数 */
////////////////////////////////////////////////////////////////////////////////

// システムメッセージの表示
void HL_disp_message(Texture& sys, Texture& frm, bool& update, bool g_set, short& chain) {
  // システムメッセージの表示位置（始点）
  float x, y;

  // メッセージエフェクト用
  static float angle1, angle2, angle3;

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

    if (angle2 == 180) { angle2 = 0; }
    else { angle2 += 0.1; }

    if (!g_set) {
      // カードの選択
      drawTextureBox(x, y, Telop_W, Telop_H,
        0, Telop_H * 4, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
    else {
      // ゲーム終了
      drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
        0, Telop_H * 7, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // ゲームのヒント表示
  if (!g_set) {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (Window::HEIGHT / 2) - (Telop_H * 4) - 2;
    angle3 += 0.5;
    if (angle3 > (Telop_W * 2)) { angle3 = 0; }

    // フレーム表示
    drawTextureBox(x - Number_W, y - (Telop_H / 2), Logo_W + (Number_W * 2), Logo_Hb,
      0, 0, Logo_W, Logo_Hb,
      frm, Color(1, 1, 1));

    // テロップ表示
    if (angle3 < Telop_W) {
      drawTextureBox(x + (Telop_W - angle3), y, angle3, Telop_H,
        0, Telop_H * 5, angle3, Telop_H,
        sys, Color(1, 1, 1));
      drawTextureBox(x, y, Telop_W - angle3, Telop_H,
        angle3, Telop_H * 6, Telop_W - angle3, Telop_H,
        sys, Color(1, 1, 1));
    }
    else {
      drawTextureBox(x + (Telop_W * 2) - angle3, y, angle3 - Telop_W, Telop_H,
        0, Telop_H * 6, angle3 - Telop_W, Telop_H,
        sys, Color(1, 1, 1));
      drawTextureBox(x, y, (Telop_W * 2) - angle3, Telop_H,
        angle3 - Telop_W, Telop_H * 5, (Telop_W * 2) - angle3, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // スコアの表示：数値そのものは別の関数で表示
  // 参照：g_common.cpp -> CO_disp_number();
  {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (-Window::HEIGHT / 2) + Telop_H * 4.5;

    // 「スコア」
    drawTextureBox(x + (Number_W * 3), y, Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // 「ハイスコア」
    drawTextureBox(x, y - (Telop_H + 2), Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 1, Icon_W, Telop_H,
      sys, Color(1, 1, 1));
  }

  // 連続成功回数の表示
  if (chain != 0) {
    x = (-Window::WIDTH / 2) + Icon_W * 6;
    y = (-Window::HEIGHT / 2) + (Telop_H * 4.5);

    // 「CHAIN!!」
    drawTextureBox(x, y, Icon_W, Telop_H,
      Icon_W * 3, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 0.5));

    // 回数の表示
    CO_disp_number(sys, chain, x - (Icon_W / 2), y);
  }

  // リザルト画面の表示
  if (g_set) {
    // ゲーム終了の表示
    {
      x = -(Icon_W + 2);
      y = 0;

      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 2, Telop_H * 1, Icon_W, Telop_H,
        sys, Color(1, 1, 1));
    }

    // ハイスコア更新の表示
  }
}

// カードのカラー変更
Color HL_set_cardcolor(bool& on_mouse) {
  if (!on_mouse) { return Color(1, 1, 1); }   // カードの上にカーソルがない
  else { return Color(1, 1, 0.5); }           // ある
}

// 時間ゲージのカラー変更
Color HL_set_timecolor(short& time) {
  float temp = (float)time / (TIME_FPS * TIME_LIM);
  short time0 = temp * 100;

  if (time0 > 40) { return Color(0.5, 1, 0.5); }
  else if (time0 > 20) { return Color(1, 1, 0.5); }
  else { return Color(1, 0.5, 0.5); }
}

// カードの表示と選択
bool HL_disp_card(Texture& card, Vec2f& pos, short& val, float x) {
  float scale = 1.0;
  Color card_color = Color(1, 1, 1);
  bool on_card = false;

  // マウス判定
  if (CO_on_mouse(pos, x - (Card_W * 0.75), -(Card_H * 0.75), Card_W * 1.5, Card_H * 1.5)) {
    scale = 1.2;
    card_color = Color(1, 1, 0.5);
    on_card = true;
  }

  // カードの表示
  drawTextureBox(x, 0, Card_W * 1.5, Card_H * 1.5,
    Card_W * (val % CARD_NUM), Card_H * (val / CARD_NUM), Card_W, Card_H,
    card, card_color,
    0, Vec2f(scale, scale), Vec2f(Card_W * 0.75, Card_H * 0.75));

  return on_card;
}

// パスの表示と選択
int HL_disp_pass(AppEnv& app, Texture& card, Vec2f& m_pos, short left, short right) {
  float x, y;        // カードの表示位置
  float scale;       // 画像の縮尺
  Color pass_card;   // 色情報

  x = (Card_W / 2); y = (Card_H / 2);

  // コマンドが選択出来る状態
  if (CO_on_mouse(m_pos, -x, -y, Card_W, Card_H)) {
    // 左クリックしたとき
    if (app.isPushButton(Mouse::LEFT)) {
      app.flushInput();

      // 左右のカードが同じ数字ならスコア追加
      return (left == right) ? SUCCESS : FAILURE;
    }
    pass_card = Color(1, 1, 0.5);
    scale = 1.2;
  }
  else { pass_card = Color(1, 1, 1); scale = 1.0; }

  // 「パス」の画像を表示
  drawTextureBox(0, 0, Card_W, Card_H,
    Card_W * 2, Card_H * 4, Card_W, Card_H,
    card, pass_card, 0, Vec2f(scale, scale), Vec2f(x, y));

  return 0;
}

// カードを選択したときの成否判定
int HL_click_check(short choice, short left, short right) {
  switch (choice) {
    case  DEF_LEFT: return (left > right) ? SUCCESS : FAILURE;
    case DEF_RIGHT: return (left < right) ? SUCCESS : FAILURE;
    default: return FAILURE;
  }
}


////////////////////////////////////////////////////////////////////////////////
