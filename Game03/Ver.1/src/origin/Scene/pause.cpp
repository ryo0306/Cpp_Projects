
#include "pause.h"

typedef cPause  pause;


cPause::cPause() :
icon_(Texture("res/png/battle_icon.png")),
telop_(FONT_),
size_(Vec2f(350, 60)),
icon_size_(Vec2f(size::Icon_W * 1.5f, size::Icon_H * 1.5f)),
now_scene_(data::system.scene[play::Now]),
next_scene_(data::system.scene[play::Next]),
fire_(data::system.command.fire_),
cannon_(data::system.command.cannon_),
trap_(data::system.command.trap_),
poison_(data::system.command.poison_),
cnt_command_(data::user.count.command_),
cnt_fire_(data::user.weapon.fire_),
cnt_cannon_(data::user.weapon.cannon_),
cnt_trap_(data::user.weapon.trap_),
cnt_poison_(data::user.weapon.poison_),
pause_(data::system.pause_) {

  telop_.size(fontSize::Small);

  text[Pause]     = "PAUSE";
  text[Button_1]  = "再開 ";
  text[Button_2]  = "タイトルに戻る ";
  text[Attention] = "データは保存されません。よろしいですか？ ";
  text[OK]        = "はい ";
  text[NG]        = "いいえ ";
}


bool pause::update() {
  // TIPS: 政策、戦闘以外のシーンではポーズ操作を許可しない
  if (now_scene_ != scene::Strategy &&
    now_scene_ != scene::Battle) { return false; }

  // TIPS: 右クリックでポーズ画面にする
  if (win::app->isPushButton(Mouse::RIGHT)) {
    data::music.sePlay(se::Click_Menu);
    pause_ = !pause_;
  }

  // TIPS: ポーズ画面でなければ、ポーズ画面の操作を許可しない
  if (!pause_) { state_ = Select; return pause_; }

  now_scene_ == scene::Strategy ? pauseMenu() : battleMenu();
  return pause_;
}


// TIPS: 単純なポーズ画面の処理
void pause::pauseMenu() {
  for (i = 0; i < 2; ++i) {
    buttonPosTranslate(i);

    on_mouse_ = rectOnMouse(pos_, size_);
    if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click_Menu);
      commandShift(i);
    }
  }
}


void pause::commandShift(const short& i) {
  switch (state_) {
    case  Select: i == 0 ? backToGame()  : ++state_;     break;
    case Clicked: i == 0 ? backToTitle() : backToGame(); break;
    default: state_ = Select;
  }
}


void pause::backToGame() {
  pause_ = false;
  state_ = Select;
}


void pause::backToTitle() {
  backToGame();
  next_scene_ = scene::Title;
}


// TIPS: 戦闘画面のポーズ兼メニュー画面の処理
void pause::battleMenu() {
  for (i = 0; i < All_Icon; ++i) {
    iconPosTranslate(i);
    on_mouse_ =
      rectOnMouse(Vec2f(pos_.x() - size::Icon_W * 0.75f,
                        pos_.y() - size::Icon_H * 0.75f), icon_size_);

    if (on_mouse_ && is_click_ &&
        win::app->isPushButton(Mouse::LEFT)) {
      data::music.sePlay(se::Click_Menu);
      commandAction(i);
    }
  }
}


void pause::commandAction(const short& icon) {
  switch (icon) {
    case c_Fire  : fire_ = true;   break;
    case c_Cannon: cannon_ = true; break;
    case c_Trap  : trap_ = true;   break;
    case c_Poison: poison_ = true; break;

    case c_BackGame : backToGame(); return;
    case c_BackTitle: backToTitle();
    default:;
  }

  ++cnt_command_;
  backToGame();
}


void pause::draw() {
  disp_back();
  now_scene_ == scene::Battle ? sceneBattle() : scenePolitics();
}


// ポーズ画面の描画
void pause::disp_back() {
  drawFillBox(-size::Half_W, -size::Half_H,
    size::WIDTH, size::HEIGHT, win::color(paint::Black, 0.5f));

  pos_.x() = -telop_.drawSize(text[Pause]).x() / 2;
  pos_.y() = size::PausePos_Y;
  telop_.draw(text[Pause], pos_, win::color(paint::White));

  if (state_ == Clicked) {
    win::draw(Vec2f(-420, size::Attension_Y - 15),
      Vec2f(840, 60), win::color(paint::Red, 0.75f));

    pos_.x() = -telop_.drawSize(text[Attention]).x() / 2;
    pos_.y() = size::Attension_Y;
    telop_.draw(text[Attention], pos_, win::color(paint::White));
  }
}


// 政策画面のポーズ表示
void pause::scenePolitics() {
  for (i = 0; i < 2; ++i) {
    buttonPosTranslate(i);

    on_mouse_ = rectOnMouse(pos_, size_);    
    if (on_mouse_) { win::draw(pos_, size_, win::color(paint::Gray, 0.75f)); }
    
    pos_.x() = -telop_.drawSize(text[button_ + i]).x() / 2;
    pos_.y() += 15;
    telop_.draw(text[button_ + i], pos_,
      win::color(on_mouse_ ? paint::Orange : paint::White));
  }
}


// 戦闘画面のポーズ表示
void pause::sceneBattle() {
  disp_window();

  for (i = 0; i < All_Icon; ++i) {
    iconPosTranslate(i);

    // TIPS: 使用可能なコマンドのみ描画状態を変更できるようにする
    on_mouse_ =
      rectOnMouse(Vec2f(pos_.x() - size::Icon_W * 0.75f,
                        pos_.y() - size::Icon_H * 0.75f), icon_size_);
    icon_scale_ = on_mouse_ ? 2.f : 1.5f;

    drawTextureBox(pos_.x(), pos_.y(), size::Icon_W, size::Icon_H,
      size::Icon_W * i, 0, size::Icon_W, size::Icon_H,
      icon_, win::color(is_click_ ? paint::White : paint::Gray),
      0, Vec2f(icon_scale_, icon_scale_),
      Vec2f(size::Icon_W / 2, size::Icon_H / 2));

    if (on_mouse_) { disp_telop(i); }
  }
}


void pause::disp_window() {
  // コマンド解説用のウィンドウ
  // TIPS: とりあえずウィンドウだけを表示
  //     : 各コマンドの処理に入った時、それぞれ別々に表示させる
  win::draw(Vec2f(size::BottomPos_X, size::BottomPos_Y),
    Vec2f(size::Bottom_W, size::Bottom_H),
    win::color(paint::Gray, 0.75f));
}


void pause::disp_telop(const short& icon) {
  std::string text_;
  pos_.x() = size::BottomPos_X + 20;
  pos_.y() = size::BottomPos_Y + 85;

  switch (icon) {
    case c_Fire:
      text_ = is_click_ ?
        "敵１体にダメージ　威力：中" : "使用できません";
      telop_.draw(text_, pos_, win::color(is_click_ ?
        paint::White : paint::Pink));

      if (is_click_) {
        pos_.y() -= 60;
        text_ = "最もHPが残っている敵に対して使用します";
        telop_.draw(text_, pos_, win::color(paint::White));
      }
      break;

    case c_Cannon:
      text_ = is_click_ ?
        "敵全体にダメージ　威力：小" : "使用できません";
      telop_.draw(text_, pos_, win::color(is_click_ ?
        paint::White : paint::Pink));
      break;

    case c_Trap:
      text_ = is_click_ ?
        "一定時間、敵全体の行動を遅くします" : "使用できません";
      telop_.draw(text_, pos_, win::color(is_click_ ?
        paint::White : paint::Pink));
      break;

    case c_Poison:
      text_ = is_click_ ?
        "一定時間、敵全体の攻撃力をダウンさせます" : "使用できません";
      telop_.draw(text_, pos_, win::color(is_click_ ?
        paint::White : paint::Pink));
      break;

    case c_BackGame:
      text_ = "戦闘に戻ります";
      telop_.draw(text_, pos_, win::color(paint::White));
      break;

    case c_BackTitle:
      text_ = "タイトル画面に戻ります";
      telop_.draw(text_, pos_, win::color(paint::White));

      pos_.y() -= 60;
      text_ = "※データは保存されません";
      telop_.draw(text_, pos_, win::color(paint::Pink));
      break;

    default:;
  }
}


void pause::buttonPosTranslate(const short& i) {
  // TIPS: ポーズ画面の状態に対応したボタンの情報を取得
  button_ = state_ == Clicked ? OK : Button_1;

  pos_.x() = -size_.x() / 2;
  pos_.y() = -size::Attension_Y - (i + 0.5f) * (size_.y() + 10);
}


void pause::iconPosTranslate(const short& i) {
  switch (i) {
    case c_Fire  : is_click_ = cnt_fire_ > 0;   break;
    case c_Cannon: is_click_ = cnt_cannon_ > 0; break;
    case c_Trap  : is_click_ = cnt_trap_ > 0;   break;
    case c_Poison: is_click_ = cnt_poison_ > 0; break;
    default: is_click_ = true;
  }

  pos_.x() = (size::Icon_W + 10) * 2.f * (i - 2.5f);
  pos_.y() = 0;
}
