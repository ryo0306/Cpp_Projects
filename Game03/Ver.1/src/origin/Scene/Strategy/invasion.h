
#pragma once
#include "../../System/draw_window.h"
#include "../../System/data_depot.h"
#include "../../Object/collision.h"

//
// êNçU
//

class cInvasion {
  Texture icon_;
  Font telop_;

  short i;
  Vec2f pos_;
  const Vec2f size_;
  const Vec2f icon_size_;
  bool on_mouse_;

  mode& mode_;
  u_short& poli_state_;
  u_short& stage_id_;
  bool& pause_;
  bool flag_;

  enum { Back = -2 };
  short draw_state_;
  short id_;

  float x;

  std::string map[3];
  std::string effect[effect::All_Effect];

  struct {
    short time_;
    short blink_;
    float alpha_;
  } anime;

  bool stateChange();
  void moveMode();
  void blinkMode();
  void backMode();

  void buttonSelect();
  void back_command();

  void disp_mapInfo(const short&);
  void disp_command();

  void buttonPosTranslate(const short&);
  void buttonPosInit();

public:
  cInvasion();

  void update();
  void init();
  void draw();
};
