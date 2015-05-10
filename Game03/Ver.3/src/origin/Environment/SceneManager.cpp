
#include "Environment.h"
#include "SceneManager.h"


cSceneManager::cSceneManager(const pScene& start) :
current_scene_(start),
alpha_(0.0f) {
}

void cSceneManager::update() {
  current_scene_->update();
}

void cSceneManager::draw() {
  current_scene_->draw();
  if (alpha_) hideScreen();
}

void cSceneManager::hideScreen() {
  const Vec2f& SIZE = env::getWindowSize();
  const Color& B = Color::black;
  const Color SCR_COL(B.r(), B.g(), B.b(), alpha_);

  drawFillBox(-SIZE.x() * 0.5f, -SIZE.y() * 0.5f,
              SIZE.x(), SIZE.y(),
              SCR_COL);
}
