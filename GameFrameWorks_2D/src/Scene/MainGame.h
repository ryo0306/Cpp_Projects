
#pragma once
#include "SceneBase.hpp"
#include "../Objects/Stage.h"
#include "../Objects/Player.h"
#include "../Objects/Gimmick.h"

namespace frameworks {
namespace scene {

class MainGame : public SceneBase {
public:
  MainGame();

  void Update() override;
  void Draw() override;

private:
  // 必要な変数、関数をここに作る
  object::Stage stage;
	object::Player player;
	object::StageGimmick gimmick;
};

}  // end scene
}  // end frameworks
