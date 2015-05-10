
#include "scene_manager.h"

typedef cSceneManager  manager;


cSceneManager::cSceneManager() :
init_(false),
now_scene_(data::system.scene[play::Now]),
next_scene_(data::system.scene[play::Next]),
is_pause_(data::system.pause_) {
  anime.mode_ = fade::FadeIn;
  anime.time_ = 0;
  anime.alpha_ = 1.0f;
}


void manager::update() {

  // TIPS: 画面切り替え中なら、一切の入力を許可しない
  if (sceneShift()) { fadeShift(); return; }

  win::mouseTranslate();

  // TIPS: ポーズ画面のとき、その他の入力を許可しない
  if (pause_.update()) { return; }

  // TIPS: 各シーンごとの更新処理
  switch (now_scene_) {
    case scene::Title   : title_.update();    break;
    case scene::Strategy: strategy_.update(); break;
    case scene::Battle  : battle_.update();   break;
    case scene::Result  : result_.update();   break;
    default: select_.update();
  }
}


// 全てのシーン演出を初期化する
void manager::init() {
  data::bugProof();
  if (!init_) { effectInit(); }
}


void manager::effectInit() {
  switch (next_scene_) {
    case scene::Title   : title_.init();    break;
    case scene::Strategy: strategy_.init(); break;
    case scene::Battle  : battle_.init();   break;
    case scene::Result  : result_.init();   break;
    default: select_.init();
  }

  init_ = true;
}


void manager::draw() {
  win::app->setupDraw();

  // TIPS: 各シーンごとの描画処理
  switch (now_scene_) {
    case scene::Title   : title_.draw();    break;
    case scene::Strategy: strategy_.draw(); break;
    case scene::Battle  : battle_.draw();   break;
    case scene::Result  : result_.draw();   break;
    default: select_.draw(); break;
  }

  // TIPS: 画面切り替えが発生したらアニメーション開始
  blackOut();

  // TIPS: ポーズ中ならポーズ画面の描画
  if (is_pause_) { pause_.draw(); }

  win::app->update();
}


// ゲームそのものを終了する
bool manager::exit() {
  return now_scene_ == scene::Exit;
}


// 画面切り替えを検出
bool manager::sceneShift() {
  // TIPS: 次のシーンが指定されたら画面切り替え開始
  //     : 切り替え処理が終わるまでは true を返し続ける
  if (now_scene_ != next_scene_) { anime.mode_ = fade::FadeOut; }
  return anime.mode_ != fade::Stop;
}


// 画面の切り替え演出
// TIPS: 黒い四角で塗りつぶして見えないようにする
void manager::blackOut() {
  drawFillBox(-size::WIDTH / 2, -size::HEIGHT / 2,
    size::WIDTH, size::HEIGHT,
    Color(0, 0, 0, anime.alpha_));
}


// フェードイン、フェードアウトの処理
void manager::fadeShift() {
  switch (anime.mode_) {
    case fade::FadeOut:
      if (!fadeOut()) {
        anime.mode_ = fade::FadeIn;
        now_scene_ = next_scene_;
        init_ = false;
      }
      break;

    case fade::FadeIn:
      if (!fadeIn()) { anime.mode_ = fade::Stop; }
      break;

    default:;
  }
}


// フェードアウト
// TIPS: 画面を見えないようにするので、黒を不透明にする
bool manager::fadeOut() {
  if (anime.alpha_ < 1.0f) { anime.alpha_ += 0.05f; }
  return anime.alpha_ < 1.0f;
}


// フェードイン
// TIPS: 画面を見えるようにするので、黒を透明にしていく
bool manager::fadeIn() {
  if (anime.alpha_ > 0) { anime.alpha_ -= 0.05f; }
  return anime.alpha_ > 0;
}
