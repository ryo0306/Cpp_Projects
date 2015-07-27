
#pragma once
#include "SceneBase.hpp"


namespace frameworks {
namespace scene {

class MainGame : public SceneBase {
public:
  MainGame();

  void Update() override;
  void Draw() override;

private:
  // 必要な変数、関数をここに作る
};

}  // end scene
}  // end frameworks
