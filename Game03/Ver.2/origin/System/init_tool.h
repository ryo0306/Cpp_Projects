
#pragma once
#include "../Literal/literal_scene.hpp"


namespace init {

  // TIPS: �����̃V�[�h�l��������
  void randomSeed();

  // TIPS: �Q�[���J�n���̃V�[����������
  // DEBUG: �����ɊJ�n�������V�[�����w��
  // * scene::Title, Strategy, Battle, Result ... etc
  void defaultScene(const scene& start = scene::Title);

  // TIPS: ���\�[�X�̏�����
  void resourceData();
}
