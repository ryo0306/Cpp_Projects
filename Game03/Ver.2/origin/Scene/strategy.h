
#pragma once
#include "../System/environment.h"
#include "../System/draw_gadget.h"
#include "../System/data_depot.h"
#include "../Scene/funds.h"
#include "../Scene/develop.h"
#include "../Scene/armaments.h"
#include "../Scene/invasion.h"
#include "../Literal/literal_font.hpp"
#include "../Literal/literal_common.hpp"
#include "../Literal/literal_effect.h"
#include "../Literal/literal_strategy.hpp"


class cStrategy {
  cFunds     funds_;
  cDevelop   develop_;
  cArmaments armaments_;
  cInvasion  invasion_;

  Font telop_;
  Font info_;

  std::array<Texture, spec::All_Stage> back_chip_;
  Texture back_;
  Texture back_sea_;
  Texture back_fog_;
  Texture back_cld_;
  Texture map_icon_;
  Texture info_icon_;

  typedef literal_strategy::StrategyStringData  str;
  std::array<std::string, str::All_Command> command_;
  std::array<std::string, str::All_Select>  saveload_;
  std::array<std::string, str::All_Command> descript_command_;
  std::array<std::string, str::All_Select> descript_saveload_;
  literal_strategy::sDescriptionText description_;
  std::array<float, str::All_Command> move_x_;

  struct {
    short blink_;
    float alpha_;
    float scroll_;
  } anime;

  // system
  bool& pause_;
  scene& now_scene_;
  scene& next_scene_;
  scene& last_scene_;
  short& draw_state_;
  short& mode_state_;

  std::list<cCharacter>::iterator it_, begin_, end_;
  unsigned short index_;
  Vec2f pos_;
  bool on_mouse_;
  bool territory_;
  short click_id_;
  short descript_time_;

  bool stateChange();
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

  void disp_window();
  void disp_countInfo();
  void disp_unitInfo();

  void disp_command();
  void disp_telop(const short);
  void disp_description();

  void buttonPosTranslate(const short);
  void buttonPosInit();
  bool drawStateCheck();

public:
  cStrategy();

  void update();
  void init();
  void draw();
};
