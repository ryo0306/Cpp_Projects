
#include "scene_manager.h"

typedef cSceneManager  manager;


cSceneManager::cSceneManager() :
is_pause_(data::system.pause_),
now_scene_(data::system.scene_[play::Now]),
next_scene_(data::system.scene_[play::Next]) {

  anime.state_ = fade::FadeOut;
  anime.alpha_ = 1.0f;

  is_init_ = false;
}


void manager::update() {

  // TIPS: 画面切り替え中のとき、一切の入力を許可しない
  if (isPlayingAnime()) { fadeShift(); return; }

  env::mousePosUpdate();

  // TIPS: ポーズ中のとき、その他の入力を許可しない
  if (pause_.update()) { return; }

  switch (now_scene_) {
    case scene::Title   : title_.update();    break;
    case scene::Strategy: strategy_.update(); break;
    case scene::Battle  : battle_.update();   break;
    case scene::Result  : result_.update();   break;
    default: save_load_.update();
  }

  // TIPS: 画面切り替えを検出
  sceneShift();
}


void manager::sceneShift() {

  // TIPS: 次のシーンを指定されたら切り替え演出を開始
  if (now_scene_ != next_scene_) { anime.state_ = fade::FadeOut; }
}


bool manager::isPlayingAnime() {
  return anime.state_ != fade::Stop;
}


void manager::fadeShift() {
  switch (anime.state_) {
    case fade::FadeOut: fadeOut(); break;
    case fade::FadeIn : fadeIn();  break;
    default:;
  }
}


void manager::fadeOut() {

  // TIPS: 画面が完全に暗くなるまでシーンの切り替えなどを許可しない
  anime.alpha_ += 0.05f;
  if (anime.alpha_ < 1.0f) { return; }

  now_scene_ = next_scene_;
  anime.state_ = fade::FadeIn;
  is_init_ = false;
}


void manager::fadeIn() {

  // TIPS: 画面が完全に明るくなるまで、演出を続ける
  anime.alpha_ -= 0.05f;
  if (anime.alpha_ > 0) { return; }

  anime.state_ = fade::Stop;
}


void manager::init() {
  // TIPS: 初期化済みなら処理をスキップ
  if (is_init_) { return; }

  switch (next_scene_) {
    case scene::Title   : title_.init();    break;
    case scene::Strategy: strategy_.init(); break;
    case scene::Battle  : battle_.init();   break;
    case scene::Result  : result_.init();   break;
    default: save_load_.init();
  }

  is_init_ = true;
}


void manager::draw() {
  env::setupDraw();

  switch (now_scene_) {
    case scene::Title   : title_.draw();    break;
    case scene::Strategy: strategy_.draw(); break;
    case scene::Battle  : battle_.draw();   break;
    case scene::Result  : result_.draw();   break;
    default: save_load_.draw();
  }

  // TIPS: 画面の切り替え演出
  blackOut();

  // TIPS: ポーズ中ならポーズ画面を表示
  if (is_pause_) { pause_.draw(); }

  env::update();
}


void manager::blackOut() {

  // TIPS: 画面全体を黒く塗りつぶして見えないようにする
  drawFillBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
              Color(0, 0, 0, anime.alpha_));
}


bool manager::isContinue() {
  // TIPS: ウィンドウを閉じるか、ゲーム終了するまで true を返す
  return env::isOpen() && now_scene_ != scene::Exit;
}
