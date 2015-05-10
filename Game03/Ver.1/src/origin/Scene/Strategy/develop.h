
#pragma once
#include "../../System/draw_window.h"
#include "../../System/data_depot.h"
#include "../../Object/collision.h"

//
// çÏêÌ
//

class cDevelop {
  Font telop_;

  short i;
  Vec2f pos_;
  const Vec2f size_;
  bool on_mouse_;
  bool is_develop_;

  enum {
    Fire,
    Bomb,
    Trap,
    Poison,
    Back,

    All_Text,
  };
  std::string text[All_Text];

  struct {
    short time_;
    short blink_;
    float alpha_;
  } anime;

  mode& mode_;
  u_short& poli_state_;
  bool& pause_;

  short draw_state_;
  short id_;

  float x[All_Text];

  u_int& money_;
  u_short& fire_;
  u_short& cannon_;
  u_short& trap_;
  u_short& poison_;

  bool stateChange();
  void moveMode();
  void blinkMode();
  void backMode();
  void buttonSelect();

  void paymentMoney();
  void is_fire();
  void is_bomb();
  void is_trap();
  void is_poison();

  void isAbleToDevelop(const short&);
  void buttonPosTranslate(const short&);
  void buttonPosInit();

  void disp_telop(const short&);

public:
  cDevelop();

  void update();
  void init();
  void draw();
};
