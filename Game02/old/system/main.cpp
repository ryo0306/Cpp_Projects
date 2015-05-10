//
// card game's 3
//

#include "../App_Lib/appEnv.hpp"
#include "system.hpp"
#include "game_title.hpp"
#include "game_select.hpp"


/* 関数（プロトタイプ） */
////////////////////////////////////////////////////////////////////////////////
/* 詳細は各.cpp, .hppファイルを確認のこと */

void game_nervous(AppEnv&);   // g_nervous.cpp + nervous.hpp
void game_arrange(AppEnv&);   // g_arrange.cpp + arrange.hpp
void game_highlow(AppEnv&);   // g_highlow.cpp + highlow.hpp


/* ゲーム本体 */
////////////////////////////////////////////////////////////////////////////////

int main() {
  AppEnv app_env(Window::WIDTH, Window::HEIGHT, false, true);

  // 背景色の設定
  app_env.bgColor(Color(0, 0, 0.75));

  // 乱数の設定
  Random r; r.setSeed((u_int)time(nullptr));

  // BGM の読み込み
  Media thema1("res/wav/title_thema1.wav");
  Media thema2("res/wav/title_thema2.wav");
  thema1.gain(0.5); thema1.looping(true);
  thema2.gain(0.5); thema2.looping(true);

  // SE の読み込み
  Media se(SE_CLICK); se.gain(0.5);

  // 乱数を使ってBGMを再生
  if (r.fromFirstToLast(0, 1)) { thema1.play(); }
  else { thema2.play(); }

  // タイトル
  game_title(app_env);
  se.play();

  // この処理内だけで使う変数
  enum Game {
    NERVOUS,
    ARRANGE,
    HIGHLOW
  };

  short game_mode;   // ゲームモード

  // 本編ループ
  while (1) {
    if (!app_env.isOpen()) { return 0; }

    // ゲームの選択
    game_mode = game_select(app_env);
    se.play();
    if (thema1.isPlaying()) { thema1.stop(); }
    if (thema2.isPlaying()) { thema2.stop(); }

    // ゲームの呼び出し
    switch (game_mode) {

        // 神経衰弱
      case NERVOUS:
        game_nervous(app_env);
        break;

        // 七並べ
      case ARRANGE:
        game_arrange(app_env);
        break;

        // High & Low
      case HIGHLOW:
        game_highlow(app_env);
        break;

        // 終了ボタンを押した
      default:
        return 0;
    }

    // ゲームが終了して戻ってきたら、乱数を再取得してBGMを再生
    if (r.fromFirstToLast(0, 1)) { thema1.play(); }
    else { thema2.play(); }

  } // 本編ループ 終了

  return 0;
}
