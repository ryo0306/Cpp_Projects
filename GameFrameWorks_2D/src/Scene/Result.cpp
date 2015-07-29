
#include "Result.h"

using namespace frameworks::scene;


// ‰Šú‰»‚ð‚·‚é
Result::Result() :
SceneBase(SceneName::Result, SceneName::Title) {
  Asset().Delete().All();

  const std::string TextureTable[] = {
    "res/png/result_newton1.png",
    "res/png/result_newton2.png",
    "res/png/result_medal.png",
    "res/png/result_0.png",
    "res/png/result_1.png",
    "res/png/result_2.png",
    "res/png/result_3.png",
    "res/png/result_4.png",
    "res/png/result_5.png",
    "res/png/result_6.png",
    "res/png/result_7.png",
    "res/png/result_8.png",
    "res/png/result_9.png",
  };

  for (const auto& data : TextureTable) {
    textureID.push_back(Asset().Append().Texture(data));
  }

  const std::string MediaTable[] = {
    "res/wav/result.wav",
    "res/wav/start.wav",
    "res/wav/select.wav",
  };

  for (const auto& data : MediaTable) {
    mediaID.push_back(Asset().Append().Media(data));
  }

  const float gain = 0.5f;
  for (const auto& id : mediaID) {
    Asset().Find().Media(id)->gain(gain);
  }

  auto bgm = Asset().Find().Media(mediaID[0]);
  bgm->looping(true);
  bgm->play();
}


// XV
void Result::Update() {
  if (Env().isPushKey(ENTER)) {
    isFinish = true;
    Asset().Find().Media(mediaID[0])->stop();
  }
}


// •`‰æ
void Result::Draw() {
  //drawFillCircle(0, 0, 50, 50, 50, Color::yellow);

  //test
  auto player = Asset().Find().Texture(textureID[0]);
  drawTextureBox(-200, -200, 200, 200, 0, 0, 128, 128, *player);
}
