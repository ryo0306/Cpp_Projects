
#pragma once
#include "draw_window.h"
#include "data_depot.h"
#include "../Scene/title.h"
#include "../Scene/strategy.h"
#include "../Scene/battle.h"
#include "../Scene/result.h"
#include "../Scene/data_select.h"
#include "../Scene/pause.h"

//
// ��ʊǗ��N���X
//

class cSceneManager {
  cTitle      title_;
  cStrategy   strategy_;
  cBattle     battle_;
  cResult     result_;
  cDataSelect select_;
  cPause      pause_;

  // ��ʐ؂�ւ����o�p�̃����o�ϐ�
  struct {
    fade mode_;     // �؂�ւ������̏��
    short time_;    // ���o�̃^�C���J�E���^
    float alpha_;   // �����x
  } anime;
  bool init_;

  scene& now_scene_;
  scene& next_scene_;
  bool& is_pause_;

  bool sceneShift();

  void blackOut();
  void fadeShift();
  bool fadeOut();
  bool fadeIn();

  void effectInit();

public:
  cSceneManager();

  void update();
  void init();
  void draw();

  bool exit();
};
