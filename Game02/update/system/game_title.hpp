//
// タイトル画面の処理
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "system.hpp"


// タイトル画面
void game_title(AppEnv& app_env) {
  // 画像読み込み
  Texture title("res/png/sys01_logo.png");    // タイトルロゴ
  Texture start("res/png/sys02_start.png");   // スタートボタン
  Texture BG("res/png/bg01_gradation.png");   // 背景

  // この処理内だけで使う変数
  const short One_Second = 60;   // 1秒 = 60.frame
  const short Logo_Line = 8;     // 描画サイズ

  const short Draw_Limit = Logo_Ha - Logo_Line;   // 描画アニメの終了判定

  short time_count = 0;   // タイマー
  short blink = 0;        // 点滅アニメーション
  float draw_y = 0;       // ロゴ変化アニメ：描画位置

  while (1) {
    if (!app_env.isOpen()) { return; }

    // 左クリックでループ終了
    if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); return; }

    // 画像の点滅処理
    {
      blink = (time_count / 30) % 2;
      time_count++;
      if (time_count == One_Second) { time_count = 0; }   // 1秒カウントしたらリセット
    }

    // タイトルロゴに変化をつける処理
    {
      draw_y += 1.5;
      if (draw_y >= Draw_Limit) { draw_y = 0; }   // 描画位置をリセット
    }

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Bg_W, Bg_H,
      BG, Color(0, 0, 0, 0.5));

    // タイトルロゴ表示
    drawTextureBox((-Logo_W * 1), (Logo_Ha * 1), Logo_W * 2, Logo_Ha,
      0, 0, Logo_W, Logo_Ha,
      title, Color(1, 0.75, 0.5));
    drawTextureBox((-Logo_W * 1), (Logo_Ha * 1) + draw_y, Logo_W * 2, Logo_Line,
      0, (Logo_Ha - Logo_Line) - draw_y, Logo_W, Logo_Line,
      title, Color(1, 1, 1));

    // クリックスタート
    drawTextureBox((-Logo_W * 0.5), (-Logo_Hb * 2), Logo_W, Logo_Hb,
      0, 0, Logo_W, Logo_Hb,
      start, Color(1, 1, 0.5, blink));

    app_env.update();
  }
}
