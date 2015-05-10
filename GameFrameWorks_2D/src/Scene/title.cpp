
#include "../FrameWork/scene_manager.h"
#include "title.h"


cTitle::cTitle() {
}


void cTitle::update() {
  if (env::get()->isPushButton(Mouse::LEFT)) {
    m_is_finish = true;
  }
}


void cTitle::draw() {
  drawFillCircle(0, 0, 50, 50, 50, Color::white);
}


SceneState cTitle::getScene() const {
  return SceneState::TITLE;
}


std::shared_ptr<cScene> cTitle::nextScene() const {
  return std::make_shared<cMainGame>();
}
