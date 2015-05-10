
#include "develop.h"

typedef cDevelop  dev;


cDevelop::cDevelop() :
telop_(FONT_),
size_(Vec2f(size::Small_W, size::Small_H)),
mode_(data::system.strategy.mode_),
poli_state_(data::system.strategy.draw_state_),
money_(data::user.weapon.money_),
fire_(data::user.weapon.fire_),
cannon_(data::user.weapon.cannon_),
trap_(data::user.weapon.trap_),
poison_(data::user.weapon.poison_),
pause_(data::system.pause_) {

  telop_.size(fontSize::Small);

  text[Fire]   = "�Ή��r ";
  text[Bomb]   = "�C�� ";
  text[Trap]   = "� ";
  text[Poison] = "�Ŗ� ";
  text[Back]   = "�߂� ";
};


void dev::update() {
  if (stateChange()) { return; }
  buttonSelect();
}


bool dev::stateChange() {
  switch (draw_state_) {
    case command::MoveIn : moveMode();  break;
    case command::Blink  : blinkMode(); break;
    case command::MoveOut: backMode();  break;

    default: return false;
  }

  return true;
}


void dev::moveMode() {
  for (i = 0; i < All_Text; ++i) {
    // TIPS: �ړ������������{�^���̏������X�L�b�v
    if (x[i] <= 0.f) { continue; }

    if (i == 0) { x[i] -= 10.f; }
    else if (x[i - 1] <= design::DefaultPos / 1.5f) { x[i] -= 10.f; }
  }
  if (anime.alpha_ < 0.75f) { anime.alpha_ += 0.0125f; }

  // TIPS: �{�^���̈ړ�������������I�����[�h�ɂ���
  if (x[Back] <= 0.f) { ++draw_state_; }
}


void dev::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= design::Fps) { ++draw_state_; }
}


void dev::backMode() {
  for (i = 0; i < All_Text; ++i) { x[i] += 10.f; }
  if (anime.alpha_ >= 0.f) { anime.alpha_ -= 0.05f; }
  if (x[Back] >= design::DefaultPos) {
    mode_ = mode::Neutral;
    poli_state_ = command::MoveIn;
    init();
  }
}


void dev::buttonSelect() {
  for (i = 0; i < All_Text; ++i) {
    buttonPosTranslate(i);
    isAbleToDevelop(i);

    // TIPS: �J���ł����ԂłȂ���΁A�N���b�N�������Ȃ�
    if (rectOnMouse(pos_, size_) && is_develop_ &&
        win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click);
      ++draw_state_;
      id_ = i;
      paymentMoney();
    }
  }
}


void dev::paymentMoney() {
  switch (id_) {
    case   Fire: is_fire();   break;
    case   Bomb: is_bomb();   break;
    case   Trap: is_trap();   break;
    case Poison: is_poison(); break;

    // �߂�{�^���̎��͉������Ȃ�
    default:;
  }
}


void dev::is_fire() {
  money_ -= price::Fire;
  ++fire_;
}


void dev::is_bomb() {
  money_ -= price::Bomb;
  ++cannon_;
}


void dev::is_trap() {
  money_ -= price::Trap;
  ++trap_;
}


void dev::is_poison() {
  money_ -= price::Poison;
  ++poison_;
}


void dev::init() {
  buttonPosInit();

  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  draw_state_ = command::MoveIn;
  id_ = -1;
}


void dev::draw() {
  for (i = 0; i < All_Text; ++i) {
    buttonPosTranslate(i);
    isAbleToDevelop(i);

    on_mouse_ = rectOnMouse(pos_, size_);
    if (draw_state_ != command::Select || pause_) { on_mouse_ = false; }
    win::draw(pos_, size_, win::color(!is_develop_ ? paint::Gray
      : on_mouse_ ? paint::Orange : paint::Blue,
      id_ == i ? (anime.blink_ / 2) % 2 : anime.alpha_), anime.alpha_ * 1.25f);

    pos_.x() += (size_.x() - telop_.drawSize(text[i]).x()) / 2;
    pos_.y() += 15;
    telop_.draw(text[i], pos_, win::color(paint::White,
      id_ == i ? (anime.blink_ / 2) % 2 : 1.f));

    if (on_mouse_) { disp_telop(i); }
  }
}


void dev::isAbleToDevelop(const short& i) {
  switch (i) {
    case   Fire: is_develop_ = money_ >= price::Fire;   break;
    case   Bomb: is_develop_ = money_ >= price::Bomb;   break;
    case   Trap: is_develop_ = money_ >= price::Trap;   break;
    case Poison: is_develop_ = money_ >= price::Poison; break;

    // TIPS: �܂Ƃ߂Ĕ��菈�������Ă���֌W�ŁA
    //     : �߂�{�^���܂Ŏg���Ȃ��Ȃ�̂ŁA��ɃN���b�N�\�ɂ��Ă���
    default: is_develop_ = true;
  }
}


void dev::buttonPosTranslate(const short& i) {
  pos_.x() = size::CommandPos_X - x[i];
  pos_.y() = size::CommandPos_Y - (i + 1.5f) * (size_.y() + 10);
}


void dev::buttonPosInit() {
  for (i = 0; i < All_Text; ++i) { x[i] = design::DefaultPos; }
}


void dev::disp_telop(const short& id) {
  std::string text_1;   // �P�s�ڂ̃e�L�X�g
  std::string text_2;   // �Q�s�ڂ̃e�L�X�g

  pos_.x() = size::BottomPos_X + 20;
  pos_.y() = size::BottomPos_Y + 85;

  // TIPS: �e�{�^���̉��
  switch (id) {
    case Fire:
      text_1 = "�J����F1000 G";
      telop_.draw(text_1, pos_, win::color(paint::White));

      if (!is_develop_) {
        pos_.x() += telop_.drawSize(text_1).x();
        text_1 = "  �J���ł��܂���";
        telop_.draw(text_1, pos_, win::color(paint::Pink));
      }

      text_2 = "�G�P�̂ɑ΂��ă_���[�W�@�З́F��";
      pos_.x() = size::BottomPos_X + 20;
      pos_.y() -= 60;
      telop_.draw(text_2, pos_, win::color(paint::White));
      break;

    case Bomb:
      text_1 = "�J����F3000 G";
      telop_.draw(text_1, pos_, win::color(paint::White));

      if (!is_develop_) {
        pos_.x() += telop_.drawSize(text_1).x();
        text_1 = "  �J���ł��܂���";
        telop_.draw(text_1, pos_, win::color(paint::Pink));
      }

      text_2 = "�G�S�̂ɑ΂��ă_���[�W�@�З́F��";
      pos_.x() = size::BottomPos_X + 20;
      pos_.y() -= 60;
      telop_.draw(text_2, pos_, win::color(paint::White));
      break;

    case Trap:
      text_1 = "�J����F3000 G";
      telop_.draw(text_1, pos_, win::color(paint::White));

      if (!is_develop_) {
        pos_.x() += telop_.drawSize(text_1).x();
        text_1 = "  �J���ł��܂���";
        telop_.draw(text_1, pos_, win::color(paint::Pink));
      }

      text_2 = "��莞�ԁA�G�S�̂̍s����x������";
      pos_.x() = size::BottomPos_X + 20;
      pos_.y() -= 60;
      telop_.draw(text_2, pos_, win::color(paint::White));
      break;

    case Poison:
      text_1 = "�J����F2000 G";
      telop_.draw(text_1, pos_, win::color(paint::White));

      if (!is_develop_) {
        pos_.x() += telop_.drawSize(text_1).x();
        text_1 = "  �J���ł��܂���";
        telop_.draw(text_1, pos_, win::color(paint::Pink));
      }

      text_2 = "��莞�ԁA�G�S�̂̍U���͂��_�E��";
      pos_.x() = size::BottomPos_X + 20;
      pos_.y() -= 60;
      telop_.draw(text_2, pos_, win::color(paint::White));
      break;

    // �߂�{�^���͉���s�v
    default:;
  }
}
