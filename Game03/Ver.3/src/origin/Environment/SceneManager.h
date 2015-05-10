
#pragma once
#include "../Scene/Title.h"
#include "../Scene/MainGame.h"


class cSceneManager {
  pScene current_scene_;
  pScene last_scene_;
  float alpha_;

  void hideScreen();

public:
  cSceneManager(const pScene&);

  void update();
  void draw();
};
