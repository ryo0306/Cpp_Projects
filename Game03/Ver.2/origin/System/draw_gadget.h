
#pragma once
#include "environment.h"
#include <array>


namespace literal_color {

  // TIPS: 描画時に指定できる色の一覧
  enum PaintColor {
    White,
    LightGray,
    DarkGray,
    Black,

    Red,
    Carmine,
    Pink,
    Purple,
    DarkBlue,
    Blue,
    SkyBlue,
    LightGreen,
    Green,
    Yellow,
    Orange,
    Brown,

    // TIPS: 配列サイズ指定用データ
    ColorVariation,
    // DANGER: ここより下にデータを作らないこと
  };
}

typedef literal_color::PaintColor  paint;


class cDrawGadget {
  // TIPS: このクラスは宣言しない
  cDrawGadget() = delete;

  // TIPS: ここでしか使わない定数の一覧
  static enum {
    WindowFrame = 4,
    GaugeFrame = 2,
    PointSize = 4,
  };

  static bool is_init_;
  static std::array<Color, paint::ColorVariation> color_data;

public:
  // 色情報の初期化
  // TIPS: ゲーム起動時に１回だけ呼ぶ
  static void colorDataInit();

  // ウィンドウの描画
  static void window(const Vec2f& pos,
                     const Vec2f& size,
                     const Color& color,
                     const float& alpha = 1.0f);

  // ゲージの描画
  static void gauge(const float& per,
                    const Vec2f& pos,
                    const Vec2f& size,
                    const Color& color);

  // 指定された色データを出力
  // TIPS: 透明度を指定しなければ不透明なデータを返す
  static const Color color(const short& color_num,
                           const float& alpha = 1.0f);
};

typedef cDrawGadget  draw;
