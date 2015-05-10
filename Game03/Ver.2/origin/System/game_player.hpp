
#pragma once
#include "scene_manager.h"
#include "init_tool.h"


class cGamePlayer {
  cSceneManager scene_;

  // TIPS: 起動時、システム全体の初期化を行う
  void systemInit() {
    init::randomSeed();
    init::defaultScene();
    init::resourceData();
  }

public:
  cGamePlayer() = default;

  // CAUTION: この関数以外にループを作らないこと
  void run() {
    systemInit();

    while (scene_.isContinue()) {
      scene_.update();
      scene_.init();
      scene_.draw();
    }
  }
};
