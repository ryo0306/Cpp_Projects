
#include "Result.h"

using namespace frameworks::scene;


// 初期化をする
Result::Result() :
SceneBase(SceneName::Result, SceneName::Title) {
}


// 更新
void Result::Update() {
  // isFinish : 現在のシーンを終了するための変数
  // true になると画面が変わります
  isFinish = Env().isPushKey(ENTER);
}


// 描画
void Result::Draw() {
  drawFillCircle(0, 0, 50, 50, 50, Color::yellow);
}
