
#pragma once
#include "../System/base.hpp"


namespace literal_title {

  // 描画状態
  enum ClickState {
    Start,
    Blink_1,
    FadeIn_1,
    Mode_Select,   // クリック可能
    Blink_2,
    FadeOut_1,
    FadeIn_2,
    Dif_Select,    // クリック可能
    Blink_3,
    SceneChange,

    FadeOut_2,
  };

  // 画像の切り取り、描画サイズ
  enum Size {
    Logo_W = 256,
    Logo_H = 48,

    Sea_Fog_WH = 512,
    Map_WH = 1024,
  };

  // 描画位置
  enum DrawPos {
    Center = 0,

    LogoPos_Y = HEIGHT / 4,
    TelopPos_Y = LogoPos_Y - 100,

    ButtonPos_Y = -HEIGHT / 6,
    ButtonPos_Span = 90,
    StringPos_Y = 15,
  };

  // システム固有
  enum System {
    ButtonDrawMax = 3,

    LogoBlink_Time = 50,
    LogoBlink_Span = LogoBlink_Time / 5,

    ScrollEndPoint = Sea_Fog_WH,

    JumpHeight = 15,
    AngleSpeed = 1,

    StartMoney = 1000,
  };

  // 文字配列用データ
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
