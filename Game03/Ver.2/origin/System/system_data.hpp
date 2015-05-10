
#pragma once
#include "../Object/character.h"
#include "../Literal/literal_scene.hpp"
#include <array>
#include <queue>


class cSystemData {
public:
  cSystemData() = default;

  // ゲームのクリアフラグ
  bool game_clear_;

  // 描画状態と段階
  // TIPS: play::Now, Next, Last で各段階の描画状態を参照する
  std::array<scene, play::All_State> scene_;
  // NOTICE: 初期化のみ game_player で行う
  //       : 以降は scene_manager で管理

  // ポーズ画面のフラグ
  // TIPS: ポーズ中なら true を保持
  bool pause_;

  // 戦闘準備画面
  struct {
    short draw_;   // 演出の描画状態
    short mode_;   // コマンドの選択状態
  } strategy_state;

  // 敵キャラ関係
  struct {
    short reward_;
    std::queue<cCharacter> order_;
  } enemy;
};
