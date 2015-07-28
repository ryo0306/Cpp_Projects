
#include "MainGame.h"
#include <iostream>
using namespace frameworks::scene;
using namespace frameworks::utility;


// ������������
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result),
player(Vec2f(0, 0)),
gimmick(Bottom, Vec2f(-200,-300)) {
}


// �X�V
void MainGame::Update() {

  const auto& playerTransform = player.GetTransform();
  const auto& stageCollisions = stage.GetTransforms();

  for (auto& block : stageCollisions) {
    Transform temp;
    temp.pos = block.pos;
    temp.scale = block.size;
    if (IsHitRectToRect(playerTransform, temp)){
      player.Translate(temp);
      player.GravityReset();
    }
  }
 
  player.Update();
  
	isFinish = IsHitRectToRect(player.GetTransform(),
								gimmick.GetTransform());
}


// �`��
void MainGame::Draw() {
  stage.Draw();
	gimmick.Draw();
	player.Draw();	
}
