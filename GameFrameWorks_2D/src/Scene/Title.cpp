
#include "Title.h"

using namespace frameworks::scene;


// ‰Šú‰»‚ð‚·‚é
Title::Title() :
SceneBase(SceneName::Title, SceneName::Main) {
  Asset().Delete().All();

  const std::string TextureTable[] = {
    "res/png/player.png",
  };

  for (const auto& data : TextureTable) {
    textureID.push_back(Asset().Append().Texture(data));
  }

  const std::string MediaTable[] = {
    "res/wav/op.wav",
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
void Title::Update() {
  if (Env().isPushKey(ENTER)) {
    isFinish = true;
    Asset().Find().Media(mediaID[0])->stop();
  }
}


// •`‰æ
void Title::Draw() {
  //drawFillCircle(0, 0, 50, 50, 50, Color::white);

  //test
  auto player = Asset().Find().Texture(textureID[0]);
  drawTextureBox(-200, -200, 200, 200, 0, 0, 128, 128, *player);
}
