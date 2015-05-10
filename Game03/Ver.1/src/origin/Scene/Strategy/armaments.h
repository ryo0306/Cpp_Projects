
#pragma once
#include "../../System/draw_window.h"
#include "../../System/data_depot.h"
#include "../../Object/collision.h"

//
// ŒR”õ
//

class cArmaments {
  Font telop_;

  short i;
  Vec2f pos_;
  const Vec2f size_;
  const Vec2f info_size_;
  bool on_mouse_;
  bool is_click_;

  enum {
    Growth,
    Change,
    Gender,
    Shift,
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

  enum CommandState {
    NoUse = Back,   // Žg—p‚µ‚È‚¢

    s_Growth,
    s_Change,
    s_Gender,
    s_Shift,

    s_Back = 20,
    s_Blink,
    s_Out,
  };

  enum State {
    Before,
    After,

    All_State,
  };
  short draw_state_;
  short id_;

  float button_x[All_Text];

  u_int& money_;
  std::list<cCharacter>::iterator it_, begin_, end_;
  short num_;

  bool stateChange();
  void moveMode();
  void blinkMode();
  void backMode();

  void buttonSelect();
  void armamentSelect();
  void growthMode();
  void changeMode();
  void genderMode();
  void shiftMode();
  void click();

  void disp_command();
  void disp_unit();
  void disp_growth_window();
  void disp_growth();
  void disp_change();
  void disp_gender();
  void disp_shift();

  void disp_telop(const short&);

  void buttonPosTranslate(const short&);
  void buttonPosInit();

  void unitPosTranslate(const short&);

public:
  cArmaments();

  void update();
  void init();
  void draw();
};
