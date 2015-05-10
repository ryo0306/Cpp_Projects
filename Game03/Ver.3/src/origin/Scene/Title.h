
#pragma once
#include "SceneBase.hpp"
#include "../Resource/Media.h"
#include "../Object/Anim.h"


class cTitle : public cSceneBase {
  enum SpriteID : short {
    Back_Ground,
    All_Sprite,
  };

  enum MusicID : short {
    M_Title = MUSIC_ID,
    All_Media,
  };

  enum SeID : short {
    Click = SE_ID,

    PREFIX,   // XXX: ‚±‚ÌID‚ÍŽg—p‚µ‚È‚¢
    All_SE = PREFIX - SE_ID,
  };

  struct AnimationPrm {
    short timer_;
    short blink_;

    const Color GROUND_COLOR_;

    AnimationPrm() :
      timer_(0), blink_(0),
      GROUND_COLOR_(0.8f, 0.6f, 0.0f) {}
  } anime;

  cAnim anim_;

  void spriteSetup();
  void mediaSetup();

  void drawBackGround();

public:
  cTitle();

  void update();
  void draw();

  pScene nextScene();
};
