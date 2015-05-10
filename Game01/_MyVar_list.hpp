//
// Revival of Kingdom の変数リスト
//

#pragma once
#include "appEnv.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////

// ウィンドウサイズ
enum Window {
  WIDTH = 512,
  HEIGHT = 512
};

// 各画像の切り出しサイズ
// キャラ：chara (W, H)
// 　文字：telop (W, H)
// 　数字：number (W)
// 　情報：status (W)
// ボタン：menu (W, H)
// 　資産：fund (W)
// 戦闘用：icon (W, H)
// W = width, H = height
enum Size {
  chara_W = 20, chara_H = 32,
  telop_W = 472, telop_H = 20,
  number_W = 14,
  status_W = 56,
  menu_W = 112, menu_H = 32,
  fund_W = 240,
  icon_W = 16, icon_H = 16
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  目次
  ・パラメータ関係
  ・フラグ関係
  ・ＵＩ関係
  ・戦闘関係
 */

// 省略記号について
// P_：プレイヤー
// E_：相手

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  パラメータ関係
  struct status {};
    status P, E;
  short P_pop, E_pop;
  const unsigned short fundinfo[], iteminfo[];
  short P_fund[], P_item[];
  short val[];
 */

struct status {
  int money;
  int fund;
  short territory;
  int soldier;
  int mercenary;
  short fame;
  short infamy;
};

// プレイヤー：P と 相手：E のステータス情報
// 所持金：money
// 　資産：fund
// 　領地：territory
// 　兵士：soldier
// 義勇兵：mercenary
// 　名声：fame
// 　悪名：infamy
status P = {0}, E = {0};

// 各陣営の人望値：pop = popularity
// 人望 = 名声 - 悪名
short P_pop, E_pop;

// 買収資産の価格情報
const unsigned short fundinfo[14] = {
  2000,     // 王子の城
  500,      // 農地
  1000,     // 牧場
  1000,     // 農地＋１
  2000,     // 牧場＋１
  5000,     // 織物工場
  10000,    // 鉄の鉱山
  20000,    // 銀の鉱山
  30000,    // 金の鉱山
  50000,    // 美術館
  20000,    // 良質な鉄鉱山
  15000,    // 武器の鍛冶屋
  15000,    // 防具の鍛冶屋
  65535,    // 店
};

// ドロップ資産の価格情報
const unsigned short iteminfo[7] = {
  1000,     // 小さな装飾品
  1500,     // 勇ましい武具
  3000,     // 高級ワイン
  10000,    // 立派なかんむり
  5000,     // ゴテゴテした服
  10000,    // 英雄の武具
  20000,    // ヒミツの日記
};

// 資産の所有情報
// fund：買収資産（売却不可）
// item：ドロップ資産（売却可）
short P_fund[14] = {0}, P_item[7] = {1, 0};

// パラメータの桁数を保存
short val[6] = {0};


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  フラグ関係
  short stage_flag;
  bool stage_clear;
  short ex_flag;
  short spy;
  short sabo;
  bool rumor;
  bool disclos;
  bool rebel;
  short button_flag;
  bool add_attack;
 */

// 現在のエピソード（章）情報
short stage_flag = 0;

// ステージのクリアフラグ
bool stage_clear = false;

// エクストラステージ突入フラグ： 100 を超えたら不可
short ex_flag = 0;

// スパイ実行中か： 10 回行動したら終了
short spy = 0;

// 妨害工作実行中か：カウントが 0になったら終了。連続実行でカウント増加（+3）
short sabo = 0;

// 風説の流布を実行したか：ステージクリアでフラグリセット
bool rumor = false;

// 悪事が発覚したか
bool disclos = false;

// 反乱の成否判定
bool rebel = false;

// どのボタンが押されたか
short button_flag;

// 追撃戦突入
bool add_attack = false;

// 相手の行動カウンタ
short E_time = 0;

// 相手行動開始までのタイムバー表示用
float E_timeGauge = 0;

// 相手が行動開始した
bool E_action = false;

// タイムバーの色
Color timeColor;


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  ＵＩ関係
  Vec2f mouse_pos;
 */

// マウスカーソルの座標
Vec2f mouse_pos;

// パラメータ数値のスキップ表示
short skip = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  戦闘関係
  int P_pow, P_attack, E_pow, E_attack;
  int P_pow0, E_pow0;
  int P_start, P_end, E_start, E_end;
  int P_damage, E_damage;

  float P_powGauge, E_powGauge;
  Color P_GaugeColor, E_GaugeColor;
  float P_Mgauge, E_Mgauge;
  float P_M0, E_M0;

  short P_lastcmd;
  short E_command;
 */

// 戦闘用データ
// pow = 兵力、attack = 攻撃力
int P_pow, P_attack, E_pow, E_attack;

// 戦闘開始時のデータ保存
int P_pow0, E_pow0;

// ターン開始時と終了時の兵力保存
int P_start, P_end, E_start, E_end;

// 戦闘中のダメージ計算
int P_damage, E_damage;

// 兵力ゲージ表示用
float P_powGauge, E_powGauge;

// 兵力ゲージの色
Color P_GaugeColor, E_GaugeColor;

// 義勇兵ゲージ表示用
float P_Mgauge, E_Mgauge;

// 戦闘開始時の義勇兵人数の保存
float P_M0, E_M0;

// プレイヤーの直前の行動
short P_lastcmd;

// 相手の選択した行動
short E_command;


////////////////////////////////////////////////////////////////////////////////////////////////////

