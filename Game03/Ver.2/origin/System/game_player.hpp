
#pragma once
#include "scene_manager.h"
#include "init_tool.h"


class cGamePlayer {
  cSceneManager scene_;

  // TIPS: �N�����A�V�X�e���S�̂̏��������s��
  void systemInit() {
    init::randomSeed();
    init::defaultScene();
    init::resourceData();
  }

public:
  cGamePlayer() = default;

  // CAUTION: ���̊֐��ȊO�Ƀ��[�v�����Ȃ�����
  void run() {
    systemInit();

    while (scene_.isContinue()) {
      scene_.update();
      scene_.init();
      scene_.draw();
    }
  }
};
