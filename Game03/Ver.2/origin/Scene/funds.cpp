
#include "funds.h"
#include "../../lib/utils.hpp"

typedef cFunds  fund;
using namespace literal_effect;
using namespace literal_strategy;


cFunds::cFunds() :
telop_(FONT_STRING),
draw_state_(data::system.strategy_state.draw_),
mode_state_(data::system.strategy_state.mode_),
money_(data::user.assets.money_),
food_(data::user.assets.food_),
work_(data::user.cnt.work_),
cnt_fire_(data::user.weapon_cnt.fire_),
cnt_bomb_(data::user.weapon_cnt.bomb_),
cnt_trap_(data::user.weapon_cnt.trap_),
cnt_poison_(data::user.weapon_cnt.poison_) {

  telop_.size(font::Standard);

  const std::string command_name[] = {
    "���m�h�� ",
    "��:�Ή��r ",
    "��:��C ",
    "��:� ",
    "��:�Ŗ� ",
    "�߂� ",
  };

  for (index_ = 0; index_ < elemsof(command_name); ++index_) {
    command_[index_] = command_name[index_];
  }

  const std::string sell = "���p�z�F ";
  const std::string unable = "���������Ă��܂��� ";
  const sDescriptionText descript_text[] = {
    "�퓬�ȊO�̊����Ŏ����𓾂܂� ", "�����F�p�[�e�B�̕��σ��x�� �~ 100 �f ", "����ȏ㊈���ł��܂��� ",
    "����F�Ή��r �𔄋p���܂� ", sell, unable,
    "����F��C�@ �𔄋p���܂� ", sell, unable,
    "����F㩁@�@ �𔄋p���܂� ", sell, unable,
    "����F�Ŗ�@ �𔄋p���܂� ", sell, unable,
    "�R�}���h�I���ɖ߂�܂� ", "", "",
  };

  short index = 0;
  for (auto& it : description_) {
    it = descript_text[index];
    ++index;
  }
}


void fund::update() {
  if (stateChange()) { return; }
}


// TIPS: �{�^�����A�j���[�V�������ĂȂ����̂݁A���͂����ł���悤�ɂ���
bool fund::stateChange() {
  switch (anime.state_) {
    case MoveIn_1 : moveMode();  break;
    case Blink_1  : blinkMode(); break;
    case MoveOut_1: backMode();  break;

    default: return false;
  }

  return true;
}


void fund::moveMode() {
  for (index_ = 0; index_ < str::All_Funds; ++index_) {
    // TIPS: �ړ������������{�^���̏������X�L�b�v
    if (move_x_[index_] >= 0.0f) { continue; }

    if ((index_ > 0 && move_x_[index_ - 1] > MoveStartPos) || index_ == 0) {
      move_x_[index_] += buttonMoveSpeed;
    }
  }
  if (anime.alpha_ < 0.75f) { anime.alpha_ += buttonAlphaSpeed / 2; }

  // TIPS: �{�^���̈ړ�������������I�����[�h�ɂ���
  if (move_x_[f_Back] >= 0.0f) { ++anime.state_; }
}


void fund::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= spec::Fps) {
    anime.state_ = click_id_ == f_Back ? MoveOut_1 : Select_1;
  }
}


void fund::backMode() {
  for (auto& it : move_x_) {
    if (it > DefaultCommandPos) { it -= buttonMoveSpeed; }
  }

  if (anime.alpha_ > 0.0f) {
    anime.alpha_ -= buttonAlphaSpeed;
    return;
  }

  buttonPosInit();
  draw_state_ = MoveIn_1;
  mode_state_ = s_Neutral;
}


void fund::init() {
  anime.state_ = MoveIn_1;
  anime.blink_ = 0;
  anime.alpha_ = 0.0f;

  click_id_ = f_Neutral;

  buttonPosInit();
}


void fund::draw() {
  for (index_ = 0; index_ < All_Funds; ++index_) {
    buttonPosTranslate(index_);
    on_mouse_ = env::rectOnMouse(pos_, buttonSize);

    // TIPS: ���o���̓{�^���̐F��ς��Ȃ�
    if (anime.state_ != Select_1) { on_mouse_ = false; }

    disp_button(index_);
    disp_telop(index_);
  }
}


void fund::disp_button(const short index) {
  draw::window(pos_, buttonSize,
               draw::color(on_mouse_ ? paint::Orange : paint::Blue,
               click_id_ == index ?
               (anime.blink_ / blinkSpeed) % 2 : anime.alpha_),
               anime.alpha_ + buttonFrameAlpha);

  const float StringPos_X
    = (buttonSize.x() - telop_.drawSize(command_[index]).x()) / 2;

  pos_.x() += StringPos_X;
  pos_.y() += StringPos_Y;
  telop_.draw(command_[index], pos_,
              draw::color(paint::White,
              anime.alpha_ + buttonFrameAlpha));
}


void fund::disp_telop(const short index) {
  // TIPS: �J�[�\�����{�^���̏�ɂȂ���΃e�L�X�g��\�����Ȃ�
  if (!on_mouse_) { return; }

  pos_ = Vec2f(DescriptPos_X, DescriptPos_Y);
  telop_.draw(description_[index].first_, pos_,
              draw::color(paint::White));

  short val;
  switch (index) {
    case Sell_Fire  : val = p_Fire / 2;   break;
    case Sell_Bomb  : val = p_Bomb / 2;   break;
    case Sell_Trap  : val = p_Trap / 2;   break;
    case Sell_Poison: val = p_Poison / 2; break;

    // TIPS: ����ȊO�̃{�^���͕\�����Ȃ�
    default: return;
  }

  sstream sell_price;
  sell_price
    << description_[index].second_
    << std::setw(PriceLength) << val << " �f ";

  pos_.y() -= DescriptLine_H;
  telop_.draw(sell_price.str(), pos_, draw::color(paint::White));
}


void fund::buttonPosTranslate(const short index) {
  pos_.x() = CommandPos_X + move_x_[index];
  pos_.y() = CommandPos_Y - Command_H * index;
}


void fund::buttonPosInit() {
  for (auto& it : move_x_) {
    it = DefaultCommandPos;
  }
}
