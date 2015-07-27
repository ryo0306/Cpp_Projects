
#pragma once
#include "Common.hpp"
#include "Scene/SceneManager.h"


namespace frameworks {

class GamePlayer {
public:
  GamePlayer(const scene::SceneName scene) : manager(scene) {}

  // ゲームの更新
  void Update() {
    manager.Update();
  }

  // ゲームの描画
  void Draw() {
    Env().begin();   // 描画準備
    manager.Draw();
    Env().end();     // 画面更新
  }

  // ゲーム起動中かどうか
  const bool IsRunning() {
    return Env().isOpen() && !Env().isPushKey(ESCAPE);
  }

private:
  SceneManager manager;
};

}  // end frameworks
