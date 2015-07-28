
#include "MainGame.h"

using namespace frameworks::scene;


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

  //isFinish = Env().isPushKey(ENTER);
}


// 描画
void MainGame::Draw() {
	gimmick.Draw();
	player.Draw();	
}
