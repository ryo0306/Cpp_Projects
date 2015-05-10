
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

  // TIPS: ゲームの難易度を保存
  save << data::user.dif_ << std::endl;

  // TIPS: マップデータを保存
  for (short i = 0; i < design::AllStage; ++i) {
    save
      << data::user.map_info[i].flag_ << " "
      << data::user.map_info[i].lose_ << " "
      << data::user.map_info[i].id_   << " "
      << data::user.map_info[i].attribute_ << std::endl;
  }

  // TIPS: クリア回数など、システムに関わるデータの保存
  save
    << data::user.count.clear_   << " "
    << data::user.count.battle_  << " "
    << data::user.count.command_ << " "
    << data::user.count.worker_  << std::endl;;

  // TIPS: 戦略コマンドのデータを保存
  save
    << data::user.weapon.money_  << " "
    << data::user.weapon.fire_   << " "
    << data::user.weapon.cannon_ << " "
    << data::user.weapon.trap_   << " "
    << data::user.weapon.poison_ << std::endl;

  // TIPS: 作成済みのキャラクター数を保存
  save << data::user.player.size() << std::endl;

  // TIPS: キャラクターデータを保存
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

  // TIPS: ゲームの難易度を取得
  load >> data::user.dif_;

  // TIPS: マップデータを取得
  for (short i = 0; i < design::AllStage; ++i) {
    load
      >> data::user.map_info[i].flag_
      >> data::user.map_info[i].lose_
      >> data::user.map_info[i].id_
      >> data::user.map_info[i].attribute_;
  }

  // TIPS: クリア回数など、システムに関わるデータの取得
  load
    >> data::user.count.clear_
    >> data::user.count.battle_
    >> data::user.count.command_
    >> data::user.count.worker_;

  // TIPS: 戦略コマンドのデータを取得
  load
    >> data::user.weapon.money_
    >> data::user.weapon.fire_
    >> data::user.weapon.cannon_
    >> data::user.weapon.trap_
    >> data::user.weapon.poison_;

  // TIPS: 作成済みのキャラクター数を取得、配列を確保する
  short num;
  load >> num;
  data::user.player.clear();
  for (short i = 0; i < num; ++i) { data::user.player.emplace_back(); }

  // TIPS: キャラクターデータを取得
  std::list<cCharacter>::iterator it, begin, end;
  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin; it != end; ++it) { it->fileLoad(load); }

  return true;
}


void Slot::getSlotData(cUser& temp, const short& slot) {
  std::ifstream fstr(getFilePass(slot));
  if (fstr.fail()) { return; }

  // TIPS: ゲームの難易度を取得
  fstr >> temp.dif_;

  // TIPS: マップデータを取得
  for (short i = 0; i < design::AllStage; ++i) {
    fstr
      >> temp.map_info[i].flag_
      >> temp.map_info[i].lose_
      >> temp.map_info[i].id_
      >> temp.map_info[i].attribute_;
  }

  // TIPS: クリア回数など、システムに関わるデータの取得
  fstr
    >> temp.count.clear_
    >> temp.count.battle_
    >> temp.count.command_
    >> temp.count.worker_;

  // TIPS: 戦略コマンドのデータを取得
  fstr
    >> temp.weapon.money_
    >> temp.weapon.fire_
    >> temp.weapon.cannon_
    >> temp.weapon.trap_
    >> temp.weapon.poison_;

  // TIPS: 作成済みのキャラクター数を取得、配列を確保する
  short num;
  fstr >> num;
  temp.player.clear();
  for (short i = 0; i < num; ++i) { temp.player.emplace_back(); }

  // TIPS: キャラクターデータを取得
  std::list<cCharacter>::iterator it, begin, end;
  begin = temp.player.begin();
  end = temp.player.end();
  for (it = begin; it != end; ++it) { it->fileLoad(fstr); }
}
