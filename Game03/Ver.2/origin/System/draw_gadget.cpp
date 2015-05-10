
#include "draw_gadget.h"


bool draw::is_init_ = false;
std::array<Color, paint::ColorVariation> draw::color_data;


void draw::colorDataInit() {
  if (is_init_) { return; }

  color_data[paint::White]      = Color(1.00f, 1.00f, 1.00f);
  color_data[paint::LightGray]  = Color(0.50f, 0.50f, 0.50f);
  color_data[paint::DarkGray]   = Color(0.25f, 0.25f, 0.25f);
  color_data[paint::Black]      = Color(0.00f, 0.00f, 0.00f);

  color_data[paint::Red]        = Color(1.00f, 0.00f, 0.00f);
  color_data[paint::Carmine]    = Color(1.00f, 0.25f, 0.25f);
  color_data[paint::Pink]       = Color(1.00f, 0.50f, 0.50f);
  color_data[paint::Purple]     = Color(0.75f, 0.00f, 0.75f);
  color_data[paint::DarkBlue]   = Color(0.25f, 0.00f, 0.75f);
  color_data[paint::Blue]       = Color(0.00f, 0.00f, 0.80f);
  color_data[paint::SkyBlue]    = Color(0.00f, 0.50f, 1.00f);
  color_data[paint::LightGreen] = Color(0.00f, 1.00f, 0.50f);
  color_data[paint::Green]      = Color(0.00f, 1.00f, 0.00f);
  color_data[paint::Yellow]     = Color(1.00f, 1.00f, 0.00f);
  color_data[paint::Orange]     = Color(1.00f, 0.75f, 0.00f);
  color_data[paint::Brown]      = Color(0.75f, 0.50f, 0.00f);

  is_init_ = true;
}

void draw::window(const Vec2f& pos,
                  const Vec2f& size,
                  const Color& color,
                  const float& alpha) {
  const Color frame_color = draw::color(paint::White, alpha);

  // 背景の描画
  // TIPS: フレームも含めて指定されたサイズ内に描画するために位置をずらす
  drawFillBox(pos.x() + WindowFrame, pos.y() + WindowFrame,
              size.x() - WindowFrame * 2, size.y() - WindowFrame * 2,
              color);

  // フレームの描画
  drawBox(pos.x() + WindowFrame / 2, pos.y() + WindowFrame / 2,
          size.x() - WindowFrame, size.y() - WindowFrame,
          WindowFrame, frame_color);

  // TIPS: 四隅が丸くなるように補完する
  {
    // 左上
    drawPoint(pos.x() + WindowFrame / 2,
              pos.y() + size.y() - WindowFrame / 2,
              PointSize, frame_color);
    // 左下
    drawPoint(pos.x() + WindowFrame / 2,
              pos.y() + WindowFrame / 2,
              PointSize, frame_color);
    // 右上
    drawPoint(pos.x() + size.x() - WindowFrame / 2,
              pos.y() + size.y() - WindowFrame / 2,
              PointSize, frame_color);
    // 右下
    drawPoint(pos.x() + size.x() - WindowFrame / 2,
              pos.y() + WindowFrame / 2,
              PointSize, frame_color);
  }
}

void draw::gauge(const float& per,
                 const Vec2f& pos,
                 const Vec2f& size,
                 const Color& color) {
  const Color& frame_color = color_data[paint::White];

  // 背景の描画
  // TIPS: フレームも含めて指定されたサイズ内に描画するために位置をずらす
  drawFillBox(pos.x() + GaugeFrame, pos.y() + GaugeFrame,
              size.x() - GaugeFrame * 2, size.y() - GaugeFrame * 2,
              color_data[paint::Black]);

  // ゲージの描画
  drawFillBox(pos.x() + GaugeFrame, pos.y() + GaugeFrame,
              (size.x() - GaugeFrame * 2) * per, size.y() - GaugeFrame * 2,
              color);

  // フレームの描画
  drawBox(pos.x() + GaugeFrame / 2, pos.y() + GaugeFrame / 2,
          size.x() - GaugeFrame, size.y() - GaugeFrame,
          GaugeFrame, frame_color);

  // TIPS: 四隅が丸くなるように補完する
  {
    // 左上
    drawPoint(pos.x() + GaugeFrame / 2,
              pos.y() + size.y() - GaugeFrame / 2,
              PointSize, frame_color);
    // 左下
    drawPoint(pos.x() + GaugeFrame / 2,
              pos.y() + GaugeFrame / 2,
              PointSize, frame_color);
    // 右上
    drawPoint(pos.x() + size.x() - GaugeFrame / 2,
              pos.y() + size.y() - GaugeFrame / 2,
              PointSize, frame_color);
    // 右下
    drawPoint(pos.x() + size.x() - GaugeFrame / 2,
              pos.y() + GaugeFrame / 2,
              PointSize, frame_color);
  }
}

const Color draw::color(const short& color_num,
                        const float& alpha) {
  Color temp = color_data[color_num];
  temp.alpha() = alpha;
  return temp;
}
