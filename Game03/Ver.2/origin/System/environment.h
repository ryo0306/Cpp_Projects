
#pragma once
#include "base.hpp"
#include <memory>


class cEnvironment {
  // TIPS: このクラスは宣言しない
  cEnvironment() = delete;

  static Vec2f mouse_;
  static std::unique_ptr<AppEnv> app_;

public:
  // アプリの起動
  // NOTICE: 必ず main() の最初に実行する
  static void execApplication();

  static void setupDraw();
  static void update();

  static void mousePosUpdate();

  // TIPS: カーソルが指定された矩形の範囲内にあれば true を返す
  static bool rectOnMouse(const Vec2f& pos,
                          const Vec2f& size);

  // TIPS: 左クリックした瞬間に true を返す
  static bool leftClick();

  // TIPS: 右クリックした瞬間に true を返す
  static bool rightClick();

  // TIPS: ゲームウィンドウが開いていれば true を返す
  static bool isOpen();
};

typedef cEnvironment  env;
