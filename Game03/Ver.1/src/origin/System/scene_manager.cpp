
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

  // TIPS: ��ʐ؂�ւ����Ȃ�A��؂̓��͂������Ȃ�
  if (sceneShift()) { fadeShift(); return; }

  win::mouseTranslate();

  // TIPS: �|�[�Y��ʂ̂Ƃ��A���̑��̓��͂������Ȃ�
  if (pause_.update()) { return; }

  // TIPS: �e�V�[�����Ƃ̍X�V����
  switch (now_scene_) {
    case scene::Title   : title_.update();    break;
    case scene::Strategy: strategy_.update(); break;
    case scene::Battle  : battle_.update();   break;
    case scene::Result  : result_.update();   break;
    default: select_.update();
  }
}


// �S�ẴV�[�����o������������
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

  // TIPS: �e�V�[�����Ƃ̕`�揈��
  switch (now_scene_) {
    case scene::Title   : title_.draw();    break;
    case scene::Strategy: strategy_.draw(); break;
    case scene::Battle  : battle_.draw();   break;
    case scene::Result  : result_.draw();   break;
    default: select_.draw(); break;
  }

  // TIPS: ��ʐ؂�ւ�������������A�j���[�V�����J�n
  blackOut();

  // TIPS: �|�[�Y���Ȃ�|�[�Y��ʂ̕`��
  if (is_pause_) { pause_.draw(); }

  win::app->update();
}


// �Q�[�����̂��̂��I������
bool manager::exit() {
  return now_scene_ == scene::Exit;
}


// ��ʐ؂�ւ������o
bool manager::sceneShift() {
  // TIPS: ���̃V�[�����w�肳�ꂽ���ʐ؂�ւ��J�n
  //     : �؂�ւ��������I���܂ł� true ��Ԃ�������
  if (now_scene_ != next_scene_) { anime.mode_ = fade::FadeOut; }
  return anime.mode_ != fade::Stop;
}


// ��ʂ̐؂�ւ����o
// TIPS: �����l�p�œh��Ԃ��Č����Ȃ��悤�ɂ���
void manager::blackOut() {
  drawFillBox(-size::WIDTH / 2, -size::HEIGHT / 2,
    size::WIDTH, size::HEIGHT,
    Color(0, 0, 0, anime.alpha_));
}


// �t�F�[�h�C���A�t�F�[�h�A�E�g�̏���
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


// �t�F�[�h�A�E�g
// TIPS: ��ʂ������Ȃ��悤�ɂ���̂ŁA����s�����ɂ���
bool manager::fadeOut() {
  if (anime.alpha_ < 1.0f) { anime.alpha_ += 0.05f; }
  return anime.alpha_ < 1.0f;
}


// �t�F�[�h�C��
// TIPS: ��ʂ�������悤�ɂ���̂ŁA���𓧖��ɂ��Ă���
bool manager::fadeIn() {
  if (anime.alpha_ > 0) { anime.alpha_ -= 0.05f; }
  return anime.alpha_ > 0;
}
