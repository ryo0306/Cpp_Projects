
#pragma once
#include "../System/draw_window.h"
#include "../System/data_depot.h"
#include "../Object/collision.h"

//
// リザルト画面
//

class cResult {
  Texture bg_;
  Font telop_;
  Font info_;

  std::list<cCharacter>::iterator it, begin, end;

  enum {
    Result,
    Win,
    Lose,
    Clear,

    Telop1,
    Telop2,
    Telop3,
    All_Text,
  };
  std::string text[All_Text];

  Vec2f pos_;
  const Vec2f size_;
  bool on_mouse_;
  bool result_;

  struct {
    u_short time_;
    u_short blink_;
    float alpha_;
  } anime;

  enum AnimeState {
    FadeIn,
    Wait,
    Blink,
    FadeOut,
  };
  short draw_state_;

  scene& next_scene_;
  phase& phase_;
  u_short& stage_id_;
  bool& ending_;

  short rank_;
  u_int& money_;
  u_short& battle_;
  u_short& command_;
  short& dif_;
  short command_count_;
  u_int guara_base_;
  u_int guarantee_;

  // 各陣営のレベル平均値
  struct {
    float player_;
    float enemy_;
  } lv_average;

  bool stateChange();
  void fadeInMode();
  void blinkMode();
  void fadeOutMode();

  void musicPlay();
  void levelAverage();

  void disp_back();
  void disp_logo();
  void disp_ending();
  void disp_score();
  void disp_loser(const int&, const bool&);
  void disp_training(const int&);

  void disp_clear();
  void disp_button();

  std::string getRank();
  short getRankColor();

public:
  cResult();

  void update();
  void init();
  void draw();
};
