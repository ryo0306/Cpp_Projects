
#include "Result.h"

using namespace frameworks::scene;


// ������������
Result::Result() :
SceneBase(SceneName::Result, SceneName::Title) {
}


// �X�V
void Result::Update() {
  // isFinish : ���݂̃V�[�����I�����邽�߂̕ϐ�
  // true �ɂȂ�Ɖ�ʂ��ς��܂�
  isFinish = Env().isPushKey(ENTER);
}


// �`��
void Result::Draw() {
  drawFillCircle(0, 0, 50, 50, 50, Color::yellow);
}
