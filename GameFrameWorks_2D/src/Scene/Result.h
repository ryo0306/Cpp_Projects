
#pragma once
#include "SceneBase.hpp"


namespace frameworks {
namespace scene {

class Result : public SceneBase {
public:
  Result();

  void Update() override;
  void Draw() override;

private:
  // 必要な変数、関数をここに作る
};

}  // end scene
}  // end frameworks
