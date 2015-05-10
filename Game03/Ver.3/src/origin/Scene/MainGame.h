
#pragma once
#include "SceneBase.hpp"
#include "../Resource/Media.h"


class cMain : public cSceneBase {
  enum SpriteID : short {
    Back_Ground,
    Chip_01, Chip_02, Chip_03, Chip_04,
    Chip_05, Chip_06, Chip_07, Chip_08,
    Chip_09, Chip_10, Chip_11, Chip_12,

    All_Sprite,
  };

  enum MediaID : short {
    M_Thema = MUSIC_ID,
    All_Media,
  };

  struct {
    float timer_;
    short blink_;
  } anime;

  void spriteSetup();
  void mediaSetup();

  void drawBackGround();

public:
  cMain();

  void update();
  void draw();

  pScene nextScene();
};
