
#pragma once
#include "environment.h"
#include "data_depot.h"
#include "../Literal/literal_scene.hpp"
#include "../Scene/title.h"
#include "../Scene/strategy.h"
#include "../Scene/battle.h"
#include "../Scene/result.h"
#include "../Scene/save_load.h"
#include "../Scene/pause.h"


class cSceneManager {
  cTitle    title_;
  cStrategy strategy_;
  cBattle   battle_;
  cResult   result_;
  cSaveLoad save_load_;
  cPause    pause_;

  enum FadeState {
    FadeIn,
    FadeOut,
    Stop,
  };
  typedef FadeState  fade;

  struct {
    fade  state_;
    float alpha_;
  } anime;

  bool is_init_;

  bool& is_pause_;
  scene& now_scene_;
  scene& next_scene_;

  void sceneShift();

  bool isPlayingAnime();
  void fadeShift();
  void fadeOut();
  void fadeIn();

  void blackOut();

public:
  cSceneManager();

  void update();
  void init();
  void draw();

  bool isContinue();
};
