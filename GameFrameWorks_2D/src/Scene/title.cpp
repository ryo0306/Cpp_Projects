
#include "Title.h"

using namespace frameworks::scene;


// 初期化をする
Title::Title() :
SceneBase(SceneName::Title, SceneName::Main) {
  textureID = Asset().Append().Texture("res/png/player.png");
  mediaID = Asset().Append().Media("res/wav/se_test.wav");
}


// 更新
void Title::Update() {
  // isFinish : 現在のシーンを終了するための変数
  // true になると画面が変わります
  isFinish = Env().isPushKey(ENTER);

  //test
  if (Env().isPushKey('A')) {
    Asset().Find().Media(mediaID)->play();
  }
}


// 描画
void Title::Draw() {
  //drawFillCircle(0, 0, 50, 50, 50, Color::white);

  //test
  auto player = Asset().Find().Texture(textureID);
  drawTextureBox(0, 0, 100, 100, 0, 0, 128, 128, *player);
}
