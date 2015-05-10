
#pragma once
#include "../System/draw_window.h"
#include <fstream>

//
// �L�����N�^�[�f�[�^
//

class cCharacter {
  Texture own_;

  // �A�j���[�V�����̏��
  struct {
    u_short state[3];
    u_short time_;
    u_short blink_;
    float alpha_;
  } anime;

  struct {
    u_short heal_;
    u_short motion_;
  } time;

  sCharacterInfo info;
  sParameter status;
  sHitPoint hp;

  float turn_;
  u_short lane_;   // ����F�U���̑_���₷���ɉe��

  u_short map_id_;
  u_short effect_;
  
  // �o�b�h�X�e�[�^�X
  struct {
    short trap_time_;
    short poison_time_;
  } bad;

  Vec2f target_;
  Vec2f move_;
  Vec2f pos_;
  Vec2f cut_;

  void initHpMax();
  void initImage();

public:
  cCharacter();

  /* �퓬�V�X�e���֌W */

  void posInit(const Vec2f&, const u_short&, const u_short&);

  void timeCount();
  void timeCountReset();

  bool isWait() const;
  bool isAnimePlaying();
  bool isDead();

  bool targetLook(const short&);
  u_short getPower(const u_short&);
  void physicalDamage(const u_short&);
  void magicalDamage(const u_short&);
  void recovery(const u_short&);

  Color hpColor() const;
  float hpGaugeRatio() const;
  Color timeColor() const;
  float timeGaugeRatio() const;

  void battleFinish();


  /* �p�����[�^����i�f�[�^�o�́j�֌W */

  sParameter getStatus() const;

  Vec2f getPos() const;
  u_short getLane() const;

  u_short hitRatio() const;
  u_short avoidRatio() const;


  /* �p�����[�^����i�ݒ�j�֌W */

  void aliasTargetPos(const Vec2f&);

  void growth();
  void classChange();
  void genderChange();
  void laneShift();


  /* �A�j���[�V�����֌W */

  void animePlay();
  void animeShift();
  void animeChange(const u_short&);
  void draw(const float&);


  /* �f�[�^�Ǘ��֌W */

  void fileLoad(std::ifstream&);

  sCharacterInfo getInfo() const;
  sParameter getBaseStatus() const;
  sHitPoint getHp() const;


  /* �G��p */

  void initEnemy(const short&, const short&);
  void growthEnemy(const short);
  Color levelColor();


  /* ��Ԉُ�ݒ� */

  void trap();
  void poison();

  void drawMinUnit(const Vec2f&);
};
