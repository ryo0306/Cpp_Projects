
#pragma once


namespace literal_common {

  // 仕様
  enum Specification {
    Fps = 60,

    BattleMember = 5,   // 戦闘に参加できる人数の上限
    Reward = 100,       // 敵を倒した時の賞金の基本額

    All_Stage = 12,     // ステージの数
    Boss_Stage = 11,    // ボスが出現する条件
  };

  // 難易度
  enum Difficulty {
    EASY,
    NORMAL,
    HARD,
  };
}

typedef literal_common::Specification  spec;
typedef literal_common::Difficulty     dif;
