
#pragma once
#include "../System/draw_window.h"
#include "../System/data_depot.h"
#include "../Object/collision.h"

//
// É|Å[ÉYâÊñ 
//

class cPause {
  Texture icon_;
  Font telop_;

  short i;
  short button_;
  Vec2f pos_;
  const Vec2f size_;
  const Vec2f icon_size_;

  scene& now_scene_;
  scene& next_scene_;
  bool& pause_;
  bool on_mouse_;

  enum {
    Pause,
    Button_1,
    Button_2,
    Attention,
    OK,
    NG,
  };
  std::string text[6];

  enum {
    c_Fire,
    c_Cannon,
    c_Trap,
    c_Poison,
    c_BackGame,
    c_BackTitle,

    All_Icon,
  };
  std::string icon_text_;
  bool is_click_;
  float icon_scale_;

  bool& fire_;
  bool& cannon_;
  bool& trap_;
  bool& poison_;

  u_short& cnt_command_;
  u_short& cnt_fire_;
  u_short& cnt_cannon_;
  u_short& cnt_trap_;
  u_short& cnt_poison_;

  enum {
    Select,
    Clicked,
  };
  short state_;

  void pauseMenu();
  void commandShift(const short&);
  void backToGame();
  void backToTitle();

  void battleMenu();
  void commandAction(const short&);

  void disp_back();
  void scenePolitics();
  void sceneBattle();
  void disp_window();
  void disp_telop(const short&);

  void buttonPosTranslate(const short&);
  void iconPosTranslate(const short&);

public:
  cPause();

  bool update();
  void draw();
};
