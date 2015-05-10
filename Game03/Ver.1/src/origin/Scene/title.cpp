
#include "title.h"

typedef cTitle  title;


cTitle::cTitle() :
bg_(Texture("res/png/world_map.png")),
sea_(Texture("res/png/world_sea.png")),
fog_(Texture("res/png/world_sea_fog.png")),
logo_(Texture("res/png/back_title_logo.png")),
telop_(FONT_),
size_(Vec2f(size::Large_W, size::Large_H)),
clear_(data::system.clear_),
next_scene_(data::system.scene[play::Next]),
last_scene_(data::system.scene[play::Last]) {

  telop_.size(fontSize::Large);

  text[Click]    = "Click Start";
  text[Back]     = "RightClick Back";
  text[NewGame]  = "New Game";
  text[Continue] = "Continue";
  text[Easy]     = "�d�`�r�x ";
  text[Normal]   = "�m�n�q�l�`�k ";
  text[Hard]     = "�g�`�q�c ";
}


void title::update() {
  sceneChange();
  if (clickStart()) { return; }
  modeSelect();
}


bool title::clickStart() {

  // TIPS: �N���b�N���ꂽ�獂���œ_�ŁA�Q�[�����J�n����
  switch (draw_state_) {
    case Start:
      if (win::app->isPushButton(Mouse::LEFT)) {
        ++draw_state_;
        data::music.sePlay(se::Click);
      }
      break;

    case Blink1:;
    case Blink2:;
    case Blink3:
      ++blink_timer_;
      if (blink_timer_ > design::Fps) {
        ++draw_state_;
        blink_timer_ = 0;
      }
      break;

    case Change:
      anime.alpha_ -= 0.05f;
      if (anime.alpha_ <= 0.f) {
        ++draw_state_;
        click_mode_ = -1;
      }
      break;

    case Cancel:
      anime.alpha_ -= 0.05f;
      if (anime.alpha_ <= 0.f) { draw_state_ = Mode; }
      break;

    default:;
  }

  return (draw_state_ != Mode && draw_state_ != Select);
}


void title::modeSelect() {
  buttonDataTranslate();

  for (i = begin_; i < end_; ++i) {
    buttonPosTranslate(i);
    on_mouse_ = rectOnMouse(pos_, size_);

    if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click);
      ++draw_state_;
      click_mode_ = i;
    }
  }

  if (draw_state_ == Select &&
    win::app->isPushButton(Mouse::RIGHT)) { draw_state_ = Cancel; }
}


void title::sceneChange() {
  if (draw_state_ > Blink3) {
    clear_ ? data::reStart(dif(click_mode_ - Easy)) :
      data::newGame(dif(click_mode_ - Easy));
    next_scene_ = scene::Strategy;
  }

  if (click_mode_ == Continue && draw_state_ > Blink2) {
    last_scene_ = scene::Title;
    next_scene_ = scene::Load;
  }
}


// ���o�̏�����
// TIPS: ���[�v�̊J�n���Ɉ�x�������������s��
void title::init() {
  anime.time_ = 0;
  anime.blink_ = 0;
  anime.scroll_ = 0.f;
  anime.alpha_ = 0.f;

  draw_state_ = Start;
  blink_timer_ = 0;
  click_mode_ = -1;

  data::music.stop();
  data::music.play(bgm::Title);
}


void title::draw() {
  effectPlay();
  disp_title();
  disp_click();
  disp_mode();
}


// TIPS: ��ʔw�i�̉��o����
void title::effectPlay() {
  ++anime.time_;
  if (anime.time_ >= design::Fps - 10) { anime.time_ = 0; }
  anime.blink_ = anime.time_ / (design::Fps / 6);

  anime.scroll_ += 0.25f;
  if (anime.scroll_ >= size::Rect_512) { anime.scroll_ = 0.f; }
}


void title::disp_title() {

  // TIPS: �F�������C�ɓ���Ȃ��̂ŁA�Ŕw�ʂɕʂ̐F����������`��
  drawFillBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    win::color(paint::White));

  // �C
  drawTextureBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    anime.scroll_ * 2, 0, size::Rect_512, size::Rect_512,
    sea_, win::color(paint::White, 0.5f));

  // �g
  drawTextureBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    anime.scroll_, 0, size::Rect_512, size::Rect_512,
    fog_, win::color(paint::SkyBlue, 0.5f));

  // �嗤
  drawTextureBox(0, 0, size::WIDTH, size::HEIGHT,
    0, 0, size::Rect_1024, size::Rect_1024,
    bg_, win::color(paint::Black),
    0, Vec2f(1.01f, 1.01f), Vec2f(size::Half_W, size::Half_H));
  drawTextureBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    0, 0, size::Rect_1024, size::Rect_1024,
    bg_, win::color(paint::Orange));

  // ���S
  drawTextureBox(0, size::LogoPos_Y, size::Logo_W, size::Logo_H,
    0, size::Logo_H * anime.blink_, size::Logo_W, size::Logo_H,
    logo_, win::color(paint::White),
    0, Vec2f(3.f, 3.f), Vec2f(size::Logo_W / 2, 0));
}


// �N���b�N����̕\��
void title::disp_click() {
  // TIPS: ��Փx�����肵����\�����Ȃ�
  if (draw_state_ > Select) { return; }

  pos_.x() =
    -telop_.drawSize(text[draw_state_ == Select ? Back : Click]).x() / 2;
  pos_.y() = 0;

  telop_.draw(text[draw_state_ == Select ? Back : Click], pos_,
    win::color(draw_state_ > Change ? paint::Green : paint::Black,
    draw_state_ == Blink1 ? (blink_timer_ / 2) % 2 : anime.time_ / (design::Fps - 35)));
}


// �R�}���h�̕`��
void title::disp_mode() {
  // TIPS: �P��N���b�N�����܂Ń{�^����\�����Ȃ�
  if (draw_state_ < Mode) { return; }

  if (anime.alpha_ < 0.75f && draw_state_ != Change) { anime.alpha_ += 0.05f; }

  buttonDataTranslate();

  for (i = begin_; i < end_; ++i) {
    buttonPosTranslate(i);
    on_mouse_ = rectOnMouse(pos_, size_);

    // TIPS: �G�t�F�N�g���s���̓{�^���̐F��ς��Ȃ�
    if (draw_state_ != Mode && draw_state_ != Select) { on_mouse_ = false; }

    // TIPS: �����p���j���[�Q�[���\�Ȃ�A�{�^���J���[��ύX
    take_over_ = (i == NewGame && clear_);

    // �{�^���̕`��
    win::draw(pos_, size_,
      win::color(on_mouse_ ? paint::Orange :
      take_over_ ? paint::Purple : paint::Blue,
      click_mode_ == i ? (blink_timer_ / 2) % 2 : anime.alpha_));

    // �e���b�v�̕\��
    pos_.x() = -telop_.drawSize(text[i]).x() / 2;
    pos_.y() += 15;
    telop_.draw(text[i], pos_,
      win::color(on_mouse_ ? paint::Black : paint::White,
      click_mode_ == i ? (blink_timer_ / 2) % 2 : 1.f));
  }
}


void title::buttonPosTranslate(const short& i) {
  short pos_y = (draw_state_ < Select) ? NewGame : Easy;

  pos_.x() = -size_.x() / 2;
  pos_.y() = (size::ModePos_Y) * (1.5f + i - pos_y);
}


void title::buttonDataTranslate() {
  begin_ = (draw_state_ < Select) ? NewGame : Easy;
  end_ = (draw_state_ < Select) ? Easy : All_Text;
}
