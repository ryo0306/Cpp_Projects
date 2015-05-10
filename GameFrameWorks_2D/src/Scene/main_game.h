
#pragma once
#include "scene.hpp"


class cMainGame : public cScene {
public:

  cMainGame();

  void update();
  void draw();

  SceneState getScene()  const;
  std::shared_ptr<cScene> nextScene() const;
};
