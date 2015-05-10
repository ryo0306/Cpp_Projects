//
// 本編ゲームモード選択画面の処理
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "system.hpp"


// ゲームモードの選択
int game_select(AppEnv& app_env) {
  // 画像読み込み
  Texture title("res/png/sys01_logo.png");      // タイトルロゴ
  Texture button("res/png/sys03_button.png");   // ゲームモード
  Texture BG("res/png/bg01_gradation.png");     // 背景
  Texture smoke1("res/png/bg02_smoke1.png");    // 背景：エフェクト１
  Texture smoke2("res/png/bg02_smoke2.png");    // 背景：エフェクト２

  // この処理内だけで使う変数
  const short Game_Mode = 4;   // ゲームモードの数
  const short Mode_W = 192;    // ボタン画像：幅
  const short Mode_H = 48;     // ボタン画像：高さ

  short i;                // 繰り返し処理用
  float x, y;             // ボタン画像の表示位置
  float angle1, angle2;   // エフェクト用
  Color button_color;     // ボタンカラーの変更
  Color effect_color;     // エフェクト用カラー設定
  Vec2f mouse_pos;        // マウスカーソル座標

  effect_color = Color(1, 1, 1, 0.5);

  while (1) {
    if (!app_env.isOpen()) { return 0; }

    // マウス座標の取得
    mouse_pos = app_env.mousePosition();

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Bg_W, Bg_H,
      BG, Color(0, 0, 0, 0.5));

    // 背景エフェクト表示
    {
      angle1 += 1; if (angle1 >= Bg_W) { angle1 = 0.0; }
      angle2 += 2; if (angle2 >= Bg_W) { angle2 = 0.0; }

      // エフェクト１
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        angle1, 0, Bg_W, Bg_H,
        smoke1, effect_color);
      // エフェクト２
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        angle2, 0, Bg_W, Bg_H,
        smoke2, effect_color);
    }

    // タイトルロゴ表示
    drawTextureBox((-Logo_W * 1), (Logo_Ha * 1), Logo_W * 2, Logo_Ha,
      0, 0, Logo_W, Logo_Ha,
      title, Color(1, 0.75, 0.5));

    // 選択肢の処理
    for (i = 0; i < Game_Mode; i += 1) {
      x = (Mode_W * 2) * (i % 2) - (Mode_W * 1.75);
      y = (-Mode_H * 2) * (i / 2);

      // ボタンの上にマウスカーソルがあればカラー変更
      if (CO_on_mouse(mouse_pos, x, y, Mode_W * 1.5, Mode_H * 1.5)) {
        button_color = Color(0.6, 0.8, 1);

        // 左クリックでループ終了
        if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); return i; }
      }
      else { button_color = Color(1, 1, 1); }

      // ボタンの表示
      drawTextureBox(x, y, Mode_W * 1.5, Mode_H * 1.5,
        0, Mode_H * i, Mode_W, Mode_H,
        button, button_color);
    }

    app_env.update();
  }

  return 0;
}
