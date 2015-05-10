
#include "strategy.h"

typedef cStrategy  strategy;


cStrategy::cStrategy() :
bg_(Texture("res/png/world_map.png")),
smoke_(Texture("res/png/world_fog.png")),
sea_(Texture("res/png/world_sea.png")),
fog_(Texture("res/png/world_sea_fog.png")),
icon_(Texture("res/png/battle_icon.png")),
world_icon_(Texture("res/png/world_icon.png")),
telop_(FONT_),
unit_(FONT_),
size_(Vec2f(size::Small_W, size::Small_H)),
mode_(data::system.strategy.mode_),
now_scene_(data::system.scene[play::Now]),
next_scene_(data::system.scene[play::Next]),
last_scene_(data::system.scene[play::Last]),
draw_state_(data::system.strategy.draw_state_),
pause_(data::system.pause_) {

  telop_.size(fontSize::Small);
  unit_.size(fontSize::Info);

  text[mode::Funds]      = "�����J�� ";
  text[mode::Armaments]  = "�R�� ";
  text[mode::Develop]    = "�J�� ";
  text[mode::Invasion]   = "�N�U ";
  text[mode::DataSelect] = "�f�[�^ ";
  text[mode::Neutral]    = "�I�� ";

  icon_name[0] = "�΁F";
  icon_name[1] = "�C�F";
  icon_name[2] = "㩁F";
  icon_name[3] = "�ŁF";

  choice[Save] = "�Z�[�u ";
  choice[Load] = "���[�h ";
  choice[Back] = "�߂� ";
}


void strategy::update() {
  // TIPS: �R�}���h��I���\�ȏ�ԂłȂ���΁A���͂������Ȃ�
  if (stateShift()) { return; }

  switch (mode_) {
    case mode::Funds    : funds_.update();     break;
    case mode::Armaments: armaments_.update(); break;
    case mode::Develop  : develop_.update();   break;
    case mode::Invasion : invasion_.update();  break;
    default: commandSelect();
  }
}


bool strategy::stateShift() {

  // TIPS: �{�^�����A�j���[�V�������Ă��Ȃ����̂݁A���͂����ł���悤�ɂ���
  switch (draw_state_) {
    case command::MoveIn : moveMode();  break;
    case command::Blink  : blinkMode(); break;
    case command::MoveOut: backMode();  break;

    default: return false;
  }

  return true;
}


void strategy::moveMode() {
  const float moveButtonSpeed = 10.0f;

  for (i = 0; i < mode::Neutral; ++i) {
    // TIPS: �ړ������������{�^���̏������X�L�b�v
    if (x[i] <= 0.f) { continue; }

    if (i == 0) { x[i] -= moveButtonSpeed; }
    else if (x[i - 1] <= design::DefaultPos / 1.5f) { x[i] -= moveButtonSpeed; }
  }
  if (anime.alpha_ < 0.75f) { anime.alpha_ += 0.0125f; }

  // TIPS: �{�^���̈ړ�������������I�����[�h�ɂ���
  if (x[mode::DataSelect] <= 0.f) { ++draw_state_; }
}


void strategy::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= design::Fps) { ++draw_state_; }
}


void strategy::backMode() {
  for (i = 0; i < mode::Neutral; ++i) { x[i] += 10.f; }
  if (anime.alpha_ >= 0.f) { anime.alpha_ -= 0.05f; }
  if (x[mode::DataSelect] >= design::DefaultPos) { commandChange(); }
}


// �{�^���I���̏���
void strategy::commandSelect() {
  select_ = mode_ == mode::DataSelect;
  for (i = 0; i < (select_ ? All_Command : mode::Neutral); ++i) {
    buttonPosTranslate(i);

    // TIPS: �o���ł��Ȃ���ԂȂ�o�������Ȃ�
    if (i == mode::Invasion && data::user.player.empty()) { continue; }

    if (rectOnMouse(pos_, size_) &&
        win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click);
      ++draw_state_;
      id_ = i;
    }
  }
}


// �R�}���h�̐؂�ւ�
void strategy::commandChange() {
  select_ ? shiftSelectMode() : shiftCommandMode();
}


void strategy::shiftCommandMode() {
  mode_ = mode(id_);

  switch (mode_) {
    case mode::Funds    : funds_.init();     break;
    case mode::Armaments: armaments_.init(); break;
    case mode::Develop  : develop_.init();   break;
    case mode::Invasion : invasion_.init();  break;
    default:;
  }

  anime.blink_ = 0;
  id_ = mode::Neutral;
  ++draw_state_;
  if (mode_ == mode::DataSelect) { draw_state_ = command::MoveIn; }
}


void strategy::shiftSelectMode() {
  switch (id_) {
    case Save: next_scene_ = scene::Save; break;
    case Load: next_scene_ = scene::Load; break;

    // TIPS: �߂�{�^��
    default: id_ = mode::Neutral;
  }

  if (id_ != mode::Neutral) { last_scene_ = now_scene_; }
  mode_ = mode(id_);
  anime.blink_ = 0;
  draw_state_ = command::MoveIn;
}


void strategy::init() {
  anime.time_ = 0;
  anime.blink_ = 0;
  anime.scroll_ = 0.f;
  anime.alpha_ = 0.f;

  mode_ = mode::Neutral;
  draw_state_ = command::MoveIn;
  id_ = mode::Neutral;
  buttonPosInit();

  data::music.stop();
  data::music.play(bgm::Strategy);
}


void strategy::draw() {
  effectPlay();
  disp_back();
  disp_mode();
  if (mode_ != mode::Invasion) { disp_symbol(); }
  disp_info();

  // TIPS: �e�R�}���h�̃{�^���Ɖ���̕\��
  switch (mode_) {
    case mode::Funds    : funds_.draw();     break;
    case mode::Armaments: armaments_.draw(); break;
    case mode::Develop  : develop_.draw();   break;
    case mode::Invasion : invasion_.draw();  break;
    default: disp_button();
  }

  // TIPS: �N�U�R�}���h�I�𒆂̓E�B���h�E�̏���\�����Ȃ�
  if (mode_ == mode::Invasion) { return; }

  info_strategy();
  info_unit();
}


void strategy::effectPlay() {
  ++anime.time_;
  if (anime.time_ >= design::Fps - 10) { anime.time_ = 0; }

  anime.scroll_ += 0.25f;
  if (anime.scroll_ >= size::Rect_512) { anime.scroll_ = 0.f; }
}


void strategy::disp_back() {

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

  // �_
  drawTextureBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    anime.scroll_, 0, size::Rect_512, size::Rect_512,
    smoke_, win::color(paint::White, 0.5f));

  // �R�}���h����p�̃E�B���h�E
  // TIPS: �Ƃ肠�����E�B���h�E������\��
  //     : �e�R�}���h�̏����ɓ��������A���ꂼ��ʁX�ɕ\��������
  win::draw(Vec2f(size::BottomPos_X, size::BottomPos_Y),
    Vec2f(size::Bottom_W, size::Bottom_H),
    win::color(paint::Gray, 0.75f));
}


void strategy::disp_mode() {
  if (next_scene_ == scene::Save || next_scene_ == scene::Load) { return; }

  pos_.x() = size::CommandPos_X;
  pos_.y() = size::CommandPos_Y;
  win::draw(pos_, size_, win::color(paint::Brown, 0.75f));

  pos_.x() += (size_.x() - telop_.drawSize(text[mode_]).x()) / 2;
  pos_.y() += 15;
  telop_.draw(text[mode_], pos_, win::color(paint::White));
}


// �v���C���[�̏��
void strategy::disp_info() {
  // TIPS: �N�U�R�}���h�I�𒆂̓E�B���h�E�̏���\�����Ȃ�
  if (mode_ == mode::Invasion) { return; }

  // �E�B���h�E
  win::draw(Vec2f(0, size::BottomPos_Y + size::Bottom_H + 10),
    Vec2f(size::Half_W - 10, size::HEIGHT - (size::Bottom_H + 30)),
    win::color(paint::Gray, 0.75f));

  // TIPS: �������ȊO�̃f�[�^�͉��o��������̂ŁA�ʂɕ`����s��
  info_money();
}


void strategy::info_money() {
  pos_.x() = size::DataPos_X;
  pos_.y() = size::DataPos_Y;
  unit_.draw("������", pos_, win::color(paint::White));

  sstream money;
  money << std::setw(8) << data::user.weapon.money_ << " G";

  pos_.y() -= size::Icon_H * 0.75f;
  unit_.draw(money.str(), pos_, win::color(paint::White));
}


void strategy::info_strategy() {
  for (i = 0; i < 4; ++i) {
    pos_.x() = size::DataPos_X + (i % 2 + 1) * 160;
    pos_.y() = size::DataPos_Y - size::Icon_H * (i / 2) * 0.75f;

    drawTextureBox(pos_.x(), pos_.y(), size::Icon_W / 2, size::Icon_H / 2,
      size::Icon_W * i, 0, size::Icon_W, size::Icon_H,
      icon_, win::color(paint::White),
      0, Vec2f(1, 1), Vec2f(0, 10));

    short val;
    switch (i) {
      case 0: val = data::user.weapon.fire_;   break;
      case 1: val = data::user.weapon.cannon_; break;
      case 2: val = data::user.weapon.trap_;   break;
      case 3: val = data::user.weapon.poison_; break;
      default: val = 0;
    }

    pos_.x() += 40;
    sstream temp;
    temp << icon_name[i] << std::setw(3) << val;
    unit_.draw(temp.str(), pos_, win::color(paint::White));
  }
}


void strategy::info_unit() {
  begin_ = data::user.player.begin();
  end_ = data::user.player.end();
  for (it_ = begin_, i = 0; it_ != end_; ++it_, ++i) {
    pos_.x() = size::DataPos_X + 150;
    pos_.y() = size::DataPos_Y - size::UnitData_Y * (i + 1.25f);

    sstream name;
    name << " LV:" << std::setw(3) << it_->getInfo().level_
      << "  " << it_->getInfo().name_;
    unit_.draw(name.str(), pos_, win::color(paint::White));

    pos_.y() -= 30;
    sstream hp;
    hp << " HP: " << std::setw(4)
      << it_->getHp().now_ << " / "
      << it_->getHp().max_;
    unit_.draw(hp.str(), pos_, win::color(paint::White));

    pos_.x() -= 100;
    pos_.y() -= 8;
    it_->drawMinUnit(pos_);
  }
}


void strategy::disp_button() {
  select_ = mode_ == mode::DataSelect;
  for (i = 0; i < (select_ ? All_Command : mode::Neutral); ++i) {
    buttonPosTranslate(i);

    unit_empty_ = (i == mode::Invasion && data::user.player.empty());

    on_mouse_ = rectOnMouse(pos_, size_);
    if (draw_state_ != command::Select || pause_) { on_mouse_ = false; }

    win::draw(pos_, size_, win::color(unit_empty_ ? paint::Gray :
      on_mouse_ ? paint::Orange : paint::Blue,
      id_ == i ? (anime.blink_ / 2) % 2 : anime.alpha_), anime.alpha_ * 1.25f);

    pos_.x() += (size_.x() - telop_.drawSize(select_ ? choice[i] : text[i]).x()) / 2;
    pos_.y() += 15;
    telop_.draw(select_ ? choice[i] : text[i], pos_, win::color(paint::White,
      id_ == i ? (anime.blink_ / 2) % 2 : 1.f));

    if (on_mouse_) { disp_telop(i); }
  }
}


// �e�R�}���h�̉���e���b�v
// TIPS: ���̊֐��ŕ\������̂̓R�}���h�I���̉���̂�
void strategy::disp_telop(const short& id) {
  pos_.x() = size::BottomPos_X + 20;
  pos_.y() = size::BottomPos_Y + 85;

  // TIPS: �e�{�^���̉��
  mode_ == mode::DataSelect ? selectMode(id) : commandMode(id);
}


void strategy::commandMode(const short& id) {
  std::string text_;

  switch (id) {
    case mode::Funds:
      text_ = "���m�̊����A�܂��͕���𔄋p���Ď����𓾂܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case mode::Armaments:
      text_ = "���m�̊Ǘ����s���܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case mode::Develop:
      text_ = "������J�����܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case mode::Invasion:
      text_ = "�G�̗̒n�i�ԁj�Ɍ����ĐN�U���܂�";
      telop_.draw(text_, pos_, win::color(paint::White));

      pos_.y() -= 60;
      text_ = "�����̗̒n�i�j�ŌP�����܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case mode::DataSelect:
      text_ = "�Z�[�u�f�[�^�̕ۑ��A�Ǎ����s���܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    default:;
  }
}


void strategy::selectMode(const short& id) {
  std::string text_;

  switch (id) {
    case Save:
      text_ = "���݂̃f�[�^��ۑ����܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case Load:
      text_ = "�ۑ����ꂽ�󋵂���ĊJ���܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    default:
      text_ = "�R�}���h�I���ɖ߂�܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
  }
}


void strategy::disp_symbol() {
  for (i = 0; i < design::AllStage; ++i) {
    pos_.x() = size::InvasionPos_X + (i % design::InvasionIcon_X) * size::Invasion_W;
    pos_.y() = size::InvasionPos_Y - (i / design::InvasionIcon_X) * size::Invasion_H;

    symbol_flag_ = data::user.map_info[i].flag_;

    drawTextureBox(pos_.x(), pos_.y(), size::Icon_W, size::Icon_H,
      0, 0, size::Icon_W, size::Icon_H,
      world_icon_,
      win::color(!symbol_flag_ ? paint::Purple : paint::SkyBlue));
  }
}


void strategy::buttonPosTranslate(const short& i) {
  pos_.x() = size::CommandPos_X - x[i];
  pos_.y() = size::CommandPos_Y - (i + 1.5f) * (size_.y() + 10);
}


void strategy::buttonPosInit() {
  for (i = 0; i < mode::Neutral; ++i) { x[i] = design::DefaultPos; }
}
