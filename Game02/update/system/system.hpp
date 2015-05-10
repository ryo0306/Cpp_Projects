//
// ゲーム全体で使う変数、定数リスト
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../App_Lib/random.hpp"
#include <ctime>


/* BGM,SE の設定用マクロ */
////////////////////////////////////////////////////////////////////////////////

#define  M_THEMA      "res/wav/main_thema.wav"         // メインテーマのループ部分
#define  M_INTRO      "res/wav/main_thema_intro.wav"   // メインテーマのイントロ
#define  SE_CLICK     "res/wav/se_click.wav"           // クリックSE
#define  SE_SUCCESS   "res/wav/se_success.wav"         // 成功SE
#define  SE_FAILURE   "res/wav/se_failure.wav"         // 失敗SE
#define  SE_FINISH    "res/wav/se_gameover.wav"        // ゲーム終了SE


/* ゲーム全体で使う定数リスト */
////////////////////////////////////////////////////////////////////////////////

// ウィンドウサイズ
enum Window {
  WIDTH = 1200,
  HEIGHT = 800
};

// *画像の切り出しサイズ*
// W = Width, H = Height
enum Size {
  Bg_W = 512,      // 背景画像：幅
  Bg_H = 512,      // 背景画像：高さ

  Card_W = 72,     // カード画像：幅
  Card_H = 96,     // カード画像：高さ

  Logo_W = 512,    // タイトルロゴ：幅
  Logo_Ha = 128,   // タイトルロゴ：高さ（128）
  Logo_Hb = 64,    // タイトルロゴ：高さ（64）

  Icon_W = 128,    // アイコン：幅
  Number_W = 16,   // 数字：幅
  Telop_W = 512,   // 文字列：幅
  Telop_H = 32     // 文字列：高さ
};

// ゲームのシステム情報
enum System {
  PLAYER = 4,      // プレイヤーを含めた人数（複数参加のゲーム）
  RIVAL = 3,       // 対戦相手の人数（複数参加のゲーム）

  JOKER_OK = 53,   // ジョーカー有りのカード総数（本来は 2枚あるが、ゲームでは 1枚だけ使う）
  JOKER_NG = 52,   // ジョーカー無しのカード総数

  ALL_CARD = 14,   // ジョーカー有りルールの手札の最大枚数
  CARD_NUM = 13,   // カードの数字（Ａ～Ｋまで：0 ～ 12）
  CARD_MARK = 4,   // カードの記号（ジョーカーを含まない）

  MARK_SPD = 0,    // スペード
  MARK_HRT = 1,    // ハート
  MARK_CLB = 2,    // クラブ
  MARK_DIA = 3,    // ダイヤ

  RETIRE_P = 2,    // リタイアになるパスの回数
  RETIRE_G = 5,    // ゲームをリタイアしたときの順位

  OPEN_ACT = 2,    // 表にできる回数
  MAX_HIST = 8     // 表にしたカードの最大記憶枚数
};


////////////////////////////////////////////////////////////////////////////////
/* g_common.cpp */

bool CO_on_mouse(Vec2f, float, float, float, float);
void CO_disp_number(Texture&, short, float, float);


////////////////////////////////////////////////////////////////////////////////
