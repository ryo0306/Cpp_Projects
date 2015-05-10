
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

  // TIPS: ��ʐ؂�ւ����̂Ƃ��A��؂̓��͂������Ȃ�
  if (isPlayingAnime()) { fadeShift(); return; }

  env::mousePosUpdate();

  // TIPS: �|�[�Y���̂Ƃ��A���̑��̓��͂������Ȃ�
  if (pause_.update()) { return; }

  switch (now_scene_) {
    case scene::Title   : title_.update();    break;
    case scene::Strategy: strategy_.update(); break;
    case scene::Battle  : battle_.update();   break;
    case scene::Result  : result_.update();   break;
    default: save_load_.update();
  }

  // TIPS: ��ʐ؂�ւ������o
  sceneShift();
}


void manager::sceneShift() {

  // TIPS: ���̃V�[�����w�肳�ꂽ��؂�ւ����o���J�n
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

  // TIPS: ��ʂ����S�ɈÂ��Ȃ�܂ŃV�[���̐؂�ւ��Ȃǂ������Ȃ�
  anime.alpha_ += 0.05f;
  if (anime.alpha_ < 1.0f) { return; }

  now_scene_ = next_scene_;
  anime.state_ = fade::FadeIn;
  is_init_ = false;
}


void manager::fadeIn() {

  // TIPS: ��ʂ����S�ɖ��邭�Ȃ�܂ŁA���o�𑱂���
  anime.alpha_ -= 0.05f;
  if (anime.alpha_ > 0) { return; }

  anime.state_ = fade::Stop;
}


void manager::init() {
  // TIPS: �������ς݂Ȃ珈�����X�L�b�v
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

  // TIPS: ��ʂ̐؂�ւ����o
  blackOut();

  // TIPS: �|�[�Y���Ȃ�|�[�Y��ʂ�\��
  if (is_pause_) { pause_.draw(); }

  env::update();
}


void manager::blackOut() {

  // TIPS: ��ʑS�̂������h��Ԃ��Č����Ȃ��悤�ɂ���
  drawFillBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
              Color(0, 0, 0, anime.alpha_));
}


bool manager::isContinue() {
  // TIPS: �E�B���h�E����邩�A�Q�[���I������܂� true ��Ԃ�
  return env::isOpen() && now_scene_ != scene::Exit;
}
