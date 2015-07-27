
#pragma once
#include "../Common.hpp"
#include <memory>


class SceneBase {
public:
  SceneBase() {}
  virtual ~SceneBase() = default;

  virtual void update() {}
  virtual void draw() {}
};

typedef std::shared_ptr<SceneBase>  pScene;
