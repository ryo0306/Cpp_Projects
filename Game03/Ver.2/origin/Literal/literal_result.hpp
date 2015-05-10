
#pragma once
#include "../System/base.hpp"


namespace literal_result {

  // �`����
  enum ClickState {
    FadeIn,
    ClickWait,
    FadeOut,
  };

  // �`���ʒu
  enum DrawPos {
    ResultLogo_Y = HEIGHT / 2 - 150,

    Score_X = 0,
    Score_Y = 0,
  };

  // �`��T�C�Y
  enum DrawSize {
  };

  // �Q�[���N���A���̃����N�]���
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
