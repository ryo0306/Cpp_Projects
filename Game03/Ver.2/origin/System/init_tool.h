
#pragma once
#include "../Literal/literal_scene.hpp"


namespace init {

  // TIPS: 乱数のシード値を初期化
  void randomSeed();

  // TIPS: ゲーム開始時のシーンを初期化
  // DEBUG: 引数に開始したいシーンを指定
  // * scene::Title, Strategy, Battle, Result ... etc
  void defaultScene(const scene& start = scene::Title);

  // TIPS: リソースの初期化
  void resourceData();
}
