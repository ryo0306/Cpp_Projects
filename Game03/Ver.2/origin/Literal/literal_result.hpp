
#pragma once
#include "../System/base.hpp"


namespace literal_result {

  // 描画状態
  enum ClickState {
    FadeIn,
    ClickWait,
    FadeOut,
  };

  // 描画基準位置
  enum DrawPos {
    ResultLogo_Y = HEIGHT / 2 - 150,

    Score_X = 0,
    Score_Y = 0,
  };

  // 描画サイズ
  enum DrawSize {
  };

  // ゲームクリア時のランク評価基準
  enum ClearRank {
    S = 30,
    A = 60,
    B = 90,
  };
}

typedef literal_result::ClickState  state;
typedef literal_result::DrawPos     pos;
typedef literal_result::DrawSize    size;
typedef literal_result::ClearRank   rank;
