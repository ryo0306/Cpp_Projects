
#include "Title.h"

using namespace frameworks::scene;


// ������������
Title::Title() :
SceneBase(SceneName::Title, SceneName::Main) {
  textureID = Asset().Append().Texture("res/png/player.png");
  mediaID = Asset().Append().Media("res/wav/se_test.wav");
}


// �X�V
void Title::Update() {
  // isFinish : ���݂̃V�[�����I�����邽�߂̕ϐ�
  // true �ɂȂ�Ɖ�ʂ��ς��܂�
  isFinish = Env().isPushKey(ENTER);

  //test
  if (Env().isPushKey('A')) {
    Asset().Find().Media(mediaID)->play();
  }
}


// �`��
void Title::Draw() {
  //drawFillCircle(0, 0, 50, 50, 50, Color::white);

  //test
  auto player = Asset().Find().Texture(textureID);
  drawTextureBox(0, 0, 100, 100, 0, 0, 128, 128, *player);
}
