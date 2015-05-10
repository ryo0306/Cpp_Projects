
#include "data_select.h"

typedef cDataSelect  d_select;


cDataSelect::cDataSelect() :
bg_(Texture("res/png/back_data_select.png")),
icon_(Texture("res/png/battle_icon.png")),
telop_(FONT_),
info_(FONT_),
slot_size_(Vec2f(size::Medium_W, size::Medium_H)),
now_scene_(data::system.scene[play::Now]),
next_scene_(data::system.scene[play::Next]),
last_scene_(data::system.scene[play::Last]) {

  telop_.size(fontSize::Small);
  info_.size(fontSize::Info);

  text[Slot1] = "スロット１ ";
  text[Slot2] = "スロット２ ";
  text[Slot3] = "スロット３ ";
  text[Back]  = "戻る ";
  text[Save]  = "Save";
  text[Load]  = "Load";

  dif_text[dif::EASY]   = "EASY";
  dif_text[dif::NORMAL] = "NORMAL";
  dif_text[dif::HARD]   = "HARD";
}


void d_select::update() {
  for (i = Slot1; i < Save; ++i) {
    buttonPosTranslate(i == Back ? i + 1 : i);
    on_mouse_ = rectOnMouse(pos_, slot_size_);

    if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) { slotSelect(); }
  }
}


void d_select::slotSelect() {
  data::music.sePlay(se::Click);

  switch (i) {
    case Back: next_scene_ = last_scene_; break;
    default:
      if (now_scene_ == scene::Save) { Slot::saveUserData(i); return; }

      Slot::loadUserData(i);
      next_scene_ = scene::Strategy;
  }
}


void d_select::init() {
  data::music.stop();
  data::music.play(bgm::DataSelect);
}


void d_select::draw() {
  disp_back();
  disp_window();
  disp_slot();
}


void d_select::disp_back() {
  drawTextureBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    0, 0, size::Rect_512, size::Rect_512,
    bg_, win::color(paint::White));

  text_id_ = now_scene_ == scene::Save ? Save : Load;
  pos_.x() = -telop_.drawSize(text[text_id_]).x() / 2;
  pos_.y() = size::Result_Y;
  telop_.draw(text[text_id_], pos_,
    win::color(text_id_ == Save ? paint::Yellow : paint::Green));
}


// セーブデータの情報
void d_select::disp_window() {
  // TIPS: ウィンドウだけ描画
  win::draw(Vec2f(size::InfoPos_X, size::InfoPos_Y),
    Vec2f(size::Info_W, size::Info_H), win::color(paint::Gray, 0.75f));
}


void d_select::disp_slot() {
  for (i = Slot1; i < Save; ++i) {
    buttonPosTranslate(i == Back ? i + 1 : i);
    on_mouse_ = rectOnMouse(pos_, slot_size_);

    win::draw(pos_, slot_size_, win::color(on_mouse_ ?
      paint::Yellow : paint::Blue, 0.5f));

    pos_.x() += (slot_size_.x() - telop_.drawSize(text[i]).x()) / 2;
    pos_.y() += 15;
    telop_.draw(text[i], pos_, win::color(paint::White));

    if (i == Back) { on_mouse_ = false; }
    if (on_mouse_) { slotState(i) ? disp_info() : disp_empty(); }
  }
}


void d_select::disp_empty() {
  pos_.x() = 90;
  pos_.y() = -40;

  telop_.draw("EMPTY", pos_, win::color(paint::White));
}


void d_select::disp_info() {
  Slot::getSlotData(temp_, i);

  info_dif();
  info_money();
  info_strategy();
  info_unit();
}


void d_select::info_dif() {
  // TIPS: すでにウィンドウは描画済みだが、
  //     : 難易度別にさらに違う色を上から描画する
  short is_dif = temp_.dif_;
  win::draw(Vec2f(size::InfoPos_X, size::InfoPos_Y),
    Vec2f(size::Info_W, size::Info_H),
    win::color(is_dif == dif::EASY ? paint::Green :
    is_dif == dif::NORMAL ? paint::Yellow : paint::Purple, 0.5f));

  pos_.x() = size::SlotData_X;
  pos_.y() = size::SlotData_Y;
  info_.draw("MODE: " + dif_text[temp_.dif_], pos_, win::color(paint::White));

  if (temp_.count.clear_ > 0) {
    pos_.x() += 300;
    info_.draw("★CLEAR★", pos_, win::color(paint::White));
  }
}


void d_select::info_money() {
  pos_.x() = size::SlotData_X;
  pos_.y() = size::SlotData_Y - 50;
  info_.draw("所持金", pos_, win::color(paint::White));

  sstream money;
  money << std::setw(8) << temp_.weapon.money_ << " G";

  pos_.y() -= size::Icon_H * 0.75f;
  info_.draw(money.str(), pos_, win::color(paint::White));
}


void d_select::info_strategy() {
  for (j = 0; j < 4; ++j) {
    pos_.x() = size::SlotData_X + (j % 2 + 1.5f) * 140;
    pos_.y() = size::SlotData_Y - size::Icon_H * (j / 2) * 0.75f - 50;

    drawTextureBox(pos_.x(), pos_.y(), size::Icon_W / 2, size::Icon_H / 2,
      size::Icon_W * j, 0, size::Icon_W, size::Icon_H,
      icon_, win::color(paint::White),
      0, Vec2f(1, 1), Vec2f(0, 10));

    short val;
    switch (j) {
      case 0: val = temp_.weapon.fire_;   break;
      case 1: val = temp_.weapon.cannon_; break;
      case 2: val = temp_.weapon.trap_;   break;
      case 3: val = temp_.weapon.poison_; break;
      default: val = 0;
    }

    pos_.x() += 40;
    sstream temp;
    temp << std::setw(3) << val;
    info_.draw(temp.str(), pos_, win::color(paint::White));
  }
}


void d_select::info_unit() {
  begin_ = temp_.player.begin();
  end_ = temp_.player.end();

  for (it_ = begin_, j = 0; it_ != end_; ++it_, ++j) {
    pos_.x() = size::SlotData_X;
    pos_.y() = size::SlotData_Y - 50 * (j + 2) - 50;

    sstream name;
    name << " LV: " << std::setw(3) << it_->getInfo().level_ << "  " << it_->getInfo().name_;
    info_.draw(name.str(), pos_, win::color(paint::White));
  }
}


void d_select::buttonPosTranslate(const short& i) {
  pos_.x() = size::SlotPos_X;
  pos_.y() = size::SlotPos_Y - (i * size::Slot_H);
}


bool d_select::slotState(const short& slot) {
  switch (slot) {
    case 0: pass_ = "res/data/user_data1.txt"; break;
    case 1: pass_ = "res/data/user_data2.txt"; break;
    case 2: pass_ = "res/data/user_data3.txt"; break;
  }

  std::ifstream load(pass_);

  return load.good();
}
