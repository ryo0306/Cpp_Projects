
#include "draw_window.h"


// カラー情報
std::array<Color, paint::ColorVariation> win::color_type_;


// ウィンドウ生成フラグ
// TIPS: 一度生成したら以降の命令を無視する
bool win::isCreate_ = false;


// ゲーム実行環境のインスタンス
// 描画準備、更新、入力など
std::unique_ptr<AppEnv> win::app;


// マウスカーソル位置（ｘ、ｙ）
Vec2f win::mouse;


// ウィンドウ生成命令
// TIPS: 必ず main() の一番最初に呼び出すようにする
void win::create() {
  if (isCreate_) { return; }

  app = std::make_unique<AppEnv>(size::WIDTH, size::HEIGHT, false, true);

  color_type_[paint::White]   = Color(1.f, 1.f, 1.f);
  color_type_[paint::Gray]    = Color(0.25f, 0.25f, 0.25f);
  color_type_[paint::Black]   = Color(0.f, 0.f, 0.f);

  color_type_[paint::Red]     = Color(1.f, 0.2f, 0.2f);
  color_type_[paint::Pink]    = Color(1.f, 0.5f, 0.5f);
  color_type_[paint::Purple]  = Color(0.8f, 0.f, 0.8f);
  color_type_[paint::Blue]    = Color(0.f, 0.f, 0.8f);
  color_type_[paint::SkyBlue] = Color(0.f, 0.5f, 1.f);
  color_type_[paint::Green]   = Color(0.f, 1.f, 0.f);
  color_type_[paint::Yellow]  = Color(1.f, 1.f, 0.f);
  color_type_[paint::Orange]  = Color(1.f, 0.75f, 0.f);
  color_type_[paint::Brown]   = Color(0.75f, 0.5f, 0.f);

  isCreate_ = true;
}


// マウス座標の取得
void win::mouseTranslate() {
  mouse = app->mousePosition();
}


// ゲーム内ウィンドウの描画
// TIPS: フレームの透明度も変更する
void win::draw(const Vec2f& pos, const Vec2f& size, const Color& col, const float& alpha) {

  // TIPS: 渡されたサイズ情報が小さすぎる場合、描画しない
  if (size.x() < 10.f || size.y() < 10.f) { return; }

  // ウィンドウの背景描画
  drawFillBox(pos.x() + 3, pos.y() + 3, size.x() - 6, size.y() - 6, col);

  // TIPS: フレームカラーの一括変更
  Color frame_color = color_type_[paint::White];
  frame_color.alpha() = alpha;

  // ウィンドウフレームの描画
  {
    drawBox(pos.x() + 2, pos.y() + 2, size.x() - 4, size.y() - 4, 4, frame_color);

    // TIPS: フレームの四隅を補完
    drawFillCircle(pos.x() + 2, pos.y() + 2,
      2, 2, 4, frame_color);
    drawFillCircle(pos.x() + 2, pos.y() + size.y() - 2,
      2, 2, 4, frame_color);
    drawFillCircle(pos.x() + size.x() - 2, pos.y() + 2,
      2, 2, 4, frame_color);
    drawFillCircle(pos.x() + size.x() - 2, pos.y() + size.y() - 2,
      2, 2, 4, frame_color);
  }
}


void win::gaugeDraw(const Vec2f& pos, const Vec2f& size, const float& per, const Color& col) {
  // ゲージの描画
  {
    // 背景
    drawFillBox(pos.x() + 2, pos.y() + 2,
      size.x() - 4, size.y() - 4, color(paint::Black));

    // 実際のゲージ
    drawFillBox(pos.x() + 2, pos.y() + 2,
      (size.x() - 4) * per, size.y() - 4, col);
  }

  // フレームの描画
  {
    Color& frame_color = color_type_[paint::White];

    drawBox(pos.x() + 1.5f, pos.y() + 1.5f,
      size.x() - 3, size.y() - 3, 3, frame_color);
  }
}


Color win::color(const short& mode, const float& alpha) {
  Color temp = color_type_[mode];
  temp.alpha() = alpha;
  return temp;
}
