
#pragma once
#include "../System/draw_window.h"
#include "../System/data_depot.h"
#include "../Object/collision.h"
#include "../Resource/map_data.h"

//
// êÌì¨âÊñ 
//

class cBattle {
  Font telop_;
  cMapData map_data_;

  short i;
  std::list<cCharacter> enemy;    // êÌì¨Ç…éQâ¡íÜÇÃìGÇÃêî
  std::queue<cCharacter> order;   // éQâ¡Ç∑ÇÈìGÇÃèáî‘
  std::list<cCharacter>::iterator it, begin, end;
  std::list<cCharacter>::iterator target, begin_, end_;

  float camera_x_;

  struct {
    u_short time_;
    u_short blink_;
    float alpha_;
  } anime;

  struct {
    short player_;
    short enemy_;
    short count_;
  } dead;

  scene& next_scene_;
  phase& phase_;
  u_short& stage_id_;

  bool& fire_;
  bool& cannon_;
  bool& trap_;
  bool& poison_;

  u_short& cnt_fire_;
  u_short& cnt_cannon_;
  u_short& cnt_trap_;
  u_short& cnt_poison_;

  Vec2f pos_;
  Vec2f cut_;

  void unitSummon();
  void unitPosTranslate();
  void enemyStandby();

  void countInit();
  void deadCount();
  void battleFinish();
  void win();
  void unitUpdate();

  void timeCount();
  void unit_animePlay();
  void unit_action();
  void physicalAttack();
  void magicalAttack();
  void cure();

  void fire_command();
  void cannon_command();
  void trap_command();
  void poison_command();
  void command_act();

  void getEnemyInfo();
  void getPlayerInfo();

  void dispUnitInfo();
  void dispPlayerInfo();
  void dispEnemyInfo();

public:
  cBattle();

  void update();
  void init();
  void draw();
};
