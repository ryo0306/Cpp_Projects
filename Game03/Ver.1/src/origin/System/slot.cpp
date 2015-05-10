
#include "slot.h"


std::string getFilePass(const short& slot0) {
  std::string temp;

  switch (slot0) {
    case  0: temp = "res/data/user_data1.txt"; break;
    case  1: temp = "res/data/user_data2.txt"; break;
    case  2: temp = "res/data/user_data3.txt"; break;
    default: temp = "res/data/user_data1.txt";
  }

  return temp;
}


bool Slot::saveUserData(const short& slot) {
  std::ofstream save(getFilePass(slot));
  if (save.fail()) { return false; }

  // TIPS: �Q�[���̓�Փx��ۑ�
  save << data::user.dif_ << std::endl;

  // TIPS: �}�b�v�f�[�^��ۑ�
  for (short i = 0; i < design::AllStage; ++i) {
    save
      << data::user.map_info[i].flag_ << " "
      << data::user.map_info[i].lose_ << " "
      << data::user.map_info[i].id_   << " "
      << data::user.map_info[i].attribute_ << std::endl;
  }

  // TIPS: �N���A�񐔂ȂǁA�V�X�e���Ɋւ��f�[�^�̕ۑ�
  save
    << data::user.count.clear_   << " "
    << data::user.count.battle_  << " "
    << data::user.count.command_ << " "
    << data::user.count.worker_  << std::endl;;

  // TIPS: �헪�R�}���h�̃f�[�^��ۑ�
  save
    << data::user.weapon.money_  << " "
    << data::user.weapon.fire_   << " "
    << data::user.weapon.cannon_ << " "
    << data::user.weapon.trap_   << " "
    << data::user.weapon.poison_ << std::endl;

  // TIPS: �쐬�ς݂̃L�����N�^�[����ۑ�
  save << data::user.player.size() << std::endl;

  // TIPS: �L�����N�^�[�f�[�^��ۑ�
  std::list<cCharacter>::iterator it;
  for (it = data::user.player.begin(); it != data::user.player.end(); ++it) {
    save
      << it->getInfo().side_   << " "
      << it->getInfo().name_   << " "
      << it->getInfo().type_   << " "
      << it->getInfo().gender_ << " "
      << it->getInfo().level_  << " "

      << it->getLane() << " "

      << it->getHp().now_ << " "
      << it->getHp().max_ << " "

      << it->getBaseStatus().pow_ << " "
      << it->getBaseStatus().vit_ << " "
      << it->getBaseStatus().mag_ << " "
      << it->getBaseStatus().spd_ << " "
      << it->getBaseStatus().dex_ << " "
      << it->getBaseStatus().luk_ << std::endl;
  }

  return true;
}


bool Slot::loadUserData(const short& slot) {
  std::ifstream load(getFilePass(slot));
  if (load.fail()) { return false; }

  // TIPS: �Q�[���̓�Փx���擾
  load >> data::user.dif_;

  // TIPS: �}�b�v�f�[�^���擾
  for (short i = 0; i < design::AllStage; ++i) {
    load
      >> data::user.map_info[i].flag_
      >> data::user.map_info[i].lose_
      >> data::user.map_info[i].id_
      >> data::user.map_info[i].attribute_;
  }

  // TIPS: �N���A�񐔂ȂǁA�V�X�e���Ɋւ��f�[�^�̎擾
  load
    >> data::user.count.clear_
    >> data::user.count.battle_
    >> data::user.count.command_
    >> data::user.count.worker_;

  // TIPS: �헪�R�}���h�̃f�[�^���擾
  load
    >> data::user.weapon.money_
    >> data::user.weapon.fire_
    >> data::user.weapon.cannon_
    >> data::user.weapon.trap_
    >> data::user.weapon.poison_;

  // TIPS: �쐬�ς݂̃L�����N�^�[�����擾�A�z����m�ۂ���
  short num;
  load >> num;
  data::user.player.clear();
  for (short i = 0; i < num; ++i) { data::user.player.emplace_back(); }

  // TIPS: �L�����N�^�[�f�[�^���擾
  std::list<cCharacter>::iterator it, begin, end;
  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin; it != end; ++it) { it->fileLoad(load); }

  return true;
}


void Slot::getSlotData(cUser& temp, const short& slot) {
  std::ifstream fstr(getFilePass(slot));
  if (fstr.fail()) { return; }

  // TIPS: �Q�[���̓�Փx���擾
  fstr >> temp.dif_;

  // TIPS: �}�b�v�f�[�^���擾
  for (short i = 0; i < design::AllStage; ++i) {
    fstr
      >> temp.map_info[i].flag_
      >> temp.map_info[i].lose_
      >> temp.map_info[i].id_
      >> temp.map_info[i].attribute_;
  }

  // TIPS: �N���A�񐔂ȂǁA�V�X�e���Ɋւ��f�[�^�̎擾
  fstr
    >> temp.count.clear_
    >> temp.count.battle_
    >> temp.count.command_
    >> temp.count.worker_;

  // TIPS: �헪�R�}���h�̃f�[�^���擾
  fstr
    >> temp.weapon.money_
    >> temp.weapon.fire_
    >> temp.weapon.cannon_
    >> temp.weapon.trap_
    >> temp.weapon.poison_;

  // TIPS: �쐬�ς݂̃L�����N�^�[�����擾�A�z����m�ۂ���
  short num;
  fstr >> num;
  temp.player.clear();
  for (short i = 0; i < num; ++i) { temp.player.emplace_back(); }

  // TIPS: �L�����N�^�[�f�[�^���擾
  std::list<cCharacter>::iterator it, begin, end;
  begin = temp.player.begin();
  end = temp.player.end();
  for (it = begin; it != end; ++it) { it->fileLoad(fstr); }
}
