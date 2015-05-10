
#pragma once
#include "Debug.h"
#include "Environment.h"
#include "SceneManager.h"
#include "../Resource/Sprite.h"
#include "../Resource/Media.h"
#include <memory>


class cGamePlayer {
  bool is_finish_;

  std::unique_ptr<cSceneManager> manager_;
  cDebug debug_;

  void debug_cmd() {
    is_finish_
      = env::get().isPushButton(GLFW_KEY_ESCAPE);

    // TODO: �f�o�b�O�p�N���X���쐬�A�R�}���h�ǉ�
  }

  void debug_prt() {
    // TODO: �f�o�b�O�p�̕`�揈����ǉ�
  }

public:
  cGamePlayer() : is_finish_(false) {
    sprt::get();
    md::get();

    std::shared_ptr<cSceneBase> start
      = std::make_shared<cTitle>();

    manager_
      = std::make_unique<cSceneManager>(start);
  }

  void update() {
#ifdef _DEBUG
    debug_cmd();
#endif
    manager_->update();
  }

  void draw() {
    env::get().begin();
    manager_->draw();
#ifdef _DEBUG
    debug_prt();
#endif
    env::get().end();
  }

  bool isRunning() const {
    return !is_finish_ && env::get().isOpen();
  }
};
