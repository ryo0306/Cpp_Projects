
#pragma once
#include "../lib/defines.hpp"
#include "../lib/appEnv.hpp"
#include "../lib/font.hpp"
#include <string>
#include <sstream>
#include <iomanip>

//
// システム共通
//

/* マクロ、定数 */
//////////////////////////////////////////////////////////////////////

#define FONT_   "res/font/arare.ttf"


// 画像など、サイズに関わるもの
namespace size {

  // アプリのウィンドウサイズ
  enum GameWindow {
    WIDTH  = 1024,
    HEIGHT = 768,

    Half_W = WIDTH / 2,
    Half_H = HEIGHT / 2,
  };

  // 画像の切り取りサイズ
  enum CutSize {
    Unit_W = 32,
    Unit_H = 32,

    Icon_W = 64,
    Icon_H = 64,

    Logo_W = 256,
    Logo_H = 48,

    Rect_128 = 128,
    Rect_256 = 256,
    Rect_512 = 512,
    Rect_1024 = 1024,
  };

  // フォントサイズ
  enum FontSize {
    Info = 24,
    Battle = 20,
    Making = 30,
    Small = 40,
    Medium,
    Large = 60,
  };

  // ボタン描画サイズ
  enum Button {
    Small_W = 200,
    Small_H = 60,

    Medium_W = 250,
    Medium_H = 60,

    Large_W = 400,
    Large_H = 80,

    MenuIcon_W = 128,
    MenuIcon_H = 128,
  };

  // タイトル画面
  enum Title {
    LogoPos_Y = HEIGHT / 4,
    ModePos_Y = -HEIGHT / 8,
  };

  // セーブロード画面
  enum DataSelect {
    SlotPos_X = -Half_W + 100,
    SlotPos_Y = HEIGHT / 6,
    Slot_H = 90,

    InfoPos_X = -110,
    InfoPos_Y = SlotPos_Y - Slot_H * 4,
    Info_W = 500,
    Info_H = Slot_H * 4 + 60,

    SlotData_X = -80,
    SlotData_Y = 150,
  };

  // ポーズ画面
  enum Pause {
    PausePos_Y = HEIGHT / 4,
    Attension_Y = HEIGHT / 8,
  };

  // 戦略画面
  enum Strategy {
    Command_H = 60,
    CommandPos_X = -WIDTH / 2 + 10,
    CommandPos_Y = HEIGHT / 2 - Command_H - 10,

    DataPos_X = 30,
    DataPos_Y = Half_H - 60,
    UnitData_Y = 90,

    InvasionPos_X = -160,
    InvasionPos_Y = Half_H - 240,
    Invasion_W = 130,
    Invasion_H = 130,

    Bottom_W = WIDTH - 20,
    Bottom_H = 140,
    BottomPos_X = -WIDTH / 2 + 10,
    BottomPos_Y = -HEIGHT / 2 + 10,
  };

  // 戦闘画面
  enum Battle {
    UnitInfo_W = WIDTH / 5,
    UnitInfo_H = 128,

    Gauge_W = 164,
    Gauge_H = 18,
    GaugePos_X = 20,
    GaugePos_Y = 20,

    NamePos_X = 20,
    NamePos_Y = 90,
  };

  // リザルト画面
  enum Result {
    Result_Y = HEIGHT / 2 - 150,

    ScorePos_X = -380,
    ScorePos_Y = Result_Y - 100,
  };
}


// システムで使う固有値
namespace design {

  // 仕様
  enum System {
    Fps = 60,             // １秒 = 60.frame

    BattleMember = 5,     // 出撃メンバーの上限（敵味方共通）
    Score = 100,          // 敵を倒した時のスコア基本値

    GameStart = 1,        // ゲーム開始時の陣地の数
    AllStage  = 12,       // 陣地の数：すべての陣地を獲得した

    NameMaxSize = 5,      // 名前の最大文字数

    DefaultPos = 300,     // 戦略画面コマンドボタンの初期位置

    InvasionIcon_X = 4,   // 侵攻コマンドのマスの数：ｘ方向
    InvasionIcon_Y = 3,   // 侵攻コマンドのマスの数：ｙ方向
  };

  // ゲーム難易度
  // TIPS: 敵キャラのパラメータに影響
  enum Difficulty {
    EASY,
    NORMAL,
    HARD,
  };

  // ゲームクリアの評価基準
  enum ClearRank {
    S = 30,
    A = 60,
    B = 90,
  };

  // 兵器開発の価格
  enum WeaponPrice {
    Fire = 1000,
    Bomb = 3000,
    Trap = 3000,
    Poison = 2000,
  };

  // 色設定
  // TIPS: 色データを管理している配列の要素を指定するときに使う
  enum ColorType {
    White,
    Gray,
    Black,
    Red,
    Pink,
    Purple,
    Blue,
    SkyBlue,
    Green,
    Yellow,
    Orange,
    Brown,

    // TIPS: 配列用データ
    // * 後から色を追加しても、これを指定していれば勝手に配列のサイズが変わる
    ColorVariation,
    // DANGER: この下には色データを作らないこと
  };
}


// 各シーンの処理状態
namespace state {

  // 描画するシーン
  enum SceneMode {
    None,
    Exit,       // ゲーム終了
    Save,
    Load,

    Title,
    Strategy,   // 戦略画面
    Battle,
    Result,
  };

  // 各段階の描画状態
  enum SceneState {
    Now,    // 現在の描画シーン
    Last,   // 直前の描画シーン
    Next,   // 次の描画シーンの準備
  };

  // フェードイン、フェードアウトの状態
  enum FadeState {
    FadeOut,
    FadeIn,
    Stop,
  };

  // 戦闘画面の状態
  enum BattlePhase {
    Standby,
    Fight,
    Win,
    Lose,
  };

  // 戦略画面の選択コマンド
  enum StrategyMode {
    Funds,
    Armaments,
    Develop,
    Invasion,
    DataSelect,

    Neutral,   // 何も選択していない状態
    All_Mode,
  };

  // 戦略画面のコマンド描画状態
  enum CommandState {
    MoveIn,
    Select,
    Blink,
    MoveOut,

    // TIPS: Neutral 以外を実行中に、Neutral のボタンを表示しない
    Wait,
  };
}


// キャラクター関係
namespace character {

  // キャラクターのクラス
  // TIPS: 使用する画像にも対応
  enum Type {
    Fighter,
    Scout,
    Knight,
    Mage,
    Priest,

    Soldier,   // 最弱
    Boss,      // 最終ステージのボス
  };

  // 描画する行動
  enum DrawAction {
    Stand,
    Damage,
    Danger,
    Move,
    Avoid,
    Dead,
    Attack,
  };

  // 行動の状態
  enum ActionState {
    Now,
    Next,   // 次のアニメの状態
    Last,   // ダメージを受ける直前の状態
  };

  // キャラクターの性別
  // TIPS: 画像切り取り位置を変更するためだけに使う
  enum Gender {
    Male,
    Female,
  };

  // キャラクターの隊列
  // TIPS: 描画する座標の計算にも使う
  enum Lane {
    Front,
    Back,
  };

  // キャラクターのパラメータ上限
  enum ParameterLimit {
    UNIT_TURN = 10000,
    LEVEL_LIMIT = 255,
    HP_LIMIT = 9999,
    PARAM_LIMIT = 999,
  };

  // マップの特殊効果
  enum MapEffect {
    Grass = 0,
    Forest = 1,
    Mount = 2,

    Plain = 0,

    Front_Adv,
    Back_Adv,

    Fighter_Adv,
    Scout_Adv,
    Knight_Adv,
    Mage_Adv,
    Priest_Adv,
    Soldier_Adv,
    Boss_Adv,

    Male_Adv,
    Female_Adv,

    Trap_Start,
    Poison_Start,
    Unlucky,

    All_Effect,
  };
}


// 音楽ファイル関係
namespace music {

  // BGM
  enum Music {
    Title,
    Strategy,
    DataSelect,
    Result_Win,
    Result_Lose,
    Battle1,
    Battle2,
    Battle3,
    Boss1,
    Boss2,

    All_BGM,
  };

  // SE
  enum SoundEffect {
    Click,
    Click_Menu,
    MouseOn,

    All_SE,
  };
}


// キャラクターのパーソナルデータ
struct sCharacterInfo {
  sCharacterInfo() : side_(true), level_(1) {}

  bool side_;
  std::string name_;
  u_short type_;
  u_short gender_;

  u_short level_;
};


// キャラクターのパラメータ
struct sParameter {
  sParameter() :
    pow_(10), vit_(10), mag_(10), spd_(10), dex_(10), luk_(10) {}

  sParameter(short n) :
    pow_(n), vit_(n), mag_(n), spd_(n), dex_(n), luk_(n) {}

  u_short pow_;    // power: 物理攻撃力に影響
  u_short vit_;    // vitality: 物理防御力、自然回復速度に影響
  u_short mag_;    // magica: 魔法の威力、防御力に影響
  u_short spd_;    // speed: 行動速度、回避に影響
  u_short dex_;    // dexterity: 命中、会心率に影響
  u_short luk_;    // luck: 命中、回避、会心率に影響
};


// キャラクターの体力
struct sHitPoint {
  sHitPoint() : now_(50), max_(50) {}

  short now_;
  u_short max_;
};


/* 定義 */
//////////////////////////////////////////////////////////////////////

typedef unsigned short            u_short;
typedef std::stringstream         sstream;

typedef size::CutSize             cut;
typedef size::FontSize            fontSize;

typedef design::Difficulty        dif;
typedef design::ClearRank         rank;
typedef design::WeaponPrice       price;
typedef design::ColorType         paint;

typedef state::SceneMode          scene;
typedef state::SceneState         play;
typedef state::FadeState          fade;
typedef state::BattlePhase        phase;
typedef state::StrategyMode       mode;
typedef state::CommandState       command;

typedef character::Type           type;
typedef character::DrawAction     action;
typedef character::Gender         gen;
typedef character::MapEffect      effect;

typedef music::Music              bgm;
typedef music::SoundEffect        se;


//////////////////////////////////////////////////////////////////////
