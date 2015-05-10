
#pragma once
#include "draw_window.h"
#include "data_depot.h"
#include "slot.h"
#include "scene_manager.h"

//
// �Q�[���Ǘ��N���X
//

class cGamePlayer {
  cSceneManager scene_;

  // TIPS: �N�����A�K�v�ȏ��������s��
  void systemInit() {

    // TIPS: �����̃V�[�h�l��ݒ�
    data::random.setSeed(u_int(time(nullptr)));

    // TIPS: BGM�ASE�f�[�^��W�J
    data::music.dataInit();

    // TIPS: �Q�[���̃N���A�t���O�����Z�b�g
    data::system.clear_ = false;

    // TIPS: �`��J�n����V�[�����w��
    data::sceneStart(scene::Title);
    scene_.init();
  }

public:
  cGamePlayer() = default;

  // �Q�[�����s
  // CAUTION: ���̊֐��ȊO�� while ���[�v�����Ȃ�
  void run() {
    systemInit();

    while (win::app->isOpen() && !scene_.exit()) {
      scene_.update();
      scene_.init();
      scene_.draw();
    }
  }
};
