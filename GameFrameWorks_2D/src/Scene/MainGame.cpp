
#include "MainGame.h"
#include <iostream>
using namespace frameworks::scene;
using namespace frameworks::utility;


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
	/*
	if (IsHitRectToRect(player.GetTransform(),
		gimmick.GetTransform())){
		std::cout << "1" << std::endl;
		std::cout << player.GetTransform().pos << std::endl;
		std::cout << gimmick.GetTransform().pos << std::endl;
	}
	*/

	


	isFinish = IsHitRectToRect(player.GetTransform(),
								gimmick.GetTransform());
}


// �`��
void MainGame::Draw() {
	gimmick.Draw();
	player.Draw();	
}
