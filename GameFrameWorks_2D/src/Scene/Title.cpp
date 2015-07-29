
#include "Title.h"

using namespace frameworks::scene;


// 初期化をする
Title::Title() :
SceneBase(SceneName::Title, SceneName::Main),
select(0) {
  Asset().Delete().All();

  const std::string TextureTable[] = {
    "res/png/title_back.png",
    "res/png/title_logo.png",
    "res/png/stage1.png",
    "res/png/stage2.png",
    "res/png/stage3.png",
    "res/png/title_cursor.png",
    "res/png/title_0.png",
    "res/png/title_1.png",
    "res/png/title_2.png",
    "res/png/title_3.png",
    "res/png/title_4.png",
    "res/png/title_5.png",
    "res/png/title_6.png",
    "res/png/title_7.png",
    "res/png/title_8.png",
    "res/png/title_9.png",
    "res/png/ochirukun_red.png",
    "res/png/ochirukun_blue.png",
    "res/png/ochirukun_green.png",
    "res/png/ochirukun_yellow.png",
  };

  for (const auto& data : TextureTable) {
    textureID.push_back(Asset().Append().Texture(data));
  }

  //icons.push_back(textureID[]);

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


// 更新
void Title::Update() {
  if (Env().isPushKey(ENTER)) {
    isFinish = true;
    Asset().Find().Media(mediaID[0])->stop();
  }
}


// 描画
void Title::Draw() {
  // 背景
  const auto back = Asset().Find().Texture(textureID[0]);
  const Vec2f backSize(1600, 900);
  drawTextureBox(-backSize.x() / 2, -backSize.y() / 2,
                 backSize.x(), backSize.y(),
                 0, 0, backSize.x(), backSize.y(),
                 *back, Color::white);

  // パーティクル

  // ロゴ
  const auto logo = Asset().Find().Texture(textureID[1]);
  const Vec2f logoSize(2048, 512);
  drawTextureBox(0, 0, logoSize.x(), logoSize.y(),
                 0, 0, logoSize.x(), logoSize.y(),
                 *logo, Color::white,
                 0, Vec2f::Ones() * 0.85f,
                 Vec2f(logoSize.x() / 2, 0));

  // アイコン
  const auto icon1 = Asset().Find().Texture(textureID[2]);
  const auto icon2 = Asset().Find().Texture(textureID[3]);
  const auto icon3 = Asset().Find().Texture(textureID[4]);
  const auto cursor = Asset().Find().Texture(textureID[5]);
  const Vec2f scale = Vec2f::Ones() * 1.2f;
}
