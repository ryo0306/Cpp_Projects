
#include "battle.h"

typedef cBattle  battle;


cBattle::cBattle() :
telop_(FONT_),
next_scene_(data::system.scene[play::Next]),
phase_(data::system.phase_),
stage_id_(data::system.strategy.stage_id_),
fire_(data::system.command.fire_),
cannon_(data::system.command.cannon_),
trap_(data::system.command.trap_),
poison_(data::system.command.poison_),
cnt_fire_(data::user.weapon.fire_),
cnt_cannon_(data::user.weapon.cannon_),
cnt_trap_(data::user.weapon.trap_),
cnt_poison_(data::user.weapon.poison_),
camera_x_(0.f) {

  telop_.size(fontSize::Battle);
}


void battle::update() {

  // TIPS: �G���������S�ł�����퓬�I��
  battleFinish();

  // TIPS: �퓬�I���Ɋւ��J�E���g�l��������
  // * �e�i�K�̏������ɃJ�E���g���擾�A�����𖞂�������퓬���I������
  countInit();

  // TIPS: �v���C���[���L�����N�^�[���ׂĂ̏���
  begin = data::user.player.begin();
  end = data::user.player.end();
  unitUpdate();

  // TIPS: ���ׂĂ̓G�̏���
  begin = enemy.begin();
  end = enemy.end();
  unitUpdate();

  // TIPS: �퓬���j���[��ʂ���g�p�����R�}���h�̏���
  fire_command();
  cannon_command();
  trap_command();
  poison_command();
  command_act();
}


// TIPS: �G���T�̈ȏア��ꍇ�A�c��̓G���t�B�[���h�ɏo��������
void battle::unitSummon() {
  phase_ = phase::Standby;

  enemy.clear();
  short num = order.size();
  if (num > design::BattleMember) { num %= design::BattleMember; }
  if (num == 0) { num = design::BattleMember; }

  for (i = 0; i < num; ++i) {
    enemy.emplace_back(order.front());
    order.pop();
  }

  unitPosTranslate();
}


void battle::unitPosTranslate() {
  u_short& map_id = data::user.map_info[stage_id_].id_;
  u_short& effect = data::user.map_info[stage_id_].attribute_;

  // TIPS: �v���C���[���L�����N�^�[�ʒu��������
  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin, i = 0; it != end; ++it, ++i) {
    pos_.x() =
      -size::Half_W + (size::Unit_W * 8.5f) - size::Unit_W * ((i + it->getLane()));
    pos_.y() = i * -80 + 80;
    it->posInit(pos_, map_id, effect);
  }

  // TIPS: �G�L�����ʒu��������
  begin = enemy.begin();
  end = enemy.end();
  for (it = begin, i = 0; it != end; ++it, ++i) {
    pos_.x() =
      size::Half_W - (size::Unit_W * 8.5f) + size::Unit_W * ((i + it->getLane()));
    pos_.y() = i * -80 + 80;
    it->posInit(pos_, map_id, effect);
  }
}


void battle::enemyStandby() {
  begin = data::system.all_enemy.begin();
  end = data::system.all_enemy.end();
  for (it = begin; it != end; ++it) { order.push(*it); }

  unitSummon();
}


// TIPS: �퓬�s�\�J�E���g�����Z�b�g
void battle::countInit() {
  dead.player_ = 0;
  dead.enemy_ = 0;
}


// TIPS: �퓬�s�\�̃L�����N�^�[���J�E���g
void battle::deadCount() {
  it->getInfo().side_ ? ++dead.player_ : ++dead.enemy_;
}


void battle::battleFinish() {
  // TIPS: ��������...�G���S��
  if (dead.enemy_ == enemy.size()) {
    dead.enemy_ = 0;
    order.empty() ? win() : unitSummon();
  }

  // TIPS: �s�k����...�������S��
  if (dead.player_ == data::user.player.size()) {
    phase_ = phase::Lose;
    next_scene_ = scene::Result;
  }
}


void battle::win() {
  phase_ = phase::Win;
  next_scene_ = scene::Result;
}


void battle::unitUpdate() {
  for (it = begin; it != end; ++it) {
    unit_animePlay();

    // TIPS: �퓬�s�\��Ԃ̃L�����N�^�[�̏������X�L�b�v
    if (it->isDead()) { deadCount(); continue; }

    timeCount();
    unit_action();
  }
}


void battle::timeCount() {
  // TIPS: ���炩�̐퓬�A�j���[�V���������s���Ȃ�J�E���g���X�L�b�v
  if (it->isAnimePlaying()) { return; }
  it->timeCount();
}


void battle::unit_animePlay() {
  it->animeShift();
  it->animePlay();
}


void battle::unit_action() {
  // TIPS: �ҋ@���Ԓ��Ȃ珈�����X�L�b�v
  if (it->isWait()) { return; }
  it->timeCountReset();
  it->animeChange(action::Attack);

  // TIPS: ���g�̃N���X�ɉ������s����I��
  it->getInfo().type_ == type::Mage ? magicalAttack()
    : it->getInfo().type_ == type::Priest ? cure()
    : physicalAttack();
}


void battle::physicalAttack() {
  bool attack = false;

  // TIPS: �s�����̃L�����ɓG�΂��Ă��鑊��̏����擾
  it->getInfo().side_ ? getEnemyInfo() : getPlayerInfo();

  for (target = begin_; target != end_; ++target) {
    // TIPS: �^�[�Q�b�g���퓬�s�\�Ȃ�U���Ώۂɂ��Ȃ�
    if (target->isDead()) { continue; }

    // TIPS: ���g�̃N���X�ƃ^�[�Q�b�g�̑�������؁A�U���ł��邩�𔻒�
    //     : �U���\�Ȃ画�菈�����I���A�U������
    if (it->targetLook(target->getLane())) { attack = true; break; }
  }

  // TIPS: �U���\�ȑ���ɓG��������Ȃ���΁A�Ō�񂩂�t���ɍU������
  if (!attack) {
    for (--target; target != begin; --target) {
      // TIPS: �^�[�Q�b�g���퓬�s�\�Ȃ�U���Ώۂɂ��Ȃ�
      if (target->isDead()) { continue; }
      if (!it->targetLook(target->getLane())) { attack = true; break; }
    }
  }

  // TIPS: ����������s���A�����Ȃ�_���[�W��^����
  target->avoidRatio() < data::random.fromZeroToLast(it->hitRatio())
    ? target->physicalDamage(it->getPower(target->getInfo().gender_))
    : target->animeChange(action::Avoid);
}


void battle::magicalAttack() {
  dead.count_ = 0;

  // TIPS: �s�����̃L�����ɓG�΂��Ă��鑊��̏����擾
  it->getInfo().side_ ? getEnemyInfo() : getPlayerInfo();

  // TIPS: �퓬�s�\��Ԃ̃L�����N�^�[�̐����擾
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { ++dead.count_; }
  }

  // TIPS: �l���������قǁA�P�l������̃_���[�W�ʂ�����
  //     : �}�b�v���ʂِ̈��ɑ΂���U���̓A�b�v�̂ݖ���
  u_short temp_damage = it->getPower(2) / (i - dead.count_);

  // TIPS: �퓬�s�\�ȃL�����ȊO�̑S�̂Ƀ_���[�W��^����
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { continue; }
    target->magicalDamage(temp_damage);
  }
}


void battle::cure() {
  dead.count_ = 0;

  // TIPS: �����̏����擾
  it->getInfo().side_ ? getPlayerInfo() : getEnemyInfo();

  // TIPS: �퓬�s�\��Ԃ̃L�����N�^�[�̐����擾
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { ++dead.count_; }
  }

  // TIPS: �l���������قǁA�P�l������̉񕜗ʂ�����
  //     : �}�b�v���ʂِ̈��ɑ΂�����ʃA�b�v�̂ݖ���
  u_short temp_recover = it->getPower(2) / (i - dead.count_);

  // TIPS: �퓬�s�\�ȃL�����ȊO����
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { continue; }
    target->recovery(temp_recover);
  }
}


void battle::fire_command() {
  // TIPS: �퓬���j���[����R�}���h���s�����܂ŏ��������s���Ȃ�
  if (!fire_) { return; }

  short temp_hp = 0;
  std::list<cCharacter>::iterator hit, target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: �ł��c��HP�̍����G������
  for (target0 = begin0; target0 != end0; ++target0) {
    if (temp_hp < target0->getHp().now_) {
      temp_hp = target0->getHp().now_;
      hit = target0;
    }
  }

  // TIPS: �ő�HP�ɉ������A�����_���[�W��^����
  //     : �����_���[�W�Ƃ��Ĉ���
  hit->physicalDamage(hit->getHp().max_ * 0.5f);
}


void battle::cannon_command() {
  // TIPS: �퓬���j���[����R�}���h���s�����܂ŏ��������s���Ȃ�
  if (!cannon_) { return; }

  dead.count_ = 0;
  std::list<cCharacter>::iterator target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: �퓬�s�\�ł͂Ȃ��G�̐����擾
  for (target0 = begin0; target0 != end0; ++target0) {
    if (target0->isDead()) { ++dead.count_; }
  }

  // TIPS: �l���ɉ������_���[�W�̊������擾
  float temp_per = 6 - enemy.size() + dead.count_;
  temp_per *= 0.1f;

  // TIPS: �G�S�̂ɍő�HP�ɉ������A�����_���[�W��^����
  //     : �����_���[�W�Ƃ��Ĉ���
  for (target0 = begin0; target0 != end0; ++target0) {
    target0->physicalDamage(target0->getHp().max_ * temp_per);
  }
}


void battle::trap_command() {
  // TIPS: �퓬���j���[����R�}���h���s�����܂ŏ��������s���Ȃ�
  if (!trap_) { return; }

  std::list<cCharacter>::iterator target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: �m����㩏�Ԃɂ���
  for (target0 = begin0; target0 != end0; ++target0) {
    if (target0->getStatus().luk_ / 2 < data::random.fromZeroToLast(1000)) {
      target0->trap();
    }
  }
}


void battle::poison_command() {
  // TIPS: �퓬���j���[����R�}���h���s�����܂ŏ��������s���Ȃ�
  if (!poison_) { return; }

  std::list<cCharacter>::iterator target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: �m���œŏ�Ԃɂ���
  for (target0 = begin0; target0 != end0; ++target0) {
    if (target0->getStatus().luk_ / 4 < data::random.fromZeroToLast(1000)) {
      target0->poison();
    }
  }
}


// TIPS: �g�p���ꂽ�R�}���h�ɑΉ������J�E���g�������A�t���O������
void battle::command_act() {
  if (fire_) {
    fire_ = false;
    --cnt_fire_;
  }

  if (cannon_) {
    cannon_ = false;
    --cnt_cannon_;
  }

  if (trap_) {
    trap_ = false;
    --cnt_trap_;
  }

  if (poison_) {
    poison_ = false;
    --cnt_poison_;
  }
}


// TIPS: �v���C���[���̃L�����N�^�[���s�����Ȃ�A�G�̏����擾
void battle::getEnemyInfo() {
  begin_ = enemy.begin();
  end_ = enemy.end();
}


// TIPS: �G�L�������s�����Ȃ�v���C���[�̏����擾������
void battle::getPlayerInfo() {
  begin_ = data::user.player.begin();
  end_ = data::user.player.end();
}


void battle::init() {
  // TIPS: �}�b�v���̏�����
  map_data_.mapInit(data::user.map_info[stage_id_].id_);

  phase_ = phase::Standby;
  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  enemyStandby();

  countInit();
  camera_x_ = 0.f;

  fire_ = false;
  cannon_ = false;
  trap_ = false;
  poison_ = false;

  // TIPS: BGM�̃v���C�i���o�[���擾
  short play_num = data::getTerritory() == design::AllStage - 1 ?
    data::random.fromFirstToLast(bgm::Boss1, bgm::Boss2) :
    data::random.fromFirstToLast(bgm::Battle1, bgm::Battle3);

  data::music.stop();
  data::music.play(play_num);
}


void battle::draw() {
  //++anime.time_;
  //if (anime.time_ >= size::WIDTH * 2) { anime.time_ = 0; }
  map_data_.draw(anime.time_);

  dispUnitInfo();

  // TIPS: �v���C���[���̃L�����N�^�[���ׂĂ̏���
  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin, i = 0; it != end; ++it, ++i) {
    it->draw(it->isDead() ? -camera_x_ : 0.f);
    dispPlayerInfo();
  }

  // TIPS: ���ׂĂ̓G�L�����̏���
  begin = enemy.begin();
  end = enemy.end();
  for (it = begin; it != end; ++it) {
    it->draw(camera_x_);
    dispEnemyInfo();
  }
}


void battle::dispUnitInfo() {

  // TIPS: �E�B���h�E������\���A���ۂ̃f�[�^�̓L�����N�^�[�`��Ɠ����ɍs��
  for (i = 0; i < 5; ++i) {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1);
    pos_.y() = -size::HEIGHT / 2;

    win::draw(pos_, Vec2f(size::UnitInfo_W, size::UnitInfo_H),
      win::color(paint::Blue, 0.8f));
  }
}


// TIPS: �v���C���[���̃L�����N�^�[�̏���\��
void battle::dispPlayerInfo() {

  // ���x���Ɩ��O
  {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1) + size::NamePos_X;
    pos_.y() = -size::HEIGHT / 2 + size::NamePos_Y;
    sstream level;
    level << "LV:" << std::setw(3) << it->getInfo().level_ << " " << it->getInfo().name_;
    telop_.draw(level.str(), pos_, win::color(paint::White));
  }

  // HP�Q�[�W
  {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1) + size::GaugePos_X;
    pos_.y() = -size::HEIGHT / 2 + size::GaugePos_Y * 2.5f;

    win::gaugeDraw(pos_, Vec2f(size::Gauge_W, size::Gauge_H),
      it->hpGaugeRatio(), it->hpColor());
  }

  // �^�C���Q�[�W
  {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1) + size::GaugePos_X;
    pos_.y() = -size::HEIGHT / 2 + size::GaugePos_Y;

    win::gaugeDraw(pos_, Vec2f(size::Gauge_W, size::Gauge_H),
      it->timeGaugeRatio(), it->timeColor());
  }
}


void battle::dispEnemyInfo() {

  // ���O
  sstream name;
  name << "LV:" << std::setw(3) << it->getInfo().level_
    << " " << it->getInfo().name_;

  pos_.x() =
    it->getPos().x() - telop_.drawSize(name.str()).x() / 2;
  pos_.y() = it->getPos().y() + size::Unit_W * 3.5f;
  telop_.draw(name.str(), pos_, it->levelColor());

  // HP�Q�[�W
  pos_.x() = it->getPos().x() - size::Gauge_W / 4;
  pos_.y() -= 20;
  win::gaugeDraw(pos_, Vec2f(size::Gauge_W / 2, size::Gauge_H * 0.75f),
    it->timeGaugeRatio(), it->timeColor());

  // �^�C���Q�[�W
  pos_.y() -= 20;
  win::gaugeDraw(pos_, Vec2f(size::Gauge_W / 2, size::Gauge_H * 0.75f),
    it->hpGaugeRatio(), it->hpColor());
}
