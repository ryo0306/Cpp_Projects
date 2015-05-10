
#pragma once


namespace literal_map {

  // マップ情報
  enum MapInfo {
    Grass,
    Forest,
    Mount,

    All_Map,
  };

  // マップの特殊効果
  enum MapEffect {
    Plain,

    Fierce,       // 前列の攻撃力・アップ、防御力・ダウン
    Fortress,     // 後列の防御力・アップ、攻撃力・ダウン
    Slowly,       // 全体の敏捷・ダウン
    Unlucky,      // 全体の幸運・ダウン
    Steep,        // スカウト以外の敏捷・大ダウン

    Weapon_Up,    // 兵器の効果がアップ

    Bad_Fire,     // たまに味方一人にダメージ
    Bad_Bomb,     // たまに味方全体にダメージ
    Bad_Trap,     // 味方全体が罠状態でスタート
    Bad_Poison,   // 味方全体が毒状態でスタート

    All_Effect,
  };
}

typedef literal_map::MapInfo    map_id;
typedef literal_map::MapEffect  effect;
