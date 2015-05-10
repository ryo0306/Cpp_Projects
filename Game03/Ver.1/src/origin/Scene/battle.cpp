
#include "battle.h"

typedef cBattle  battle;


cBattle::cBattle() :
telop_(FONT_),
next_scene_(data::system.scene[play::Next]),
phase_(data::system.phase_),
stage_id_(data::system.strategy.stage_id_),
fire_(data::system.command.fire_),
cannon_(data::system.command.cannon_),
trap_(data::system.command.trap_),
poison_(data::system.command.poison_),
cnt_fire_(data::user.weapon.fire_),
cnt_cannon_(data::user.weapon.cannon_),
cnt_trap_(data::user.weapon.trap_),
cnt_poison_(data::user.weapon.poison_),
camera_x_(0.f) {

  telop_.size(fontSize::Battle);
}


void battle::update() {

  // TIPS: 敵か味方が全滅したら戦闘終了
  battleFinish();

  // TIPS: 戦闘終了に関わるカウント値を初期化
  // * 各段階の処理中にカウントを取得、条件を満たしたら戦闘を終了する
  countInit();

  // TIPS: プレイヤー側キャラクターすべての処理
  begin = data::user.player.begin();
  end = data::user.player.end();
  unitUpdate();

  // TIPS: すべての敵の処理
  begin = enemy.begin();
  end = enemy.end();
  unitUpdate();

  // TIPS: 戦闘メニュー画面から使用したコマンドの処理
  fire_command();
  cannon_command();
  trap_command();
  poison_command();
  command_act();
}


// TIPS: 敵が５体以上いる場合、残りの敵をフィールドに出現させる
void battle::unitSummon() {
  phase_ = phase::Standby;

  enemy.clear();
  short num = order.size();
  if (num > design::BattleMember) { num %= design::BattleMember; }
  if (num == 0) { num = design::BattleMember; }

  for (i = 0; i < num; ++i) {
    enemy.emplace_back(order.front());
    order.pop();
  }

  unitPosTranslate();
}


void battle::unitPosTranslate() {
  u_short& map_id = data::user.map_info[stage_id_].id_;
  u_short& effect = data::user.map_info[stage_id_].attribute_;

  // TIPS: プレイヤー側キャラクター位置を初期化
  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin, i = 0; it != end; ++it, ++i) {
    pos_.x() =
      -size::Half_W + (size::Unit_W * 8.5f) - size::Unit_W * ((i + it->getLane()));
    pos_.y() = i * -80 + 80;
    it->posInit(pos_, map_id, effect);
  }

  // TIPS: 敵キャラ位置を初期化
  begin = enemy.begin();
  end = enemy.end();
  for (it = begin, i = 0; it != end; ++it, ++i) {
    pos_.x() =
      size::Half_W - (size::Unit_W * 8.5f) + size::Unit_W * ((i + it->getLane()));
    pos_.y() = i * -80 + 80;
    it->posInit(pos_, map_id, effect);
  }
}


void battle::enemyStandby() {
  begin = data::system.all_enemy.begin();
  end = data::system.all_enemy.end();
  for (it = begin; it != end; ++it) { order.push(*it); }

  unitSummon();
}


// TIPS: 戦闘不能カウントをリセット
void battle::countInit() {
  dead.player_ = 0;
  dead.enemy_ = 0;
}


// TIPS: 戦闘不能のキャラクターをカウント
void battle::deadCount() {
  it->getInfo().side_ ? ++dead.player_ : ++dead.enemy_;
}


void battle::battleFinish() {
  // TIPS: 勝利判定...敵が全滅
  if (dead.enemy_ == enemy.size()) {
    dead.enemy_ = 0;
    order.empty() ? win() : unitSummon();
  }

  // TIPS: 敗北判定...味方が全滅
  if (dead.player_ == data::user.player.size()) {
    phase_ = phase::Lose;
    next_scene_ = scene::Result;
  }
}


void battle::win() {
  phase_ = phase::Win;
  next_scene_ = scene::Result;
}


void battle::unitUpdate() {
  for (it = begin; it != end; ++it) {
    unit_animePlay();

    // TIPS: 戦闘不能状態のキャラクターの処理をスキップ
    if (it->isDead()) { deadCount(); continue; }

    timeCount();
    unit_action();
  }
}


void battle::timeCount() {
  // TIPS: 何らかの戦闘アニメーションを実行中ならカウントをスキップ
  if (it->isAnimePlaying()) { return; }
  it->timeCount();
}


void battle::unit_animePlay() {
  it->animeShift();
  it->animePlay();
}


void battle::unit_action() {
  // TIPS: 待機時間中なら処理をスキップ
  if (it->isWait()) { return; }
  it->timeCountReset();
  it->animeChange(action::Attack);

  // TIPS: 自身のクラスに応じた行動を選択
  it->getInfo().type_ == type::Mage ? magicalAttack()
    : it->getInfo().type_ == type::Priest ? cure()
    : physicalAttack();
}


void battle::physicalAttack() {
  bool attack = false;

  // TIPS: 行動中のキャラに敵対している相手の情報を取得
  it->getInfo().side_ ? getEnemyInfo() : getPlayerInfo();

  for (target = begin_; target != end_; ++target) {
    // TIPS: ターゲットが戦闘不能なら攻撃対象にしない
    if (target->isDead()) { continue; }

    // TIPS: 自身のクラスとターゲットの隊列を検証、攻撃できるかを判定
    //     : 攻撃可能なら判定処理を終了、攻撃する
    if (it->targetLook(target->getLane())) { attack = true; break; }
  }

  // TIPS: 攻撃可能な隊列に敵が見つからなければ、最後列から逆順に攻撃する
  if (!attack) {
    for (--target; target != begin; --target) {
      // TIPS: ターゲットが戦闘不能なら攻撃対象にしない
      if (target->isDead()) { continue; }
      if (!it->targetLook(target->getLane())) { attack = true; break; }
    }
  }

  // TIPS: 命中判定を行い、命中ならダメージを与える
  target->avoidRatio() < data::random.fromZeroToLast(it->hitRatio())
    ? target->physicalDamage(it->getPower(target->getInfo().gender_))
    : target->animeChange(action::Avoid);
}


void battle::magicalAttack() {
  dead.count_ = 0;

  // TIPS: 行動中のキャラに敵対している相手の情報を取得
  it->getInfo().side_ ? getEnemyInfo() : getPlayerInfo();

  // TIPS: 戦闘不能状態のキャラクターの数を取得
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { ++dead.count_; }
  }

  // TIPS: 人数が多いほど、１人あたりのダメージ量が減衰
  //     : マップ効果の異性に対する攻撃力アップのみ無効
  u_short temp_damage = it->getPower(2) / (i - dead.count_);

  // TIPS: 戦闘不能なキャラ以外の全体にダメージを与える
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { continue; }
    target->magicalDamage(temp_damage);
  }
}


void battle::cure() {
  dead.count_ = 0;

  // TIPS: 味方の情報を取得
  it->getInfo().side_ ? getPlayerInfo() : getEnemyInfo();

  // TIPS: 戦闘不能状態のキャラクターの数を取得
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { ++dead.count_; }
  }

  // TIPS: 人数が多いほど、１人あたりの回復量が減衰
  //     : マップ効果の異性に対する効果アップのみ無効
  u_short temp_recover = it->getPower(2) / (i - dead.count_);

  // TIPS: 戦闘不能なキャラ以外を回復
  for (target = begin_, i = 0; target != end_; ++target, ++i) {
    if (target->isDead()) { continue; }
    target->recovery(temp_recover);
  }
}


void battle::fire_command() {
  // TIPS: 戦闘メニューからコマンド実行されるまで処理を実行しない
  if (!fire_) { return; }

  short temp_hp = 0;
  std::list<cCharacter>::iterator hit, target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: 最も残りHPの高い敵を検索
  for (target0 = begin0; target0 != end0; ++target0) {
    if (temp_hp < target0->getHp().now_) {
      temp_hp = target0->getHp().now_;
      hit = target0;
    }
  }

  // TIPS: 最大HPに応じた、割合ダメージを与える
  //     : 物理ダメージとして扱う
  hit->physicalDamage(hit->getHp().max_ * 0.5f);
}


void battle::cannon_command() {
  // TIPS: 戦闘メニューからコマンド実行されるまで処理を実行しない
  if (!cannon_) { return; }

  dead.count_ = 0;
  std::list<cCharacter>::iterator target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: 戦闘不能ではない敵の数を取得
  for (target0 = begin0; target0 != end0; ++target0) {
    if (target0->isDead()) { ++dead.count_; }
  }

  // TIPS: 人数に応じたダメージの割合を取得
  float temp_per = 6 - enemy.size() + dead.count_;
  temp_per *= 0.1f;

  // TIPS: 敵全体に最大HPに応じた、割合ダメージを与える
  //     : 物理ダメージとして扱う
  for (target0 = begin0; target0 != end0; ++target0) {
    target0->physicalDamage(target0->getHp().max_ * temp_per);
  }
}


void battle::trap_command() {
  // TIPS: 戦闘メニューからコマンド実行されるまで処理を実行しない
  if (!trap_) { return; }

  std::list<cCharacter>::iterator target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: 確率で罠状態にする
  for (target0 = begin0; target0 != end0; ++target0) {
    if (target0->getStatus().luk_ / 2 < data::random.fromZeroToLast(1000)) {
      target0->trap();
    }
  }
}


void battle::poison_command() {
  // TIPS: 戦闘メニューからコマンド実行されるまで処理を実行しない
  if (!poison_) { return; }

  std::list<cCharacter>::iterator target0, begin0, end0;
  begin0 = enemy.begin();
  end0 = enemy.end();

  // TIPS: 確率で毒状態にする
  for (target0 = begin0; target0 != end0; ++target0) {
    if (target0->getStatus().luk_ / 4 < data::random.fromZeroToLast(1000)) {
      target0->poison();
    }
  }
}


// TIPS: 使用されたコマンドに対応したカウントを減少、フラグを消す
void battle::command_act() {
  if (fire_) {
    fire_ = false;
    --cnt_fire_;
  }

  if (cannon_) {
    cannon_ = false;
    --cnt_cannon_;
  }

  if (trap_) {
    trap_ = false;
    --cnt_trap_;
  }

  if (poison_) {
    poison_ = false;
    --cnt_poison_;
  }
}


// TIPS: プレイヤー側のキャラクターが行動中なら、敵の情報を取得
void battle::getEnemyInfo() {
  begin_ = enemy.begin();
  end_ = enemy.end();
}


// TIPS: 敵キャラが行動中ならプレイヤーの情報を取得させる
void battle::getPlayerInfo() {
  begin_ = data::user.player.begin();
  end_ = data::user.player.end();
}


void battle::init() {
  // TIPS: マップ情報の初期化
  map_data_.mapInit(data::user.map_info[stage_id_].id_);

  phase_ = phase::Standby;
  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  enemyStandby();

  countInit();
  camera_x_ = 0.f;

  fire_ = false;
  cannon_ = false;
  trap_ = false;
  poison_ = false;

  // TIPS: BGMのプレイナンバーを取得
  short play_num = data::getTerritory() == design::AllStage - 1 ?
    data::random.fromFirstToLast(bgm::Boss1, bgm::Boss2) :
    data::random.fromFirstToLast(bgm::Battle1, bgm::Battle3);

  data::music.stop();
  data::music.play(play_num);
}


void battle::draw() {
  //++anime.time_;
  //if (anime.time_ >= size::WIDTH * 2) { anime.time_ = 0; }
  map_data_.draw(anime.time_);

  dispUnitInfo();

  // TIPS: プレイヤー側のキャラクターすべての処理
  begin = data::user.player.begin();
  end = data::user.player.end();
  for (it = begin, i = 0; it != end; ++it, ++i) {
    it->draw(it->isDead() ? -camera_x_ : 0.f);
    dispPlayerInfo();
  }

  // TIPS: すべての敵キャラの処理
  begin = enemy.begin();
  end = enemy.end();
  for (it = begin; it != end; ++it) {
    it->draw(camera_x_);
    dispEnemyInfo();
  }
}


void battle::dispUnitInfo() {

  // TIPS: ウィンドウだけを表示、実際のデータはキャラクター描画と同時に行う
  for (i = 0; i < 5; ++i) {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1);
    pos_.y() = -size::HEIGHT / 2;

    win::draw(pos_, Vec2f(size::UnitInfo_W, size::UnitInfo_H),
      win::color(paint::Blue, 0.8f));
  }
}


// TIPS: プレイヤー側のキャラクターの情報を表示
void battle::dispPlayerInfo() {

  // レベルと名前
  {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1) + size::NamePos_X;
    pos_.y() = -size::HEIGHT / 2 + size::NamePos_Y;
    sstream level;
    level << "LV:" << std::setw(3) << it->getInfo().level_ << " " << it->getInfo().name_;
    telop_.draw(level.str(), pos_, win::color(paint::White));
  }

  // HPゲージ
  {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1) + size::GaugePos_X;
    pos_.y() = -size::HEIGHT / 2 + size::GaugePos_Y * 2.5f;

    win::gaugeDraw(pos_, Vec2f(size::Gauge_W, size::Gauge_H),
      it->hpGaugeRatio(), it->hpColor());
  }

  // タイムゲージ
  {
    pos_.x() = -size::WIDTH / 2 + i * (size::UnitInfo_W + 1) + size::GaugePos_X;
    pos_.y() = -size::HEIGHT / 2 + size::GaugePos_Y;

    win::gaugeDraw(pos_, Vec2f(size::Gauge_W, size::Gauge_H),
      it->timeGaugeRatio(), it->timeColor());
  }
}


void battle::dispEnemyInfo() {

  // 名前
  sstream name;
  name << "LV:" << std::setw(3) << it->getInfo().level_
    << " " << it->getInfo().name_;

  pos_.x() =
    it->getPos().x() - telop_.drawSize(name.str()).x() / 2;
  pos_.y() = it->getPos().y() + size::Unit_W * 3.5f;
  telop_.draw(name.str(), pos_, it->levelColor());

  // HPゲージ
  pos_.x() = it->getPos().x() - size::Gauge_W / 4;
  pos_.y() -= 20;
  win::gaugeDraw(pos_, Vec2f(size::Gauge_W / 2, size::Gauge_H * 0.75f),
    it->timeGaugeRatio(), it->timeColor());

  // タイムゲージ
  pos_.y() -= 20;
  win::gaugeDraw(pos_, Vec2f(size::Gauge_W / 2, size::Gauge_H * 0.75f),
    it->hpGaugeRatio(), it->hpColor());
}
