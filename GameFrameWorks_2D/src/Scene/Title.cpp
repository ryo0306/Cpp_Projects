
#include "Title.h"

using namespace frameworks::scene;


// 初期化をする
Title::Title() :
SceneBase(SceneName::Title, SceneName::Main),
select(0) {
  Asset().Delete().All();

  const std::string TextureTable[] = {
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

    "res/png/title_back.png",     //[10]
    "res/png/title_logo.png",     //[11]
    "res/png/stage1.png",         //[12]
    "res/png/stage2.png",         //[13]
    "res/png/stage3.png",         //[14]
    "res/png/title_cursor.png",   //[15]

    "res/png/ochirukun_red.png",
    "res/png/ochirukun_blue.png",
    "res/png/ochirukun_green.png",
    "res/png/ochirukun_yellow.png",
  };

  for (const auto& data : TextureTable) {
    textureID.push_back(Asset().Append().Texture(data));
  }

  icons.push_back(textureID[12]);
  icons.push_back(textureID[13]);
  icons.push_back(textureID[14]);

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
  if (Env().isPushKey(SPACE)) {
    isFinish = true;
    Asset().Find().Media(mediaID[0])->stop();
    GameData::Get().SetStageID(StageID(select));
  }

  if (Env().isPushKey(KEY_L) && select > 0) { --select; }
  if (Env().isPushKey(KEY_R) && select < 2) { ++select; }
}


// 描画
void Title::Draw() {
  // 背景
  const auto back = Asset().Find().Texture(textureID[10]);
  const Vec2f backSize(1600, 900);
  drawTextureBox(-backSize.x() / 2, -backSize.y() / 2,
                 backSize.x(), backSize.y(),
                 0, 0, backSize.x(), backSize.y(),
                 *back, Color::white);

  // パーティクル

  // ロゴ
  const auto logo = Asset().Find().Texture(textureID[11]);
  const Vec2f logoSize(2048, 512);
  drawTextureBox(0, 0, logoSize.x(), logoSize.y(),
                 0, 0, logoSize.x(), logoSize.y(),
                 *logo, Color::white,
                 0, Vec2f::Ones() * 0.85f,
                 Vec2f(logoSize.x() / 2, 0));

  // アイコン
  const Vec2f scale = Vec2f::Ones() * 1.2f;
  const Vec2f iconSize = Vec2f::Ones() * 256.0f;
  int count = 0;

  // カーソル
  const auto cursor = Asset().Find().Texture(textureID[15]);
  const Vec2f cursorSize(100, 160);

  for (auto& icon : icons) {
    const auto texture = Asset().Find().Texture(icon);
    const bool selected = (count == select);
    const float posX = (iconSize.x() + 180) * (count - 1);
    const float posY = -HEIGHT / 2 + 128;

    drawTextureBox(posX, posY, iconSize.x(), iconSize.y(),
                   0, 0, 480, 480,
                   *texture, Color::white, 0,
                   selected ? scale : Vec2f::Ones(),
                   iconSize * 0.5f);

    if (selected) {
      drawTextureBox(posX, posY + 200, cursorSize.x(), cursorSize.y(),
                     0, 0, cursorSize.x(), cursorSize.y(),
                     *cursor, Color::white, 0,
                     Vec2f::Ones(), cursorSize * 0.5f);
    }

    ++count;
  }
}
