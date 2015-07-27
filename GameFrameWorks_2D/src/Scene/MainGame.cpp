
#include "MainGame.h"

using namespace frameworks::scene;


// 初期化をする
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result) {
}


// 更新
void MainGame::Update() {
  // isFinish : 現在のシーンを終了するための変数
  // true になると画面が変わります
  isFinish = Env().isPushKey(ENTER);
}


// 描画
void MainGame::Draw() {
  drawFillCircle(0, 0, 50, 50, 50, Color::red);
}
