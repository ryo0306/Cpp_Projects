
#include "invasion.h"

typedef cInvasion  inv;


cInvasion::cInvasion() :
icon_(Texture("res/png/world_icon.png")),
telop_(FONT_),
size_(Vec2f(size::Small_W, size::Small_H)),
icon_size_(Vec2f(size::Icon_W, size::Icon_H)),
mode_(data::system.strategy.mode_),
poli_state_(data::system.strategy.draw_state_),
stage_id_(data::system.strategy.stage_id_),
pause_(data::system.pause_) {

  telop_.size(fontSize::Small);

  map[effect::Grass]  = "�����F�q�� Up";
  map[effect::Forest] = "�X�сF�q�� Down�A���U Down�A���� Up";
  map[effect::Mount]  = "�R�x�F���� Down�A�Z�p Down�A���h Up";

  effect[effect::Plain]        = "�@�@�F������ʂȂ�";
  effect[effect::Front_Adv]    = "�@�@�F�O��̃��j�b�g ����";
  effect[effect::Back_Adv]     = "�@�@�F���̃��j�b�g ����";
  effect[effect::Fighter_Adv]  = "�@�@�F�t�@�C�^�[ ����";
  effect[effect::Scout_Adv]    = "�@�@�F�X�J�E�g ����";
  effect[effect::Knight_Adv]   = "�@�@�F�i�C�g ����";
  effect[effect::Mage_Adv]     = "�@�@�F���C�W ����";
  effect[effect::Priest_Adv]   = "�@�@�F�v���[�X�g ����";
  effect[effect::Soldier_Adv]  = "�@�@�F���l�L������";
  effect[effect::Boss_Adv]     = "�@�@�F�s�K�ȏo�������N���肻���E�E�E";
  effect[effect::Male_Adv]     = "�@�@�F�j�� �� ���� �ɑ΂��� �L��";
  effect[effect::Female_Adv]   = "�@�@�F���� �� �j�� �ɑ΂��� �L��";
  effect[effect::Trap_Start]   = "�@�@�F�s�K�ȏo�������N���肻���E�E�E";
  effect[effect::Poison_Start] = "�@�@�F�s�K�ȏo�������N���肻���E�E�E";
  effect[effect::Unlucky]      = "�@�@�F�s�K�ȏo�������N���肻���E�E�E";
};


void inv::update() {
  if (stateChange()) { return; }
  buttonSelect();
  back_command();
}


bool inv::stateChange() {
  switch (draw_state_) {
    case command::MoveIn : moveMode();  break;
    case command::Blink  : blinkMode(); break;
    case command::MoveOut: backMode();  break;

    default: return false;
  }

  return true;
}


void inv::moveMode() {
  const float moveButtonSpeed = 10.0f;
  if (x > 0.f) { x -= moveButtonSpeed; }

  // TIPS: �{�^���̃t�F�[�h�C�����I���܂Ŏ��̒i�K�ɐi�܂��Ȃ�
  if (anime.alpha_ < 0.75f) { anime.alpha_ += 0.0125f; return; }
  ++draw_state_;
}


void inv::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= design::Fps) { ++draw_state_; }
}


void inv::backMode() {

  // TIPS: �����ɂȂ�܂Ŏ��̒i�K�ɐi�܂��Ȃ�
  if (anime.alpha_ >= 0.f) { anime.alpha_ -= 0.05f; return; }

  if (id_ != Back) {
    stage_id_ = id_;
    data::system.scene[play::Next] = scene::Battle;
    ++data::user.count.battle_;
    data::enemySpown();
  }

  mode_ = mode::Neutral;
  poli_state_ = command::MoveIn;
  init();
}


// �N�U����}�X�̏���
void inv::buttonSelect() {
  for (i = 0; i < design::AllStage; ++i) {
    buttonPosTranslate(i);

    if (rectOnMouse(pos_, icon_size_) &&
        win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click);
      ++draw_state_;
      id_ = i;
    }
  }
}


// �߂�{�^���̏���
void inv::back_command() {
  pos_.x() = size::CommandPos_X - x;
  pos_.y() = size::CommandPos_Y - 1.5f * (size_.y() + 10);

  if (rectOnMouse(pos_, size_) &&
      win::app->isPushButton(Mouse::LEFT)) {
    data::music.sePlay(se::Click);
    ++draw_state_;
    id_ = Back;
  }
}


void inv::init() {
  buttonPosInit();

  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  draw_state_ = command::MoveIn;
  id_ = -1;
}


// �N�U����}�X
void inv::draw() {
  for (i = 0; i < design::AllStage; ++i) {
    buttonPosTranslate(i);

    on_mouse_ = rectOnMouse(pos_, icon_size_);
    if (draw_state_ != command::Select || pause_) { on_mouse_ = false; }

    flag_ = data::user.map_info[i].flag_;

    drawTextureBox(pos_.x(), pos_.y(), size::Icon_W, size::Icon_H,
      0, 0, size::Icon_W, size::Icon_H,
      icon_, win::color(!flag_ ? on_mouse_ ?
      paint::Pink : paint::Purple : paint::SkyBlue,
      id_ == i ? (anime.blink_ / 2) % 2 : 1.f));

    if (on_mouse_) { disp_mapInfo(i); }
  }

  disp_command();
}


void inv::disp_mapInfo(const short& map_id) {

  // TIPS: �}�b�v���ʕ\��
  pos_.x() = size::BottomPos_X + 30;
  pos_.y() = size::BottomPos_Y + 85;

  u_short& map_info = data::user.map_info[map_id].id_;
  u_short& effect_info = data::user.map_info[map_id].attribute_;

  telop_.draw(map[map_info], pos_, win::color(paint::White));

  pos_.y() -= 60;
  telop_.draw(effect[effect_info], pos_, win::color(paint::White));
}


// �߂�{�^��
void inv::disp_command() {
  pos_.x() = size::CommandPos_X - x;
  pos_.y() = size::CommandPos_Y - 1.5f * (size_.y() + 10);

  on_mouse_ = rectOnMouse(pos_, size_);
  if (draw_state_ != command::Select || pause_) { on_mouse_ = false; }
  win::draw(pos_, size_, win::color(on_mouse_ ? paint::Orange : paint::Blue,
    id_ == Back ? (anime.blink_ / 2) % 2 : anime.alpha_), anime.alpha_ * 1.25f);

  pos_.x() += (size_.x() - telop_.drawSize("�߂� ").x()) / 2;
  pos_.y() += 15;
  telop_.draw("�߂� ", pos_, win::color(paint::White,
    id_ == Back ? (anime.blink_ / 2) % 2 : anime.alpha_ * 1.25f));
}


void inv::buttonPosTranslate(const short& i) {
  pos_.x() = size::InvasionPos_X + (i % design::InvasionIcon_X) * size::Invasion_W;
  pos_.y() = size::InvasionPos_Y - (i / design::InvasionIcon_X) * size::Invasion_H;
}


void inv::buttonPosInit() {
  x = design::DefaultPos;
}
