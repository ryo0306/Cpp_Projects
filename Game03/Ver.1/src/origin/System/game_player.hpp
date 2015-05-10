
#pragma once
#include "draw_window.h"
#include "data_depot.h"
#include "slot.h"
#include "scene_manager.h"

//
// ゲーム管理クラス
//

class cGamePlayer {
  cSceneManager scene_;

  // TIPS: 起動時、必要な初期化を行う
  void systemInit() {

    // TIPS: 乱数のシード値を設定
    data::random.setSeed(u_int(time(nullptr)));

    // TIPS: BGM、SEデータを展開
    data::music.dataInit();

    // TIPS: ゲームのクリアフラグをリセット
    data::system.clear_ = false;

    // TIPS: 描画開始するシーンを指定
    data::sceneStart(scene::Title);
    scene_.init();
  }

public:
  cGamePlayer() = default;

  // ゲーム実行
  // CAUTION: この関数以外に while ループを作らない
  void run() {
    systemInit();

    while (win::app->isOpen() && !scene_.exit()) {
      scene_.update();
      scene_.init();
      scene_.draw();
    }
  }
};
