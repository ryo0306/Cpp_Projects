
#pragma once
#include "environment.h"
#include <array>


namespace literal_color {

  // TIPS: �`�掞�Ɏw��ł���F�̈ꗗ
  enum PaintColor {
    White,
    LightGray,
    DarkGray,
    Black,

    Red,
    Carmine,
    Pink,
    Purple,
    DarkBlue,
    Blue,
    SkyBlue,
    LightGreen,
    Green,
    Yellow,
    Orange,
    Brown,

    // TIPS: �z��T�C�Y�w��p�f�[�^
    ColorVariation,
    // DANGER: ������艺�Ƀf�[�^�����Ȃ�����
  };
}

typedef literal_color::PaintColor  paint;


class cDrawGadget {
  // TIPS: ���̃N���X�͐錾���Ȃ�
  cDrawGadget() = delete;

  // TIPS: �����ł����g��Ȃ��萔�̈ꗗ
  static enum {
    WindowFrame = 4,
    GaugeFrame = 2,
    PointSize = 4,
  };

  static bool is_init_;
  static std::array<Color, paint::ColorVariation> color_data;

public:
  // �F���̏�����
  // TIPS: �Q�[���N�����ɂP�񂾂��Ă�
  static void colorDataInit();

  // �E�B���h�E�̕`��
  static void window(const Vec2f& pos,
                     const Vec2f& size,
                     const Color& color,
                     const float& alpha = 1.0f);

  // �Q�[�W�̕`��
  static void gauge(const float& per,
                    const Vec2f& pos,
                    const Vec2f& size,
                    const Color& color);

  // �w�肳�ꂽ�F�f�[�^���o��
  // TIPS: �����x���w�肵�Ȃ���Εs�����ȃf�[�^��Ԃ�
  static const Color color(const short& color_num,
                           const float& alpha = 1.0f);
};

typedef cDrawGadget  draw;
