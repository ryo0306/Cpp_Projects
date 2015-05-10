
#pragma once
#include "environment.h"
#include "resource.h"
#include "scene_manager.h"


class cGamePlayer {

  cSceneManager m_manager;
  bool m_is_escape;

public:

  cGamePlayer() : m_is_escape(false) {
    res::get();
    m_manager.start(std::make_shared<cTitle>());
  }

  void update() {
    m_manager.update();
  }

  void draw() {
    env::get()->begin();
    m_manager.draw();
    env::get()->end();
  }

  bool isRunning() {
    m_is_escape = env::get()->isPushKey(GLFW_KEY_ESCAPE);
    return env::get()->isOpen() && !m_is_escape;
  }
};
