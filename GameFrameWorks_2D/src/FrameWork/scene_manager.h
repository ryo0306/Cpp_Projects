
#pragma once
#include <memory>
#include "environment.h"
#include "../Scene/title.h"
#include "../Scene/main_game.h"

// TODO: édólïœçXÇ∑ÇÈÇ©Ç‡

class cSceneManager {

  float m_alpha;
  std::shared_ptr<cScene> m_current_scene;
  std::shared_ptr<cScene> m_next_scene;

  void sceneShift();
  void hideScreen();

public:

  cSceneManager() = default;

  void start(std::shared_ptr<cScene> start);

  void update();
  void draw();
};
