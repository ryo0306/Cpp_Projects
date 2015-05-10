
#include "../FrameWork/scene_manager.h"
#include "main_game.h"


cMainGame::cMainGame() {
}


void cMainGame::update() {
  if (env::get()->isPushButton(Mouse::LEFT)) {
    m_is_finish = true;
  }
}


void cMainGame::draw() {
  drawFillCircle(0, 0, 100, 100, 50, Color::yellow);
}


SceneState cMainGame::getScene() const {
  return SceneState::MAIN_GAME;
}


std::shared_ptr<cScene> cMainGame::nextScene() const {
  return nullptr;
}
