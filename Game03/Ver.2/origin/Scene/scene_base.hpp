
#pragma once

// TEST

class cSceneBase {
public:
  cSceneBase() = default;

  virtual void update() = 0;
  virtual void init() = 0;
  virtual void draw() = 0;
};
