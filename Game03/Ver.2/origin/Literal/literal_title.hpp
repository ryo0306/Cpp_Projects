
#pragma once
#include "../System/base.hpp"


namespace literal_title {

  // �`����
  enum ClickState {
    Start,
    Blink_1,
    FadeIn_1,
    Mode_Select,   // �N���b�N�\
    Blink_2,
    FadeOut_1,
    FadeIn_2,
    Dif_Select,    // �N���b�N�\
    Blink_3,
    SceneChange,

    FadeOut_2,
  };

  // �摜�̐؂���A�`��T�C�Y
  enum Size {
    Logo_W = 256,
    Logo_H = 48,

    Sea_Fog_WH = 512,
    Map_WH = 1024,
  };

  // �`��ʒu
  enum DrawPos {
    Center = 0,

    LogoPos_Y = HEIGHT / 4,
    TelopPos_Y = LogoPos_Y - 100,

    ButtonPos_Y = -HEIGHT / 6,
    ButtonPos_Span = 90,
    StringPos_Y = 15,
  };

  // �V�X�e���ŗL
  enum System {
    ButtonDrawMax = 3,

    LogoBlink_Time = 50,
    LogoBlink_Span = LogoBlink_Time / 5,

    ScrollEndPoint = Sea_Fog_WH,

    JumpHeight = 15,
    AngleSpeed = 1,

    StartMoney = 1000,
  };

  // �����z��p�f�[�^
  enum StringData {
    Mouse,
    Click,
    Back,

    NewGame,
    Continue,
    Quit,

    Easy,
    Normal,
    Hard,

    All_Text,
  };
}
