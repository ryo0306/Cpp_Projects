
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
// 場面管理クラス
//

class cSceneManager {
  cTitle      title_;
  cStrategy   strategy_;
  cBattle     battle_;
  cResult     result_;
  cDataSelect select_;
  cPause      pause_;

  // 画面切り替え演出用のメンバ変数
  struct {
    fade mode_;     // 切り替え処理の状態
    short time_;    // 演出のタイムカウンタ
    float alpha_;   // 透明度
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
