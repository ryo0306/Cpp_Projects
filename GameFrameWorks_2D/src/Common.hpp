
#pragma once
#include "lib/defines.hpp"
#include "lib/appEnv.hpp"


namespace frameworks {

enum WindowSize {
  WIDTH  = 1200,
  HEIGHT = 900,
};

// キーボード入力の一覧
enum KeyBind {
  ESCAPE = GLFW_KEY_ESCAPE,
  ENTER  = GLFW_KEY_ENTER,
  KEY_L = GLFW_KEY_LEFT,
  KEY_R = GLFW_KEY_RIGHT,
  KEY_U = GLFW_KEY_UP,
  KEY_D = GLFW_KEY_DOWN,
  SPACE = GLFW_KEY_SPACE,
};

// 座標、サイズ、回転状態、回転速度
struct Transform {
  Vec2f pos, scale;
  float rotate;
  float angle;
};

// 画像、切り出し開始位置、切り取りサイズ、色
struct Sprite {
  Texture texture;
  Vec2f start, size;
  Color color;
};

// アプリケーション機能の呼び出し
AppEnv& Env();

}  // end frameworks
