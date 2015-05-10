
#pragma once
#include "../System/environment.h"
#include "../System/draw_gadget.h"
#include "../System/data_depot.h"
#include "../Literal/literal_font.hpp"
#include "../Literal/literal_common.hpp"
#include "../Literal/literal_effect.h"
#include "../Literal/literal_map.hpp"
#include "../Literal/literal_title.hpp"


class cTitle {
  Font telop_;
  Texture back_;
  Texture back_sea_;
  Texture back_fog_;
  Texture logo_;

  typedef literal_title::StringData  str;
  std::string text[str::All_Text];
  short click_button_;

  struct {
    short state_;
    bool is_play_;

    short time_;
    float angle_;

    short logo_;
    short blink_;
    float alpha_;
    float scroll_;
  } anime;

  const Vec2f button_size_;

  bool draw_switch_;
  short draw_telop_;
  unsigned short index_;
  short begin_, end_;

  bool on_mouse_;
  bool take_over_;
  Vec2f pos_;

  bool& game_clear_;
  scene& now_scene_;
  scene& next_scene_;
  scene& last_scene_;

  bool stateChange();
  void clickAction();
  void blinkMode();
  void fadeInMode();
  void fadeOutMode();

  void nextSceneShift();
  void newGame();
  void reStart();

  void userDataInit();
  void mapDataInit();

  void effectPlay();
  void disp_back();
  void disp_telop();
  void disp_button();

  void buttonIndexTranslate();
  void buttonPosTranslate(const short);
  void buttonStringTranslate(const short);

public:
  cTitle();

  void update();
  void init();
  void draw();
};
