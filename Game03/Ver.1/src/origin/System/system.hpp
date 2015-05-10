
#pragma once
#include "../common.h"
#include "../Object/character.h"
#include <list>
#include <queue>

//
// システムデータ
// TIPS: 外部に保存しないデータ類
//     : データの管理は cDataManager クラスで行う
//

class cSystem {
public:
  cSystem() = default;

  // ポーズ画面判定
  bool pause_;

  // ゲームのクリアフラグ
  // TIPS: 初期化処理を切り替える
  bool clear_;

  // 描画シーンの状態
  // TIPS: 起動時の初期化は cGameManager クラスで行う
  // * 以降は scene_manager クラス内で管理
  // * play::Now, Last, Next で各段階の描画状態を参照する
  state::SceneMode scene[3];

  // 戦略画面の状態
  struct {
    mode mode_;          // コマンド選択状態
    u_short draw_state_;   // ボタン描画状態
    u_short stage_id_;     // 侵攻コマンド選択マス
  } strategy;

  // 戦闘中に使用されたコマンドのフラグ
  struct {
    bool fire_;
    bool cannon_;
    bool trap_;
    bool poison_;
  } command;

  // 戦闘シーンの状態
  // TIPS: 戦闘、リザルトの各シーン内で処理を切り替える
  // * phase::Win, Lose でリザルトの処理を切り替える
  state::BattlePhase phase_;

  // ユニット情報
  std::list<cCharacter> all_enemy;   // 戦闘に参加できる全ての敵
};
