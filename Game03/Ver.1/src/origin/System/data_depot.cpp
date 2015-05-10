
#include "data_depot.h"


cSystem data::system;
cUser   data::user;
Random  data::random;

cMusicResource data::music;


// �f�[�^�̐������`�F�b�N
// TIPS: ���[�v�̍ŏ��Ɏ��s����
void data::bugProof() {
}


// �Q�[���J�n���̕`��V�[���w��
// TIPS: ��{�I�Ƀf�o�b�O�p�B�����łł� Title �݂̂��w�肷��
void data::sceneStart(const state::SceneMode& scene0) {
  system.scene[play::Last] = scene::None;
  system.scene[play::Now] = scene::None;
  system.scene[play::Next] = scene0;

  system.pause_ = false;
  system.strategy.mode_ = mode::Neutral;
  system.phase_ = phase::Standby;
}


// �V�K�Q�[��
void data::newGame(const short& difficulty) {
  user.dif_ = difficulty;

  // TIPS: �������g���ăX�e�[�W�̏�Ԃ�ݒ�
  short r = random.fromZeroToLast(design::AllStage);
  for (short i = 0; i < design::AllStage; ++i) {
    user.map_info[i].flag_ = (r == i) ? true : false;
    user.map_info[i].lose_ = false;
    user.map_info[i].id_ = random.fromZeroToLast(9999) % 3;
    user.map_info[i].attribute_ = random.fromZeroToLast(effect::All_Effect);
  }

  user.count.clear_ = 0;
  user.count.battle_ = 0;
  user.count.command_ = 0;
  user.count.worker_ = 0;

  user.weapon.money_ = 1000;

  user.weapon.fire_ = 0;
  user.weapon.cannon_ = 0;
  user.weapon.trap_ = 0;
  user.weapon.poison_ = 0;

  // TIPS: �|�[�Y��ʂ���߂��Ă����ꍇ������̂ŁA�K������������
  user.player.clear();
  for (short i = 0; i < design::BattleMember; ++i) {
    user.player.emplace_back();
  }
}


// �����ăj���[�Q�[��
void data::reStart(const short& difficulty) {
  user.dif_ = difficulty;

  // TIPS: �������g���ăX�e�[�W�̏�Ԃ�ݒ�
  short r = random.fromZeroToLast(design::AllStage);

  for (short i = 0; i < design::AllStage; ++i) {
    user.map_info[i].flag_ = (r == i) ? true : false;
    user.map_info[i].lose_ = false;
    user.map_info[i].id_ = random.fromZeroToLast(9999) % 3;
    user.map_info[i].attribute_ = random.fromZeroToLast(effect::All_Effect);
  }

  ++user.count.clear_;
  user.count.battle_ = 0;
  user.count.command_ = 0;
  user.count.worker_ = 0;

  // TIPS: �N���A�ς݃t���O�����Z�b�g
  system.clear_ = false;
}


void data::enemySpown() {
  std::list<cCharacter>::iterator it, begin, end;

  // TIPS: �G�L�����̃f�[�^�͂��ׂč�蒼���̂Ń��Z�b�g
  system.all_enemy.clear();

  // TIPS: �Q�[���̐i�s�x�ɍ��킹�ēG�̐���ݒ�
  short enemy_num = 2 + user.count.clear_;
  enemy_num += getTerritory() + (user.count.battle_ + 1) / (4 - user.dif_);

  // TIPS: �ŏI�X�e�[�W�Ȃ�{�X���o��������
  short boss_spown = getTerritory() == design::AllStage - 1 ?
    user.dif_ + 1 : 0;

  enemy_num += boss_spown;

  // TIPS: �G�̃f�[�^����
  for (short i = 0; i < enemy_num; ++i) {
    system.all_enemy.emplace_back();
  }

  // TIPS: �v���C���[���̕��σ��x�����擾
  short lv_average = 0;
  begin = user.player.begin();
  end = user.player.end();

  for (it = begin; it != end; ++it) {
    lv_average += it->getInfo().level_;
  }

  lv_average /= user.player.size();
  if (lv_average < 1) { lv_average = 1; }

  // TIPS: �o�����̃��x�������擾
  short lv_min = lv_average - (5 - user.dif_ * 3);
  short lv_max = lv_average + user.dif_ * 3;
  if (lv_min < 1) { lv_min = 1; }
  if (lv_max <= lv_min) { lv_max = lv_min + 1; }

  // TIPS: ��Փx���ƂɃ��x�������ݒ�
  switch (user.dif_) {
    case dif::EASY:
      if (lv_min > 50) { lv_min = 50; }
      if (lv_max > 50) { lv_max = 50; }
      break;

    case dif::NORMAL:
      if (lv_min > 100) { lv_min = 50; }
      if (lv_max > 100) { lv_max = 50; }
      break;

    case dif::HARD:
      if (lv_min > 200) { lv_min = 50; }
      if (lv_max > 200) { lv_max = 50; }
      break;

    default:;
  }

  // TIPS: �G�̃��x���Ȃǂ�ݒ�
  begin = system.all_enemy.begin();
  end = system.all_enemy.end();
  short num;
  short type0, gen0;
  for (it = begin, num = 0; it != end; ++it, ++num) {
    type0 = data::random.fromZeroToLast(1000) % character::Boss;
    gen0  = data::random.fromZeroToLast(1000) % 2;

    if (num >= enemy_num - boss_spown) {
      type0 = character::Boss;
    }

    // TIPS: �G�̕���A���ʂ�ݒ�
    it->initEnemy(type0, gen0);

    // TIPS: �S�̂̃��x�����v���C���[���̕��σ��x���Ɠ����ɂ���
    it->growthEnemy(data::random.fromFirstToLast(lv_min, lv_max));
  }
}


// �l���ςݗ̒n�̐����擾
short data::getTerritory() {
  short num = 0;

  for (short i = 0; i < design::AllStage; ++i) {
    if (user.map_info[i].flag_) { ++num; }
  }

  return num;
}
