
#include "scene_manager.h"


// FIXME: 画面切り替えのエフェクトが未実装
void cSceneManager::sceneShift() {
  if (!m_current_scene->sceneFinish()) { return; }

  m_next_scene = m_current_scene->nextScene();
  m_current_scene = m_next_scene;

  m_next_scene = nullptr;
}


void cSceneManager::hideScreen() {
  drawFillBox(-WIDTH / 2, -HEIGHT / 2,
              WIDTH, HEIGHT,
              Color(0, 0, 0, m_alpha));
}


void cSceneManager::start(std::shared_ptr<cScene> start_scene) {
  m_alpha = 0.0f;
  m_current_scene = start_scene;
}


void cSceneManager::update() {
  sceneShift();
  m_current_scene->update();
}


void cSceneManager::draw() {
  m_current_scene->draw();
  hideScreen();
}
