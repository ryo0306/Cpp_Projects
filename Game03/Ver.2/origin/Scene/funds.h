
#pragma once
#include "../System/environment.h"
#include "../System/draw_gadget.h"
#include "../System/data_depot.h"
#include "../Literal/literal_font.hpp"
#include "../Literal/literal_common.hpp"
#include "../Literal/literal_effect.h"
#include "../Literal/literal_strategy.hpp"


class cFunds {
  Font telop_;

  typedef literal_strategy::FundsStringData  str;
  std::array<std::string, str::All_Funds> command_;
  std::array<literal_strategy::sDescriptionText, str::All_Funds> description_;
  std::array<float, str::All_Funds> move_x_;

  struct {
    short state_;   // ClickState::MoveIn_1 -> MoveOut_1 ‚Ü‚Å‚ðŽg—p‚·‚é
    short blink_;
    float alpha_;
  } anime;

  // system
  short& draw_state_;
  short& mode_state_;

  // user
  u_int& money_;        // aseets.money_
  short& food_;         // assets.food_
  short& work_;         // cnt.work_
  short& cnt_fire_;     // weapon_cnt.fire_
  short& cnt_bomb_;     // weapon_cnt.bomb_
  short& cnt_trap_;     // weapon_cnt.trap_
  short& cnt_poison_;   // weapon_cnt.poison_

  unsigned short index_;
  Vec2f pos_;
  bool on_mouse_;
  bool is_sell_;
  short click_id_;

  bool stateChange();
  void moveMode();
  void blinkMode();
  void backMode();

  void disp_button(const short);
  void disp_telop(const short);

  void buttonPosTranslate(const short);
  void buttonPosInit();

public:
  cFunds();

  void update();
  void init();
  void draw();
};
