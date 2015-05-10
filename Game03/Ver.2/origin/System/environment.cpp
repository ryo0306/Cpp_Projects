
#include "environment.h"


Vec2f env::mouse_;
std::unique_ptr<AppEnv> env::app_;


void env::execApplication() {
  if (app_ != nullptr) { return; }
  app_ = std::make_unique<AppEnv>(WIDTH, HEIGHT, false, true);
}


void env::setupDraw() {
  app_->setupDraw();
}


void env::update() {
  app_->update();
}


void env::mousePosUpdate() {
  mouse_ = app_->mousePosition();
}


bool env::rectOnMouse(const Vec2f& pos0,
                      const Vec2f& size0) {
  if (mouse_.x() > pos0.x() &&
      mouse_.x() < pos0.x() + size0.x() &&
      mouse_.y() > pos0.y() &&
      mouse_.y() < pos0.y() + size0.y()) {
    return true;
  }

  return false;
}


bool env::leftClick() {
  return app_->isPushButton(Mouse::LEFT);
}


bool env::rightClick() {
  return app_->isPushButton(Mouse::RIGHT);
}


bool env::isOpen() {
  return app_->isOpen();
}
