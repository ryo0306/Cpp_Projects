
#include "strategy.h"
#include "../../lib/utils.hpp"

typedef cStrategy  strategy;
using namespace literal_effect;
using namespace literal_strategy;


cStrategy::cStrategy() :
telop_(FONT_STRING),
info_(FONT_STRING),
back_(Texture("res/png/back_title.png")),
back_sea_(Texture("res/png/world_sea.png")),
back_fog_(Texture("res/png/world_sea_fog.png")),
back_cld_(Texture("res/png/world_fog.png")),
map_icon_(Texture("res/png/world_icon.png")),
info_icon_(Texture("res/png/battle_icon.png")),
pause_(data::system.pause_),
now_scene_(data::system.scene_[play::Now]),
next_scene_(data::system.scene_[play::Next]),
last_scene_(data::system.scene_[play::Last]),
draw_state_(data::system.strategy_state.draw_),
mode_state_(data::system.strategy_state.mode_) {

  telop_.size(font::Standard);
  info_.size(font::Info);

  const std::string filePass[] = {
    "res/png/map_01.png",
    "res/png/map_02.png",
    "res/png/map_03.png",
    "res/png/map_04.png",
    "res/png/map_05.png",
    "res/png/map_06.png",
    "res/png/map_07.png",
    "res/png/map_08.png",
    "res/png/map_09.png",
    "res/png/map_10.png",
    "res/png/map_11.png",
    "res/png/map_12.png",
  };

  for (index_ = 0; index_ < elemsof(filePass); ++index_) {
    back_chip_[index_] = Texture(filePass[index_]);
  }

  const std::string tempData_1[] = {
    "資金繰り ",
    "開発 ",
    "軍備 ",
    "侵攻 ",
    "データ ",
  };

  for (index_ = 0; index_ < elemsof(tempData_1); ++index_) {
    command_[index_] = tempData_1[index_];
  }

  const std::string tempData_2[] = {
    "セーブ ",
    "ロード ",
    "戻る ",
  };

  for (index_ = 0; index_ < elemsof(tempData_2); ++index_) {
    saveload_[index_] = tempData_2[index_];
  }

  const std::string command_text[] = {
    "兵士の活動、または兵器を売却して資金を得ます ",
    "戦闘中に使用できる兵器を開発します ",
    "兵士の管理を行います ",
    "敵の領地（赤）に向けて侵攻します ",
    "データの保存、読込を行います ",
  };

  short temp_index = 0;
  for (auto& it : descript_command_) {
    it = command_text[temp_index];
    ++temp_index;
  }

  const std::string saveload_text[] = {
    "現在のデータを保存します ",
    "保存された状況から再開します ",
    "コマンド選択に戻ります ",
  };

  temp_index = 0;
  for (auto& it : descript_saveload_) {
    it = saveload_text[temp_index];
    ++temp_index;
  }

  const sDescriptionText temp_text = {
    "※左クリックで決定、右クリックでポーズ画面 ",
    "※敵領地（赤）が無くなったらゲームクリア！ ",
    "",
  };
  description_ = temp_text;
}


void strategy::update() {
  // TIPS: 何らかの演出中なら入力を許可しない
  if (stateChange()) { return; }

  switch (mode_state_) {
    case s_Funds: funds_.update(); break;
    //case s_Develop: develop_.update(); break;
    //case s_Armaments: armaments_.update(); break;
    //case s_Invasion: invasion_.update(); break;
    default: commandSelect();
  }
}


// TIPS: ボタンがアニメーションしてない時のみ、入力を許可できるようにする
bool strategy::stateChange() {
  switch (draw_state_) {
    case MoveIn_1:;
    case MoveIn_2:
      moveMode(); break;

    case Blink_1:;
    case Blink_2:
      blinkMode(); break;

    case MoveOut_1:;
    case MoveOut_2:
      backMode(); break;

    default: return false;
  }

  return true;
}


void strategy::moveMode() {
  for (index_ = 0; index_ < str::All_Command; ++index_) {
    // TIPS: 移動が完了したボタンの処理をスキップ
    if (move_x_[index_] >= 0.0f) { continue; }

    if ((index_ > 0 && move_x_[index_ - 1] > MoveStartPos) || index_ == 0) {
      move_x_[index_] += buttonMoveSpeed;
    }
  }
  if (anime.alpha_ < 0.75f) { anime.alpha_ += buttonAlphaSpeed / 2; }

  // TIPS: ボタンの移動が完了したら選択モードにする
  if (move_x_[s_SaveLoad] >= 0.0f) { ++draw_state_; }
}


void strategy::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= spec::Fps) { ++draw_state_; }
}


void strategy::backMode() {
  for (auto& it : move_x_) {
    if (it > DefaultCommandPos) { it -= buttonMoveSpeed; }
  }

  if (anime.alpha_ > 0.0f) {
    anime.alpha_ -= buttonAlphaSpeed;
    return;
  }

  buttonPosInit();
  commandChange();
}


void strategy::commandSelect() {
  const bool saveload = mode_state_ == s_SaveLoad;

  for (index_ = 0; index_ < (saveload ? str::All_Select : str::All_Command); ++index_) {
    buttonPosTranslate(index_);
    on_mouse_ = env::rectOnMouse(pos_, buttonSize);

    if (on_mouse_ && env::leftClick()) {
      click_id_ = index_;
      ++draw_state_;
      data::media.sePlay(se::Click);
    }
  }
}


void strategy::commandChange() {
  mode_state_ == s_SaveLoad ? shiftSelectMode() : shiftCommandMode();
}


void strategy::shiftCommandMode() {
  mode_state_ = click_id_;

  switch (mode_state_) {
    case s_Funds: funds_.init(); break;
    //case s_Develop: develop_.init(); break;
    //case s_Armaments: armaments_.init(); break;
    //case s_Invasion: invasion_.init(); break;
    default:;
  }

  anime.blink_ = 0;
  click_id_ = s_Neutral;
  draw_state_ = mode_state_ == s_SaveLoad ? MoveIn_2 : Wait;
}


void strategy::shiftSelectMode() {
  switch (click_id_) {
    case Save: next_scene_ = scene::Save; return;
    case Load: next_scene_ = scene::Load; return;

    // TIPS: 戻るボタン
    default: click_id_ = s_Neutral;
  }

  mode_state_ = click_id_;
  anime.blink_ = 0;
  draw_state_ = MoveIn_1;
}


void strategy::init() {
  anime.blink_ = 0;
  anime.alpha_ = 0.0f;
  anime.scroll_ = 0.0f;

  descript_time_ = descriptDrawTime;

  buttonPosInit();

  draw_state_ = MoveIn_1;
  mode_state_ = s_Neutral;
  click_id_ = s_Neutral;

  last_scene_ = scene::Strategy;

  data::media.musicStop();
  data::media.musicPlay(bgm::Strategy);
}


void strategy::draw() {
  effectPlay();
  disp_back();
  disp_mode();
  disp_window();
  disp_command();

  switch (mode_state_) {
    case s_Funds: funds_.draw(); break;
    //case s_Develop: develop_.draw(); break;
    //case s_Armaments: armaments_.draw(); break;
    //case s_Invasion: invasion_.draw(); break;
    default:;
  }

  // TIPS: 侵攻モードのときは情報ウィンドウを表示しない
  if (mode_state_ == s_Invasion) { return; }

  disp_countInfo();
  //disp_unitInfo();
}


void strategy::effectPlay() {
  anime.scroll_ += scrollSpeed;
  if (anime.scroll_ >= ScrollEndPoint) { anime.scroll_ = 0.0f; }
}


void strategy::disp_back() {

  // TIPS: 色合いが気に入らないので、最背面に別の色をこっそり描画
  drawFillBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
              draw::color(paint::White));

  // 海
  drawTextureBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
                 anime.scroll_ * 2, 0, Sea_Fog_WH, Sea_Fog_WH,
                 back_sea_,
                 draw::color(paint::White, backAlpha));

  // 波
  drawTextureBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
                 anime.scroll_, 0, Sea_Fog_WH, Sea_Fog_WH,
                 back_fog_,
                 draw::color(paint::SkyBlue, backAlpha));

  // 大陸
  drawTextureBox(0, 0, WIDTH, HEIGHT, 0, 0, Map_WH, Map_WH,
                 back_, draw::color(paint::Black),
                 0, mapScale, centerTranslate);
  for (index_ = 0; index_ < spec::All_Stage; ++index_) {
    territory_ = data::user.map_info_[index_].territory_;

    drawTextureBox(0, 0, WIDTH, HEIGHT, 0, 0, Map_WH, Map_WH,
                   back_chip_[index_],
                   draw::color(territory_ ? paint::SkyBlue : paint::Pink),
                   0, defaultScale, centerTranslate);
  }

  // 雲
  drawTextureBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
                 anime.scroll_, 0, Sea_Fog_WH, Sea_Fog_WH,
                 back_cld_, draw::color(paint::White, backAlpha));
}


// TIPS: 現在選択中のコマンドを表示
void strategy::disp_mode() {
  pos_ = Vec2f(CommandPos_X, ModePos_Y);
  draw::window(pos_, buttonSize,
               draw::color(paint::Brown, buttonAlpha));

  const std::string text
    = mode_state_ == s_Neutral ? "選択 " : command_[mode_state_];

  const float StringPos_X
    = (buttonSize.x() - telop_.drawSize(text).x()) / 2;

  pos_.x() += StringPos_X;
  pos_.y() += StringPos_Y;
  telop_.draw(text, pos_, draw::color(paint::White));
}


void strategy::disp_window() {

  // TIPS: ウィンドウのみを表示
  //     : 実際に表示するデータ、テロップは別の関数で処理
  draw::window(Vec2f(BtmWindowPos_X, BtmWindowPos_Y),
               Vec2f(BtmWindow_W, BtmWindow_H),
               draw::color(paint::DarkGray, backAlpha));

  // TIPS: 侵攻モードのときは情報ウィンドウを表示しない
  if (mode_state_ == s_Invasion) { return; }

  draw::window(Vec2f(InfoWindowPos_X, InfoWindowPos_Y),
               Vec2f(InfoWindow_W, InfoWindow_H),
               draw::color(paint::DarkGray, backAlpha));
}


void strategy::disp_countInfo() {

  const std::string telop[] = {
    "　所持金: ", " Ｇ ",
    "　　派遣: あと ", " 回 ",
    "戦闘回数: ", " 回 ",
    "　(撤退): ", " 回 ",
  };

  // TIPS: 所持金、戦闘回数などのカウント表示
  for (index_ = 0; index_ < All_CountInfo; ++index_) {
    pos_.x() = DataPos_X;
    pos_.y() = DataPos_Y - Data_H * index_;

    short temp_val;
    switch (index_) {
      case i_Money : temp_val = data::user.assets.money_; break;
      case i_Food  : temp_val = data::user.assets.food_;  break;
      case i_Battle: temp_val = data::user.cnt.battle_;   break;
      case i_Lose  : temp_val = data::user.cnt.lose_;     break;
      default:;
    }

    short width = index_ == i_Food ? 4 : 9;
    sstream temp;
    temp
      << telop[index_ * 2] << std::setw(width)
      << temp_val << telop[index_ * 2 + 1];
    info_.draw(temp.str(), pos_, draw::color(paint::White));
  }

  const Vec2f iconScale = Vec2f(0.5f, 0.5f);
  const std::string icon_name[] = {
    "火: ", "砲: ", "罠: ", "毒: ",
  };

  // TIPS: 兵器のカウント表示
  for (index_ = 0; index_ < All_WeaponInfo; ++index_) {
    pos_.x() = DataPos_X + Data_W;
    pos_.y() = DataPos_Y - Data_H * index_;

    drawTextureBox(pos_.x(), pos_.y(), iconSize.x(), iconSize.y(),
                   iconSize.x()*index_, 0, iconSize.x(), iconSize.y(),
                   info_icon_, draw::color(paint::White),
                   0, iconScale, iconPosTranslate);

    short temp_val;
    switch (index_) {
      case i_Fire  : temp_val = data::user.weapon_cnt.fire_;   break;
      case i_Bomb  : temp_val = data::user.weapon_cnt.bomb_;   break;
      case i_Trap  : temp_val = data::user.weapon_cnt.trap_;   break;
      case i_Poison: temp_val = data::user.weapon_cnt.poison_; break;
      default:;
    }

    sstream temp;
    temp << icon_name[index_] << std::setw(3) << temp_val << " ";

    pos_.x() += WeaponCount_X;
    info_.draw(temp.str(), pos_, draw::color(paint::White));
  }
}


void strategy::disp_unitInfo() {
  short i = 0;
  for (auto it : data::user.player) {
    pos_.x() = UnitInfoPos_X;
    pos_.y() = UnitInfoPos_Y - Data_H * i;

    ++i;
  }
}


void strategy::disp_command() {
  const bool saveload = mode_state_ == s_SaveLoad;

  for (index_ = 0; index_ < (saveload ? str::All_Select : str::All_Command); ++index_) {
    buttonPosTranslate(index_);
    on_mouse_ = env::rectOnMouse(pos_, buttonSize);

    // TIPS: 演出中はボタンの色を変えない
    if (drawStateCheck()) { on_mouse_ = false; }

    draw::window(pos_, buttonSize,
                 draw::color(on_mouse_ ? paint::Orange : paint::Blue,
                 click_id_ == index_ ?
                 (anime.blink_ / blinkSpeed) % 2 : anime.alpha_),
                 anime.alpha_ + buttonFrameAlpha);

    const std::string text
      = saveload ? saveload_[index_] : command_[index_];

    const float StringPos_X
      = (buttonSize.x() - telop_.drawSize(text).x()) / 2;

    pos_.x() += StringPos_X;
    pos_.y() += StringPos_Y;
    telop_.draw(text, pos_,
                draw::color(paint::White,
                anime.alpha_ + buttonFrameAlpha));

    if (on_mouse_) { disp_telop(index_); descript_time_ = 0; }
  }

  ++descript_time_;
  if (descript_time_ > descriptDrawTime) { disp_description(); }
}


void strategy::disp_telop(const short index) {
  std::string temp_text = mode_state_ == s_SaveLoad ?
    descript_saveload_[index] : descript_command_[index];

  pos_ = Vec2f(DescriptPos_X, DescriptPos_Y);
  telop_.draw(temp_text, pos_, draw::color(paint::White));
}


void strategy::disp_description() {
  if (draw_state_ != Select_1&&draw_state_ != Select_2) { return; }

  pos_ = Vec2f(DescriptPos_X, DescriptPos_Y);
  telop_.draw(description_.first_, pos_, draw::color(paint::White));

  pos_.y() -= DescriptLine_H;
  telop_.draw(description_.second_, pos_, draw::color(paint::White));
}


void strategy::buttonPosTranslate(const short index) {
  pos_.x() = CommandPos_X + move_x_[index];
  pos_.y() = CommandPos_Y - Command_H * index;
}


void strategy::buttonPosInit() {
  for (auto& it : move_x_) {
    it = DefaultCommandPos;
  }
}


bool strategy::drawStateCheck() {
  return
    (draw_state_ != Select_1 && draw_state_ != Select_2) || pause_;
}
