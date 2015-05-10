
#pragma once
#include "../Object/character.h"
#include <array>
#include <list>


class cUserData {
  struct MapInfo {
    bool territory_;
    bool lose_;
    short id_;
    short attribute_;
  };

public:
  cUserData() = default;

  // ゲームの難易度
  // TIPS: 新規ゲーム開始した時のみ変更を加える
  short dif_;

  // 資産
  struct {
    u_int money_;   // 所持金
    short food_;    // 食糧：兵士派遣の残り回数
  } assets;

  // TIPS: ゲームの進行状態など、システムで使用するカウント
  struct {
    short game_clear_;   // ゲームのクリア回数
    short battle_;       // 戦闘回数
    short lose_;         // 敗北、撤退した回数
    short battle_cmd_;   // 戦闘中のコマンド使用回数
    short work_;         // 兵士派遣の回数
  } cnt;

  // TIPS: 開発した兵器の残り使用回数
  struct {
    short fire_;     // 火炎瓶
    short bomb_;     // 砲撃
    short trap_;     // 罠
    short poison_;   // 毒矢
  } weapon_cnt;

  // マップデータ
  // TIPS: フラグと読み込む画像のID、マップの効果を保持
  // * 新規ゲーム開始したときのみ変更を加える
  std::array<MapInfo, spec::All_Stage> map_info_;

  // ユニット情報
  std::list<cCharacter> player;
};
