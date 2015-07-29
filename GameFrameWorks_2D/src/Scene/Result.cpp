
#include "Result.h"

using namespace frameworks::scene;


// ������������
Result::Result() :
SceneBase(SceneName::Result, SceneName::Title) {

  const std::string TextureTable[] = {
    "res/png/player.png",
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


// �X�V
void Result::Update() {
  // isFinish : ���݂̃V�[�����I�����邽�߂̕ϐ�
  // true �ɂȂ�Ɖ�ʂ��ς��܂�
  isFinish = Env().isPushKey(ENTER);
}


// �`��
void Result::Draw() {
  //drawFillCircle(0, 0, 50, 50, 50, Color::yellow);

  //test
  auto player = Asset().Find().Texture(textureID[0]);
  drawTextureBox(-200, -200, 200, 200, 0, 0, 128, 128, *player);
}
