
#include "MainGame.h"

using namespace frameworks::scene;


// ������������
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result),
player(Vec2f(0, 0)),
gimmick(Bottom, Vec2f(-200,-300)) {
}


// �X�V
void MainGame::Update() {
  // isFinish : ���݂̃V�[�����I�����邽�߂̕ϐ�
  // true �ɂȂ�Ɖ�ʂ��ς��܂�
	player.Update();

  //isFinish = Env().isPushKey(ENTER);
}


// �`��
void MainGame::Draw() {
	gimmick.Draw();
	player.Draw();	
}
