
#pragma once
#include "../System/draw_window.h"
#include "../System/data_depot.h"
#include "../System/slot.h"
#include "../Object/collision.h"

//
// データのセーブ、ロード
//

class cDataSelect {
  Texture bg_;
  Texture icon_;
  Font telop_;
  Font info_;

  std::list<cCharacter>::iterator it_, begin_, end_;
  cUser temp_;

  short i, j;
  Vec2f pos_;
  const Vec2f slot_size_;
  bool on_mouse_;

  enum {
    Slot1,
    Slot2,
    Slot3,
    Back,

    Save,
    Load,

    All_Text,
  };
  std::string text[All_Text];
  short text_id_;

  std::string dif_text[3];

  enum {
    Select,
    Exe,

    SaveDone,
    LoadDone,
    Exit,
  };
  short click_state_;

  std::string pass_;

  scene& now_scene_;
  scene& next_scene_;
  scene& last_scene_;

  void slotSelect();

  void disp_back();
  void disp_window();
  void disp_slot();

  void disp_empty();
  void disp_info();
  void info_dif();
  void info_money();
  void info_strategy();
  void info_unit();

  void buttonPosTranslate(const short&);
  bool slotState(const short&);

public:
  cDataSelect();

  void update();
  void init();
  void draw();
};
