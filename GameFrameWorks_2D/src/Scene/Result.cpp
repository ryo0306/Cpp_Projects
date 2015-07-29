
#include "Result.h"

using namespace frameworks::scene;


// 初期化をする
Result::Result() :
SceneBase(SceneName::Result, SceneName::Title) {
  Asset().Delete().All();

  const std::string TextureTable[] = {
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
    "res/png/result_newton1.png",
    "res/png/result_newton2.png",
    "res/png/result_medal.png",
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


// 更新
void Result::Update() {
  if (Env().isPushKey(SPACE)) {
    isFinish = true;
    Asset().Find().Media(mediaID[0])->stop();
  }
}


// 描画
void Result::Draw() {


  //クリア時ギミック使用回数
  u_int count = 3;

  bool get_medal = false;
  
  //ステージごとにメダル獲得かいっ数を調べる
  switch (GameData::Get().GetStageID())
  {
  case StageID::Stage1:
      get_medal = count <= 4;
    break;
  case StageID::Stage2:
      get_medal = count <= 4;
    break;
  case StageID::Stage3:
    get_medal = count <= 9;
    break;
  default:
    break;
  }

  get_medal = true;

  //メダル有か無を判定
  if (get_medal){
    const auto result_medal = Asset().Find().Texture(textureID[10]);
    drawTextureBox(-800, -450, 1600, 900, 0, 0, 1600, 900, *result_medal);
    const auto medal = Asset().Find().Texture(textureID[12]);
    drawTextureBox(-170, -325, 512, 512, 0, 0, 512, 512, *medal);
  }
  else {
    const auto result_no_medal = Asset().Find().Texture(textureID[11]);
    drawTextureBox(-800, -450, 1600, 900, 0, 0, 1600, 900, *result_no_medal);
  }

  //数字
    const auto num_1 = Asset().Find().Texture(textureID[count % 10]);
    drawTextureBox(-580, -500, 480, 480, 0, 0, 512, 512, *num_1);
  

    if (count / 10){
      const auto num_2 = Asset().Find().Texture(textureID[count / 10]);
      drawTextureBox(-760, -500, 480, 480, 0, 0, 512, 512, *num_2);
    }


 

  
}
