//
// 汎用関数リスト
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "system.hpp"
#include <vector>


/* 汎用関数 */
////////////////////////////////////////////////////////////////////////////////

// マウスカーソル判定
bool CO_on_mouse(Vec2f pos, float x, float y, float tex_w, float tex_h) {

  if (pos.x() > x && pos.x() < (x + tex_w) &&
      pos.y() > y && pos.y() < (y + tex_h)) {
    return true;
  }
  return false;
}

// 数値の表示
void CO_disp_number(Texture& sys, short val, float x, float y) {

  // 動的配列とイテレータの宣言
  std::vector<short> v;
  std::vector<short>::iterator it;

  // 引数の各桁の値を抽出する
  do {
    v.emplace_back(val % 10);
    val /= 10;
  } while (val > 0);

  // 配列の初期位置を代入
  it = v.begin();

  // 各桁の値を表示
  for (int i = 0; it != v.end(); ++i, ++it) {
    drawTextureBox(x - (Number_W * i), y, Number_W, Telop_H,
      Number_W * (*it), Telop_H * 2, Number_W, Telop_H,
      sys, Color(1, 1, 1));
  }
}


////////////////////////////////////////////////////////////////////////////////
