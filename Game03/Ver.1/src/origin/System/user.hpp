
#pragma once
#include "../common.h"
#include "../Object/character.h"
#include <list>

//
// ユーザーデータ
// TIPS: 外部に保存するデータ類
//     : データの管理は cDataManager クラスで行う
//

class cUser {
public:
  cUser() = default;

  // ゲームの難易度
  // TIPS: 新規ゲーム開始した時のみ値を設定
  short dif_;

  // 読み込むマップデータと獲得済み判定
  // TIPS: 新規ゲーム開始した時のみ、ランダムに値を設定
  struct {
    bool flag_;           // 所有フラグ
    bool lose_;           // すでに戦闘済み＝１回は敗北している
    u_short id_;          // 読み込むデータの番号
    u_short attribute_;   // 特殊効果
  } map_info[design::AllStage];

  // 各種カウント
  // TIPS: ゲームの難易度、エンディングの評価で使用
  struct {
    u_short clear_;     // ゲームのクリア回数
    u_short battle_;    // 戦闘回数
    u_short command_;   // 戦略コマンドを実行した回数
    u_short worker_;    // 資金繰り：兵士派遣の回数
  } count;

  // 戦略コマンド
  // TIPS: ポーズ画面で使用する
  struct {
    u_int money_;      // 開発用資金

    u_short fire_;     // 火炎瓶
    u_short cannon_;   // 砲撃
    u_short trap_;     // 罠
    u_short poison_;   // 毒矢
  } weapon;

  // ユニット情報
  std::list<cCharacter> player;   // プレイヤー
};
