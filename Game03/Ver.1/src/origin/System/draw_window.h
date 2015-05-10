
#pragma once
#include "../common.h"
#include <array>
#include <memory>

//
// ウィンドウ描画クラス
//

class cDrawWindow {
  cDrawWindow() = delete;

  static std::array<Color, paint::ColorVariation> color_type_;
  static bool isCreate_;

public:
  static std::unique_ptr<AppEnv> app;

  static Vec2f mouse;

  static void create();
  static void mouseTranslate();

  static void draw(const Vec2f&, const Vec2f&, const Color&, const float& alpha = 1.f);
  static void gaugeDraw(const Vec2f&, const Vec2f&, const float&, const Color&);

  static Color color(const short&, const float& alpha = 1.f);
};

typedef cDrawWindow  win;
