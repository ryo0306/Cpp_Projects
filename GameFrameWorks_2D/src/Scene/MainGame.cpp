
#include "MainGame.h"
#include <iostream>
using namespace frameworks::scene;
using namespace frameworks::utility;


// 初期化をする
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result),
player(Vec2f(0, 0)),
gimmick(Bottom, Vec2f(-200,-300)) {
}


// 更新
void MainGame::Update() {
  // isFinish : 現在のシーンを終了するための変数
  // true になると画面が変わります
	player.Update();
	/*
	if (IsHitRectToRect(player.GetTransform(),
		gimmick.GetTransform())){
		std::cout << "1" << std::endl;
		std::cout << player.GetTransform().pos << std::endl;
		std::cout << gimmick.GetTransform().pos << std::endl;
	}
	*/

	


	isFinish = IsHitRectToRect(player.GetTransform(),
								gimmick.GetTransform());
}


// 描画
void MainGame::Draw() {
	gimmick.Draw();
	player.Draw();	
}
