
#pragma once
#include "../System/draw_window.h"
#include "../System/data_depot.h"
#include "../Object/collision.h"
#include "Strategy/funds.h"
#include "Strategy/armaments.h"
#include "Strategy/develop.h"
#include "Strategy/invasion.h"

//
// 戦略画面
//

class cStrategy {
  cFunds     funds_;       // 資金繰り
  cArmaments armaments_;   // 軍備
  cDevelop   develop_;     // 開発
  cInvasion  invasion_;    // 侵攻：ステージセレクト

  Texture bg_;
  Texture smoke_;
  Texture sea_;
  Texture fog_;
  Texture icon_;
  Texture world_icon_;

  Font telop_;
  Font unit_;

  short i;
  Vec2f pos_;
  const Vec2f size_;
  bool on_mouse_;
  bool select_;
  bool unit_empty_;

  std::string text[mode::All_Mode];
  std::string icon_name[4];

  std::list<cCharacter>::iterator it_, begin_, end_;

  enum {
    Save,
    Load,
    Back,

    All_Command,
  };
  std::string choice[All_Command];

  struct {
    short time_;
    short blink_;
    float scroll_;
    float alpha_;
  } anime;

  mode& mode_;
  scene& now_scene_;
  scene& next_scene_;
  scene& last_scene_;
  u_short& draw_state_;
  bool& pause_;
  short id_;

  float x[mode::All_Mode - 1];
  bool symbol_flag_;

  bool stateShift();
  void moveMode();
  void blinkMode();
  void backMode();

  void commandSelect();
  void commandChange();
  void shiftCommandMode();
  void shiftSelectMode();

  void effectPlay();
  void disp_back();
  void disp_mode();

  void disp_info();
  void info_money();
  void info_strategy();
  void info_unit();
  
  void disp_button();
  void disp_telop(const short&);
  void commandMode(const short&);
  void selectMode(const short&);
  void disp_symbol();

  void buttonPosTranslate(const short&);
  void buttonPosInit();

public:
  cStrategy();

  void update();
  void init();
  void draw();
};
