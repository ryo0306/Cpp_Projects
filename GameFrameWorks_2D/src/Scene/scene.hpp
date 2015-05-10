
#pragma once
#include <memory>
#include "../common.hpp"


class cSceneManager;


class cScene {
protected:

  bool m_is_finish;

  cScene() : m_is_finish(false) {};

public:

  virtual void update() = 0;
  virtual void draw()   = 0;

  virtual SceneState getScene() const = 0;
  virtual std::shared_ptr<cScene> nextScene() const = 0;

  bool sceneFinish() const { return m_is_finish; }
};
