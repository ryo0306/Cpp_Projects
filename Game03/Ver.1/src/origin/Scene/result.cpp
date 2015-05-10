
#include "result.h"

typedef cResult  result;


cResult::cResult() :
bg_(Texture("res/png/back_data_select.png")),
telop_(FONT_),
info_(FONT_),
size_(Vec2f(size::Small_W, size::Small_H)),
next_scene_(data::system.scene[play::Next]),
phase_(data::system.phase_),
stage_id_(data::system.strategy.stage_id_),
ending_(data::system.clear_),
money_(data::user.weapon.money_),
battle_(data::user.count.battle_),
command_(data::user.count.command_),
dif_(data::user.dif_),
command_count_(0) {

  telop_.size(fontSize::Large);
  info_.size(fontSize::Small);

  text[Result] = "Result";
  text[Win]    = "Win";
  text[Lose]   = "Lose";
  text[Clear]  = "Congratulation !!";
  text[Telop1] = "すべての地域を制圧しました ";
  text[Telop2] = "NewGame でデータを引き継いで最初から遊べます ";
  text[Telop3] = "再開した後、必ずセーブしてください ";
}


void result::update() {
  if (stateChange()) { return; }

  pos_.x() = -size_.x() / 2;
  pos_.y() = -size::Half_H + size_.y() * 1.5f;

  on_mouse_ = rectOnMouse(pos_, size_);
  if (on_mouse_ && win::app->isPushButton(Mouse::LEFT)) {
    data::music.sePlay(se::Click);
    ++draw_state_;
    money_ += guarantee_;
    data::user.count.worker_ = 0;
  }
}


bool result::stateChange() {
  switch (draw_state_) {
    case  FadeIn: fadeInMode();  break;
    case   Blink: blinkMode();   break;
    case FadeOut: fadeOutMode(); break;

    default: return false;
  }

  return true;
}


void result::fadeInMode() {
  // TIPS: フェードイン処理が終わったら操作を受け付けるようにする
  anime.alpha_ += 0.05f;
  if (anime.alpha_ >= 1.f) { ++draw_state_; }
}


void result::blinkMode() {
  ++anime.blink_;
  if (anime.blink_ >= design::Fps) { ++draw_state_; }
}


void result::fadeOutMode() {
  if (anime.alpha_ > 0.f) { anime.alpha_ -= 0.05f; return; }

  command_count_ = command_;
  if (result_) { data::user.map_info[stage_id_].flag_ = true; }
  next_scene_ = ending_ ? scene::Title : scene::Strategy;
}


void result::init() {
  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  draw_state_ = FadeIn;
  result_ = phase_ == phase::Win;

  // TIPS: 敗北ならフラグを ON にして次回から報酬半減
  if (!result_) {
    data::user.map_info[stage_id_].lose_ = true;
  }

  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin; it != end; ++it) {
    it->battleFinish();
  }

  levelAverage();

  ending_ = data::getTerritory() == design::AllStage;

  // TIPS: エンディングならクリアランクの計算を行う
  if (ending_) {
    ++data::user.count.clear_;
    guarantee_ = guara_base_;
    rank_ = (battle_ - design::AllStage) * 10 + (command_ - (dif_ + 1) * 5);
  }

  musicPlay();
}


// TIPS: 戦闘結果の勝敗、エンディングを判定、対応した BGM を再生する
void result::musicPlay() {
  data::music.stop();

  // FIXME: エンディングBGM は後日差し替え
  data::music.play(ending_ ? bgm::Result_Win :
    result_ ? bgm::Result_Win : bgm::Result_Lose);
}


void result::levelAverage() {
  lv_average.player_ = 0;
  lv_average.enemy_ = 0;

  std::list<cCharacter>::iterator it, begin, end;

  // TIPS: プレイヤー側の平均レベルを取得
  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin; it != end; ++it) {
    lv_average.player_ += it->getInfo().level_;
  }
  lv_average.player_ /= data::user.player.size();
  if (lv_average.player_ < 1) { lv_average.player_ = 1; }

  // TIPS: 敵キャラの平均レベルを取得
  //     : 獲得金額のベースは合計レベルを基に取得する
  begin = data::system.all_enemy.begin();
  end = data::system.all_enemy.end();
  for (it = begin; it != end; ++it) {
    lv_average.enemy_ += it->getInfo().level_;
  }

  guara_base_ = lv_average.enemy_ * design::Score;

  lv_average.enemy_ /= data::system.all_enemy.size();
  if (lv_average.enemy_ < 1) { lv_average.enemy_ = 1; }
}


void result::draw() {
  disp_back();
  disp_button();
  ending_ ? disp_clear() : disp_score();
}


void result::disp_back() {

  // 背景
  drawTextureBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    0, 0, 512, 512,
    bg_, win::color(paint::White, draw_state_ != FadeOut ? 1.f : anime.alpha_));

  ending_ ? disp_ending() : disp_logo();
}


void result::disp_logo() {
  pos_ = Vec2f(-telop_.drawSize(text[Result]).x() * 1.1f, size::Result_Y);
  telop_.draw(text[Result], pos_,
    win::color(paint::White, draw_state_ != FadeOut ? 1.f : anime.alpha_));

  // 勝敗
  pos_.x() = telop_.drawSize(result_ ? text[Win] : text[Lose]).x() / 2;
  telop_.draw(result_ ? text[Win] : text[Lose], pos_,
    win::color(result_ ? paint::Yellow : paint::Pink, anime.alpha_));
}


void result::disp_ending() {
  pos_.x() = -telop_.drawSize(text[Clear]).x() / 2;
  pos_.y() = size::Result_Y;
  telop_.draw(text[Clear], pos_, win::color(paint::Yellow, anime.alpha_));

  pos_.x() = -info_.drawSize(text[Telop1]).x() / 2;
  pos_.y() -= 50;
  info_.draw(text[Telop1], pos_, win::color(paint::Yellow, anime.alpha_));
}


// ステージクリアの評価
// TIPS: 実際の評価は初期化処理で終了済み
void result::disp_score() {
  pos_.x() = size::ScorePos_X;
  pos_.y() = size::ScorePos_Y;

  bool& my_territory = data::user.map_info[stage_id_].flag_;
  bool& loser = data::user.map_info[stage_id_].lose_;

  // 獲得金額（ベース）
  sstream money_base;
  money_base << "　　　獲得賞金：  " << std::setw(10) << guara_base_ << " G";
  info_.draw(money_base.str(), pos_, win::color(paint::White, anime.alpha_));

  // レベルの差によるボーナス
  pos_.y() -= 80;
  short lv_diff = short(lv_average.enemy_ - lv_average.player_);
  sstream level_bonus;
  level_bonus << "戦力差(平均LV)： + (10 + " << std::setw(3) << lv_diff << ") × 50";
  info_.draw(level_bonus.str(), pos_, win::color(paint::White, anime.alpha_));

  // コマンド使用回数によるボーナス
  pos_.y() -= 50;
  int cmd_bonus = command_ - command_count_;
  sstream count;
  count << "　コマンド回数： + 　　　" << std::setw(3) << cmd_bonus << "  × -100";
  info_.draw(count.str(), pos_, win::color(paint::White, anime.alpha_));

  // 結果によるボーナス
  // TIPS: 敗北したときのみ表示
  int temp = guara_base_ + lv_diff * 50 - cmd_bonus * 100;
  if (temp < 0) { temp = 0; }
  guarantee_ = temp;
  pos_.y() -= 50;
  my_territory ? disp_training(temp) : disp_loser(temp, loser);

  // 獲得金額（結果）
  pos_.y() -= 80;
  sstream money_result;
  money_result << "　　　　　収入：  " << std::setw(10) << guarantee_ << " G";
  info_.draw(money_result.str(), pos_, win::color(paint::White, anime.alpha_));
}


void result::disp_loser(const int& temp, const bool& loser) {
  if (!loser) { return; }

  std::string lose_text;
  lose_text = result_ ? "　　　　再挑戦：  " : "　　　　　敗北：  ";
  sstream lose_ratio;
  lose_ratio << lose_text << std::setw(10) << temp << "  × 0.5";
  info_.draw(lose_ratio.str(), pos_, win::color(paint::Pink, anime.alpha_));

  guarantee_ *= 0.5f;
}


void result::disp_training(const int& temp) {
  sstream training;
  training << "　　　　　訓練：  " << std::setw(10) << temp << "  × 0.5";
  info_.draw(training.str(), pos_, win::color(paint::Pink, anime.alpha_));

  guarantee_ *= 0.5f;
}


// ゲームクリア
void result::disp_clear() {
  pos_.x() = -size::Half_W / 2;
  pos_.y() = size::HEIGHT / 6 - 10;

  // 戦闘回数
  sstream battle;
  battle << "　　　　戦闘回数： " << std::setw(4) << battle_;
  info_.draw(battle.str(), pos_, win::color(paint::White, anime.alpha_));

  // 政策コマンド回数
  pos_.y() -= 50;
  sstream command;
  command << "戦略コマンド回数： " << std::setw(4) << command_;
  info_.draw(command.str(), pos_, win::color(paint::White, anime.alpha_));

  // クリアランク
  pos_.y() -= 75;
  info_.draw("　　クリアランク：", pos_,
    win::color(paint::White, anime.alpha_));
  pos_.x() = 150;
  info_.draw(getRank(), pos_,
    win::color(getRankColor(), anime.alpha_));

  // 周回プレイの説明
  pos_.x() = -info_.drawSize(text[Telop2]).x() / 2;
  pos_.y() -= 75;
  info_.draw(text[Telop2], pos_, win::color(paint::White, anime.alpha_));

  // 警告
  pos_ = Vec2f(-360, pos_.y() - 80);
  win::draw(pos_, Vec2f(-pos_.x() * 2, size::Command_H),
    win::color(paint::Red, anime.alpha_ * 0.75f), anime.alpha_);

  pos_.x() = -info_.drawSize(text[Telop3]).x() / 2;
  pos_.y() += 15;
  info_.draw(text[Telop3], pos_, win::color(paint::White, anime.alpha_));
}


void result::disp_button() {
  pos_.x() = -size_.x() / 2;
  pos_.y() = -size::Half_H + size_.y() * 1.5f;

  on_mouse_ = rectOnMouse(pos_, size_);
  if (draw_state_ != command::Select) { on_mouse_ = false; }
  win::draw(pos_, size_,
    win::color(on_mouse_ ? paint::Orange : paint::Blue,
    draw_state_ == Blink ? (anime.blink_ / 2) % 2 : anime.alpha_ * 0.75f),
    draw_state_ == Blink ? (anime.blink_ / 2) % 2 : anime.alpha_);

  std::string back = "戻る ";
  pos_.x() = -info_.drawSize(back).x() / 2;
  pos_.y() += 15;
  info_.draw(back, pos_, win::color(paint::White,
    draw_state_ == Blink ? (anime.blink_ / 2) % 2 : anime.alpha_));
}


std::string result::getRank() {
  return
    rank_ < rank::S ? "Ｓ" :
    rank_ < rank::A ? "Ａ" :
    rank_ < rank::B ? "Ｂ" : "Ｃ";
}


short result::getRankColor() {
  return
    rank_ < rank::S ? paint::Yellow :
    rank_ < rank::A ? paint::Pink :
    rank_ < rank::B ? paint::Green : paint::Purple;
}
