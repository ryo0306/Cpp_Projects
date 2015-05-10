
#pragma once
#include <memory>

class cSceneBase;
typedef std::shared_ptr<cSceneBase>  pScene;


class cSceneBase {
protected:
  bool is_finish_;

public:
  cSceneBase() : is_finish_(false) {}

  virtual void update() = 0;
  virtual void draw()   = 0;

  virtual pScene nextScene() { return nullptr; }
  bool isFinish() { return is_finish_; }
};
