
#include "funds.h"

typedef cFunds  fund;


cFunds::cFunds() :
telop_(FONT_),
size_(Vec2f(200, size::Command_H)),
mode_(data::system.strategy.mode_),
poli_state_(data::system.strategy.draw_state_),
worker_(data::user.count.worker_),
money_(data::user.weapon.money_),
fire_(data::user.weapon.fire_),
cannon_(data::user.weapon.cannon_),
trap_(data::user.weapon.trap_),
poison_(data::user.weapon.poison_),
pause_(data::system.pause_) {

  telop_.size(fontSize::Small);

  text[Worker]      = "兵士派遣 ";
  text[Sell_Fire]   = "売:火炎瓶 ";
  text[Sell_Bomb]   = "売:砲撃 ";
  text[Sell_Trap]   = "売:罠 ";
  text[Sell_Poison] = "売:毒 ";
  text[Back]        = "戻る ";
};


void fund::update() {
  if (stateChange()) { return; }
  buttonSelect();
}


bool fund::stateChange() {
  switch (draw_state_) {
    case command::MoveIn : moveMode();  break;
    case command::Blink  : blinkMode(); break;
    case command::MoveOut: backMode();  break;

    default: return false;
  }

  return true;
}


void fund::moveMode() {
  for (i = 0; i < All_Text; ++i) {
    // TIPS: 移動が完了したボタンの処理をスキップ
    if (x[i] <= 0.f) { continue; }

    if (i == 0) { x[i] -= 10.f; }
    else if (x[i - 1] <= design::DefaultPos / 1.5f) { x[i] -= 10.f; }
  }
  if (anime.alpha_ < 0.75f) { anime.alpha_ += 0.0125f; }

  // TIPS: ボタンの移動が完了したら選択モードにする
  if (x[Back] <= 0.f) { ++draw_state_; }
}


void fund::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= design::Fps) { ++draw_state_; }
}


void fund::backMode() {
  for (i = 0; i < All_Text; ++i) { x[i] += 10.f; }
  if (anime.alpha_ >= 0.f) { anime.alpha_ -= 0.05f; }
  if (x[Back] >= design::DefaultPos) {
    mode_ = mode::Neutral;
    poli_state_ = command::MoveIn;
    init();
  }
}


void fund::buttonSelect() {
  for (i = 0; i < All_Text; ++i) {
    buttonPosTranslate(i);
    isAbleToSell(i);

    // TIPS: 売却できる状態でなければクリックを許可しない
    if (rectOnMouse(pos_, size_) && is_sell_ &&
        win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click);
      ++draw_state_;
      id_ = i;
      incomeMoney();
    }
  }
}


void fund::incomeMoney() {
  switch (id_) {
    case      Worker: is_worker(); break;
    case   Sell_Fire: is_fire();   break;
    case   Sell_Bomb: is_bomb();   break;
    case   Sell_Trap: is_trap();   break;
    case Sell_Poison: is_poison(); break;

    // 戻るボタンの時は何もしない
    default:;
  }
}


void fund::is_worker() {
  begin = data::user.player.begin();
  end = data::user.player.end();

  short payment = 0;
  for (it = begin; it != end; ++it) { payment += it->getHp().max_ * 0.5f; }
  money_ += payment;
  ++worker_;
}


void fund::is_fire() {
  money_ += price::Fire / 2;
  --fire_;
}


void fund::is_bomb() {
  money_ += price::Bomb / 2;
  --cannon_;
}


void fund::is_trap() {
  money_ += price::Trap / 2;
  --trap_;
}


void fund::is_poison() {
  money_ += price::Poison / 2;
  --poison_;
}


void fund::init() {
  buttonPosInit();

  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  draw_state_ = command::MoveIn;
  id_ = -1;
}


// TIPS: ボタンのみ描画
//     : 背景その他はまとめて politics 画面で描画処理を行っている
void fund::draw() {
  for (i = 0; i < All_Text; ++i) {
    buttonPosTranslate(i);
    isAbleToSell(i);

    on_mouse_ = rectOnMouse(pos_, size_);
    if (draw_state_ != command::Select || pause_) { on_mouse_ = false; }
    win::draw(pos_, size_, win::color(!is_sell_ ? paint::Gray :
      on_mouse_ ? paint::Orange : paint::Blue,
      id_ == i ? (anime.blink_ / 2) % 2 : anime.alpha_), anime.alpha_ * 1.25f);

    pos_.x() += (size_.x() - telop_.drawSize(text[i]).x()) / 2;
    pos_.y() += 15;
    telop_.draw(text[i], pos_, win::color(paint::White,
      id_ == i ? (anime.blink_ / 2) % 2 : 1.f));

    if (on_mouse_) { disp_telop(i); }
  }
}


void fund::isAbleToSell(const short& i) {
  switch (i) {
    case      Worker: is_sell_ = isWorking(); break;
    case   Sell_Fire: is_sell_ = fire_   > 0; break;
    case   Sell_Bomb: is_sell_ = cannon_ > 0; break;
    case   Sell_Trap: is_sell_ = trap_   > 0; break;
    case Sell_Poison: is_sell_ = poison_ > 0; break;

    // TIPS: まとめて判定処理をしている関係で、
    //     : 戻るボタンまで使えなくなるので、常にクリック可能にしておく
    default: is_sell_ = true;
  };
}


void fund::buttonPosTranslate(const short& i) {
  pos_.x() = size::CommandPos_X - x[i];
  pos_.y() = size::CommandPos_Y - (i + 1.5f) * (size_.y() + 10);
}


void fund::buttonPosInit() {
  for (i = 0; i < All_Text; ++i) { x[i] = design::DefaultPos; }
}


void fund::disp_telop(const short& id) {
  std::string text_;
  sstream num;

  pos_.x() = size::BottomPos_X + 20;
  pos_.y() = size::BottomPos_Y + 85;

  // TIPS: 各ボタンの解説
  switch (id) {
    case Worker:
      text_ = is_sell_ ?
        "戦闘以外の活動で給料を得ます" : "これ以上活動できません";
      telop_.draw(text_, pos_,
        win::color(is_sell_ ? paint::White : paint::Pink));

      text_ = "収入：全員のHPMAX 50%  残り回数：";
      pos_.y() -= 60;
      num << text_ << std::setw(3) << data::getTerritory() - worker_;
      telop_.draw(num.str(), pos_, win::color(paint::White));
      break;

    case Sell_Fire:
      text_ = is_sell_ ?
        "火炎瓶を売却します：500 G" : "火炎瓶を所持していません。";
      telop_.draw(text_, pos_,
        win::color(is_sell_ ? paint::White : paint::Pink));
      break;

    case Sell_Bomb:
      text_ = is_sell_ ?
        "砲台を売却します： 1500 G" : "砲台を所持していません。";
      telop_.draw(text_, pos_,
        win::color(is_sell_ ? paint::White : paint::Pink));
      break;

    case Sell_Trap:
      text_ = is_sell_ ?
        "罠を売却します：    400 G" : "罠を所持していません。";
      telop_.draw(text_, pos_,
        win::color(is_sell_ ? paint::White : paint::Pink));
      break;

    case Sell_Poison:
      text_ = is_sell_ ?
        "毒矢を売却します：  750 G" : "毒矢を所持していません。";
      telop_.draw(text_, pos_,
        win::color(is_sell_ ? paint::White : paint::Pink));
      break;

    // 戻るボタンは解説不要
    default:;
  }
}


// TIPS: コマンドが使用可能か検証、不可能ならコマンドを使えないようにする
bool fund::isWorking() {
  return worker_ < data::getTerritory();
}
