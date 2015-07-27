
#include "MainGame.h"

using namespace frameworks::scene;


// ������������
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result) {
}


// �X�V
void MainGame::Update() {
  // isFinish : ���݂̃V�[�����I�����邽�߂̕ϐ�
  // true �ɂȂ�Ɖ�ʂ��ς��܂�
  isFinish = Env().isPushKey(ENTER);
}


// �`��
void MainGame::Draw() {
  drawFillCircle(0, 0, 50, 50, 50, Color::red);
}
