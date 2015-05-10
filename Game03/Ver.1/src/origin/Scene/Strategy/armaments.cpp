
#include "armaments.h"

typedef cArmaments  arma;


cArmaments::cArmaments() :
size_(Vec2f(200, size::Command_H)),
info_size_(Vec2f(size::Half_W - 50, size::UnitData_Y - 10)),
mode_(data::system.strategy.mode_),
poli_state_(data::system.strategy.draw_state_),
money_(data::user.weapon.money_),
pause_(data::system.pause_),
telop_(FONT_) {

  telop_.size(fontSize::Small);

  text[Growth] = "�琬 ";
  text[Change] = "����ύX ";
  text[Gender] = "���ʕύX ";
  text[Shift]  = "����ύX ";
  text[Back]   = "�߂� ";
};


void arma::update() {
  if (stateChange()) { return; }
  draw_state_ == command::Select ? buttonSelect() : armamentSelect();
}


bool arma::stateChange() {
  switch (draw_state_) {
    // TIPS: �{�^���̉��o
    case command::MoveIn : moveMode();  break;
    case s_Blink:;
    case command::Blink  : blinkMode(); break;
    case s_Out:;
    case command::MoveOut: backMode();  break;

    // TIPS: �{�^���I��
    default: return false;
  }

  return true;
}


void arma::moveMode() {
  for (i = 0; i < All_Text; ++i) {
    // TIPS: �ړ������������{�^���̏������X�L�b�v
    if (button_x[i] <= 0.f) { continue; }

    if (i == 0) { button_x[i] -= 10.f; }
    else if (button_x[i - 1] <= design::DefaultPos / 1.5f) { button_x[i] -= 10.f; }
  }
  if (anime.alpha_ < 0.75f) { anime.alpha_ += 0.0125f; }

  // TIPS: �{�^���̈ړ�������������I�����[�h�ɂ���
  if (button_x[Back] <= 0.f) { ++draw_state_; }
}


void arma::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= design::Fps) {
    if (id_ == s_Back) {
      draw_state_ = command::Select;
      anime.blink_ = 0;
      return;
    }

    if (id_ == Back) {
      draw_state_ = command::MoveOut;
      return;
    }

    draw_state_ = id_ + s_Growth;
    id_ = -1;
  }
}


void arma::backMode() {
  for (i = 0; i < All_Text; ++i) { button_x[i] += 10.f; }
  if (anime.alpha_ >= 0.f) { anime.alpha_ -= 0.05f; }
  if (button_x[Back] >= design::DefaultPos) {
    mode_ = mode::Neutral;
    poli_state_ = command::MoveIn;
    init();
  }
}


void arma::buttonSelect() {
  for (i = 0; i < All_Text; ++i) {
    buttonPosTranslate(i);

    if (rectOnMouse(pos_, size_) &&
        win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click);
      ++draw_state_;
      id_ = i;
    }
  }
}


void arma::armamentSelect() {
  begin_ = data::user.player.begin();
  end_ = data::user.player.end();

  for (it_ = begin_, num_ = 0; it_ != end_; ++it_, ++num_) {
    unitPosTranslate(num_);
    on_mouse_ = rectOnMouse(pos_, info_size_);

    switch (draw_state_) {
      case s_Growth: growthMode(); break;
      case s_Change: changeMode(); break;
      case s_Gender: genderMode(); break;
      case  s_Shift: shiftMode();  break;

      default:;
    }
  }

  buttonPosTranslate(0);
  on_mouse_ = rectOnMouse(pos_, size_);
  if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
    click();
    id_ = s_Back;
    draw_state_ = s_Blink;
    anime.blink_ = 0;
  }
}


// �N���X���ƂɌ��܂����l�����p�����[�^����
void arma::growthMode() {
  u_short cost = it_->getInfo().level_ * 100;
  is_click_ = (money_ >= cost);

  // TIPS: ���x���A�b�v�ł��邾���̏��������Ȃ���΁A���͂������Ȃ�
  if (is_click_ && on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
    click();
    money_ -= it_->getInfo().level_ * 100;
    it_->growth();
  }
}


// ���ԂɃN���X��؂�ւ���
void arma::changeMode() {
  if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
    click();
    it_->classChange();
  }
}


void arma::genderMode() {
  if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
    click();
    it_->genderChange();
  }
}


// ��������ւ���
void arma::shiftMode() {
  if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
    click();
    it_->laneShift();
  }
}


// TIPS: �N���b�N�����Ƃ��̋��ʏ���
void arma::click() {
  data::music.sePlay(se::Click);
}


void arma::init() {
  buttonPosInit();

  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  draw_state_ = command::MoveIn;
  id_ = -1;
}


void arma::draw() {
  draw_state_ < s_Growth ? disp_command() : disp_unit();
}


void arma::disp_command() {
  for (i = 0; i < All_Text; ++i) {
    buttonPosTranslate(i);

    on_mouse_ = rectOnMouse(pos_, size_);

    // TIPS: ���o���̓{�^���̐F��ς��Ȃ�
    if (draw_state_ != command::Select || pause_) { on_mouse_ = false; }
    win::draw(pos_, size_, win::color(on_mouse_ ? paint::Orange : paint::Blue,
      id_ == i ? (anime.blink_ / 2) % 2 : anime.alpha_), anime.alpha_ * 1.25f);

    // TIPS: �{�^���̒����ɗ���悤�Ɉʒu�𒲐�
    pos_.x() += (size_.x() - telop_.drawSize(text[i]).x()) / 2;
    pos_.y() += 15;
    telop_.draw(text[i], pos_, win::color(paint::White,
      id_ == i ? (anime.blink_ / 2) % 2 : 1.f));

    if (on_mouse_) { disp_telop(i); }
  }
}


void arma::disp_unit() {
  begin_ = data::user.player.begin();
  end_ = data::user.player.end();

  if (draw_state_ == s_Growth) { disp_growth_window(); }

  // TIPS: ���j�b�g���
  for (it_ = begin_, num_ = 0; it_ != end_; ++it_, ++num_) {
    unitPosTranslate(num_);

    on_mouse_ = rectOnMouse(pos_, info_size_);
    if (draw_state_ < s_Growth || draw_state_ > s_Shift || pause_) { on_mouse_ = false; }
    win::draw(pos_, info_size_,
      win::color(on_mouse_ ? paint::Orange : paint::Gray, anime.alpha_),
      anime.alpha_ * 1.25f);

    switch (draw_state_) {
      case s_Growth:
        if (on_mouse_) { disp_growth(); }
        break;

      case s_Change:
        if (on_mouse_) { disp_change(); }
        break;

      case s_Gender: disp_gender(); break;
      case s_Shift : disp_shift();  break;
      default:;
    }
  }

  // TIPS: �߂�{�^��
  buttonPosTranslate(0);
  on_mouse_ = rectOnMouse(pos_, size_);
  if (draw_state_ < s_Growth || draw_state_ > s_Shift || pause_) { on_mouse_ = false; }
  win::draw(pos_, size_, win::color(on_mouse_ ? paint::Orange : paint::Blue,
    id_ == s_Back ? (anime.blink_ / 2) % 2 : anime.alpha_),
    id_ == s_Back ? (anime.blink_ / 2) % 2 : anime.alpha_ * 1.25f);

  pos_.x() += (size_.x() - telop_.drawSize(text[Back]).x()) / 2;
  pos_.y() += 15;
  telop_.draw(text[Back], pos_, win::color(paint::White,
    id_ == s_Back ? (anime.blink_ / 2) % 2 : 1.f));
}


void arma::disp_growth_window() {
  // TIPS: ���E�B���h�E�`��
  pos_.x() = size::BottomPos_X;
  pos_.y() = size::BottomPos_Y + size::Bottom_H + 10;
  win::draw(pos_, Vec2f(490, 220), win::color(paint::Gray, 0.75f));
}


void arma::disp_growth() {
  // TIPS: �}�E�X�����킹�Ă��郆�j�b�g�̃p�����[�^�����ׂĕ\��
  pos_.x() = size::BottomPos_X + 30;
  pos_.y() = -70;
  sstream pow;
  pow << "���U: " << std::setw(3) << it_->getStatus().pow_;
  telop_.draw(pow.str(), pos_, win::color(paint::White));

  pos_.x() += 250;
  sstream vit;
  vit << "���h: " << std::setw(3) << it_->getStatus().vit_;
  telop_.draw(vit.str(), pos_, win::color(paint::White));

  pos_.x() = size::BottomPos_X + 30;
  pos_.y() -= 60;
  sstream mag;
  mag << "����: " << std::setw(3) << it_->getStatus().mag_;
  telop_.draw(mag.str(), pos_, win::color(paint::White));

  pos_.x() += 250;
  sstream spd;
  spd << "�q��: " << std::setw(3) << it_->getStatus().spd_;
  telop_.draw(spd.str(), pos_, win::color(paint::White));

  pos_.x() = size::BottomPos_X + 30;
  pos_.y() -= 60;
  sstream dex;
  dex << "�Z�p: " << std::setw(3) << it_->getStatus().dex_;
  telop_.draw(dex.str(), pos_, win::color(paint::White));

  pos_.x() += 250;
  sstream luk;
  luk << "�K�^: " << std::setw(3) << it_->getStatus().luk_;
  telop_.draw(luk.str(), pos_, win::color(paint::White));
}


void arma::disp_change() {
  std::string unit_info;

  // TIPS: �N���X�ɉ���������̕\��
  pos_.x() = size::BottomPos_X + 30;
  pos_.y() = size::BottomPos_Y + 85;

  switch (it_->getInfo().type_) {
    case character::Fighter:
      unit_info = "�X�J�E�g�@�@�ɃN���X�`�F���W���܂�";
      telop_.draw(unit_info, pos_, win::color(paint::White));

      pos_.y() -= 60;
      unit_info = "�t�@�C�^�[�F�q���������A�����U��������";
      telop_.draw(unit_info, pos_, win::color(paint::White));
      break;

    case character::Scout:
      unit_info = "�i�C�g�@�@�@�ɃN���X�`�F���W���܂�";
      telop_.draw(unit_info, pos_, win::color(paint::White));

      pos_.y() -= 60;
      unit_info = "�X�J�E�g�@�F���̓G��D�悵�čU������";
      telop_.draw(unit_info, pos_, win::color(paint::White));
      break;

    case character::Knight:
      unit_info = "���C�W�@�@�@�ɃN���X�`�F���W���܂�";
      telop_.draw(unit_info, pos_, win::color(paint::White));

      pos_.y() -= 60;
      unit_info = "�i�C�g�@�@�F�ݏd�����ł��ꋭ���A�U���͂�����";
      telop_.draw(unit_info, pos_, win::color(paint::White));
      break;

    case character::Mage:
      unit_info = "�v���[�X�g�@�ɃN���X�`�F���W���܂�";
      telop_.draw(unit_info, pos_, win::color(paint::White));

      pos_.y() -= 60;
      unit_info = "���C�W�@�@�F���@�œG�S�̂��U������";
      telop_.draw(unit_info, pos_, win::color(paint::White));
      break;

    case character::Priest:
      unit_info = "�t�@�C�^�[�@�ɃN���X�`�F���W���܂�";
      telop_.draw(unit_info, pos_, win::color(paint::White));

      pos_.y() -= 60;
      unit_info = "�v���[�X�g�F�����S�̂��񕜂ł��邪�A�U���s��";
      telop_.draw(unit_info, pos_, win::color(paint::White));
      break;

    default:;
  }
}


void arma::disp_gender() {
  pos_.x() -= 120;
  win::draw(pos_, Vec2f(80, 80),
    win::color(it_->getInfo().gender_ == character::Male ?
    paint::Blue : paint::Pink, 0.75f));

  telop_.draw(it_->getInfo().gender_ == character::Male ? "�j" : "��",
    Vec2f(pos_.x() + 20, pos_.y() + 25), win::color(paint::White));
}


void arma::disp_shift() {
  pos_.x() -= 120;
  win::draw(pos_, Vec2f(80, 80),
    win::color(it_->getLane() == character::Front ?
    paint::Green : paint::SkyBlue, 0.75f));

  telop_.draw(it_->getLane() == character::Front ? "�O" : "��",
    Vec2f(pos_.x() + 20, pos_.y() + 25), win::color(paint::White));
}


void arma::disp_telop(const short& id) {
  std::string text_;
  pos_.x() = size::BottomPos_X + 20;
  pos_.y() = size::BottomPos_Y + 85;

  switch (id) {
    case Growth:
      text_ = "�������g���ĕ��m���������܂�";
      telop_.draw(text_, pos_, win::color(paint::White));

      pos_.y() -= 60;
      text_ = "�\�͒l�̐����͌��݂̕���ɂ���ĕω����܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case Change:
      text_ = "����ɑΉ������퓬�s�����s�Ȃ��܂�";
      telop_.draw(text_, pos_, win::color(paint::White));

      pos_.y() -= 60;
      text_ = "�\�͒l�A�퓬���̍U���Ώۂɉe�����܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case Gender:
      text_ = "���m�̊O�ς�ύX���܂�";
      telop_.draw(text_, pos_, win::color(paint::White));

      pos_.y() -= 60;
      text_ = "���̎q�����ς����D���Ƃ��A�j���D���ȕ��A�ǂ���";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case Shift:
      text_ = "�O��A�������ւ��܂�";
      telop_.draw(text_, pos_, win::color(paint::White));

      pos_.y() -= 60;
      text_ = "�퓬���̑_���₷���ɉe�����܂�";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    default:;
  }
}


void arma::buttonPosTranslate(const short& i) {
  pos_.x() = size::CommandPos_X - button_x[i];
  pos_.y() = size::CommandPos_Y - (i + 1.5f) * (size_.y() + 10);
}


void arma::buttonPosInit() {
  for (i = 0; i < All_Text; ++i) { button_x[i] = design::DefaultPos; }
}


void arma::unitPosTranslate(const short& i) {
  pos_.x() = size::DataPos_X - 10;
  pos_.y() = size::DataPos_Y - (i + 1.75f) * size::UnitData_Y;
}
