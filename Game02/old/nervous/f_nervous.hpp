//
// 神経衰弱専用 関数リスト
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../system/system.hpp"


/* マクロ */
////////////////////////////////////////////////////////////////////////////////

#define DEF_PLAYER   0   // プレイヤー情報
#define DEF_RIVAL    1   // 対戦相手の情報


/* ゲーム専用関数 */
////////////////////////////////////////////////////////////////////////////////

// システムメッセージの表示
void NV_disp_message(Texture& sys, Texture& frm, bool& isCont, bool& g_set, short& chain, short score[]) {
  float x, y;   // システムメッセージ表示位置（始点）
  short z;      // ランク表示用

  // メッセージエフェクト用
  static float angle1, angle2;
  static short blink;

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

    blink += 1;
    if (blink == 60) { blink = 0; }

    if (!g_set) {
      if (isCont) {
        // カードの選択
        drawTextureBox(x, y, Telop_W, Telop_H,
          0, Telop_H * 4, Telop_W, Telop_H,
          sys, Color(1, 1, 1, (blink / 30)));
      }
      else {
        // 次のターンにする
        drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
          0, Telop_H * 5, Telop_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }
    else {
      // ゲーム終了
      drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
        0, Telop_H * 6, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // スコアの表示：数値そのものは別の関数で表示
  // 参照：g_common.cpp -> CO_disp_number();
  {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (-Window::HEIGHT / 2) + Telop_H * 5;

    // 「点数」
    drawTextureBox(x, y, Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // 「プレイヤー」
    drawTextureBox(x + Icon_W * 0.25, y - (Telop_H + 2) * 1, Icon_W, Telop_H,
      Icon_W * 1, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // 「相手」
    drawTextureBox(x + Icon_W * 0.25, y - (Telop_H + 2) * 2, Icon_W, Telop_H,
      Icon_W * 2, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));
  }

  // 連続発見回数の表示
  if (chain != 0 && !g_set) {
    x = (-Window::WIDTH / 2) + Card_W + Icon_W * 5;
    y = (-Window::HEIGHT / 2) + (Telop_H * 4) - 2;

    // 「CHAIN!!」
    drawTextureBox(x, y, Icon_W, Telop_H,
      Icon_W * 3, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 0.5));

    // 回数の表示
    CO_disp_number(sys, chain, x - Icon_W, y);
  }

  // リザルト画面の処理
  if (g_set) {
    // 順位の表示
    {
      x = (-Window::WIDTH / 2) + Card_W + Telop_W + Icon_W;
      y = (-Window::HEIGHT / 2) + (Telop_H * 5) - (Telop_H + 2);

      // 「順位」
      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 0, Telop_H * 1, Icon_W * 2, Telop_H,
        sys, Color(1, 1, 1));

      // 点数を比較して、相手のほうがよかったら２位を表示
      {
        z = 1;
        if (score[DEF_PLAYER] < score[DEF_RIVAL]) { z += 1; }

        drawTextureBox(x + (Icon_W * 2) + Number_W, y, Number_W * 2, Telop_H,
          Number_W * z, Telop_H * 2, Number_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }

    // ゲーム終了の表示
    {
      x = -Icon_W;
      y = 0;

      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 2, Telop_H * 1, Icon_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }
}

// カードのカラー変更
Color NV_set_cardcolor(bool& on_mouse) {
  if (!on_mouse) { return Color(1, 1, 1); }   // カードの上にカーソルがない
  else { return Color(1, 1, 0.5); }           // ある
}


////////////////////////////////////////////////////////////////////////////////
