
#include "character.h"

typedef cCharacter  chara;
using namespace character;


cCharacter::cCharacter() : lane_(Front) { 
  info.type_ = Fighter;
  info.gender_ = Male;
  initImage();
  initHpMax();
}


/* 戦闘システム関係 */
////////////////////////////////////////////////////////////

// TIPS: 戦闘開始時のパラメータ初期化
void chara::posInit(const Vec2f& pos0, const u_short& id0, const u_short& efc0) {
  pos_ = pos0;

  map_id_ = id0;
  effect_ = efc0;

  turn_ = 0.f;

  anime.state[Now]  = action::Stand;
  anime.state[Next] = action::Stand;
  anime.time_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.f;

  bad.trap_time_ = 0;
  bad.poison_time_ = 0;

  // TIPS: 罠開始のマップ効果だったとき、罠状態でスタート
  if (efc0 == effect::Trap_Start && info.side_) { trap(); }

  // TIPS: 毒開始のマップ効果だったとき、毒状態でスタート
  if (efc0 == effect::Poison_Start && info.side_) { poison(); }

  time.heal_ = 0;
  time.motion_ = 0;
}


// TIPS: 行動可能になるまで、カウントを進める
void chara::timeCount() {
  short speed = 50 + getStatus().spd_ / 10;

  // TIPS: 地形効果によるボーナスを付与
  switch (map_id_) {
    case effect::Grass : speed *= 1.2f; break;
    case effect::Forest: speed *= 0.8f; break;
    default:;
  }

  // TIPS: 罠状態なら速度を下げる
  //     : 罠の効果時間は自身の敏捷によって変化
  if (bad.trap_time_ > 0) {
    speed *= 0.5f;
    bad.trap_time_ -= speed;
  }

  // TIPS: 毒状態なら残り時間のカウントを減らす
  //     : 毒の効果は攻撃力計算の処理で反映
  if (bad.poison_time_ > 0) {
    bad.poison_time_ -= speed *= 0.5f;
  }

  turn_ += speed;
}


void chara::timeCountReset() {
  // TIPS: 行動したら自身のタイムゲージを減らして、
  //     : アニメの描画状態を攻撃アクションに切り替える
  turn_ -= UNIT_TURN;
  animeChange(action::Attack);
}


bool chara::isWait() const {
  // TIPS: まだ行動できないなら true を返す
  return turn_ < UNIT_TURN;
}


bool chara::isAnimePlaying() {
  bool play_ = time.motion_ > 0;

  // TIPS: アニメーションが終了したら待機状態に戻す
  play_ ? --time.motion_ :
    anime.state[Next] = (hpGaugeRatio() > 0.25f) ?
    action::Stand : action::Danger;

  return play_;
}


bool chara::isDead() {
  // TIPS: 戦闘不能なら描画状態を変更して、以降の処理を禁止する
  if (hp.now_ <= 0) { anime.state[Next] = action::Dead; }
  return anime.state[Now] == action::Dead;
}


// TIPS: 攻撃対象の決定（物理攻撃のみ）
bool chara::targetLook(const short& target_lane) {

  // TIPS: Scout クラスのみ、後列にいる敵を優先して狙う
  switch (info.type_) {
    case Scout: return target_lane == Back  ? true : false;
    default:    return target_lane == Front ? true : false;
  }
}


// TIPS: 攻撃力、回復量計算のためのパラメータを出力
u_short chara::getPower(const u_short& gen0) {
  u_short temp;

  // TIPS: 自身のクラスに応じた攻撃力基礎値を取得
  switch (info.type_) {
    case Fighter:;
    case   Scout:;
    case  Knight:;
    case Soldier:;
    case    Boss:
      temp = getStatus().pow_;
      // TIPS: 地形効果による攻撃力ボーナスを付与
      if (map_id_ == effect::Forest) { temp *= 0.8f; }
      break;

    case    Mage:;
    case  Priest:
      temp = getStatus().mag_;
      break;

    default: temp = 0;
  }

  // TIPS: マップ特殊効果による攻撃力ボーナスを付与
  switch (effect_) {
    case effect::Front_Adv:
      if (lane_ == Front) { temp *= 1.2f; }
      break;

    case effect::Back_Adv:
      if (lane_ == Back) { temp *= 1.2f; }
      break;

    case effect::Fighter_Adv:
      if (info.type_ == Fighter) { temp *= 1.2f; }
      break;

    case effect::Scout_Adv:
      if (info.type_ == Scout) { temp *= 1.2f; }
      break;

    case effect::Knight_Adv:
      if (info.type_ == Knight) { temp *= 1.2f; }
      break;

    case effect::Mage_Adv:
      if (info.type_ == Mage) { temp *= 1.2f; }
      break;

    case effect::Priest_Adv:
      if (info.type_ == Priest) { temp *= 1.2f; }
      break;

    case effect::Soldier_Adv:
      if (info.type_ == Soldier) { temp *= 1.5f; }
      break;

    case effect::Boss_Adv:
      if (info.type_ == Boss) { temp *= 1.2f; }
      break;

    case effect::Male_Adv:
      if (info.gender_ == Male && gen0 == Female) { temp *= 1.5f; }
      break;

    case effect::Female_Adv:
      if (info.gender_ == Female && gen0 == Male) { temp *= 1.5f; }
      break;

    default:;
  }

  // TIPS: 毒状態なら攻撃力を減少
  //     : 毒の効果時間減少はタイムカウントの処理で実行
  if (bad.poison_time_ > 0) {
    temp *= 0.75f;
  }

  return temp;
}


// TIPS: 補正済みの攻撃力を受け取って計算を行う
void chara::physicalDamage(const u_short& val) {
  // TIPS: 自身のパラメータを取得
  sParameter temp = getStatus();

  // TIPS: 地形効果のボーナスを付与
  switch (map_id_) {
    case effect::Mount: temp.vit_ *= 1.2f; break;
    default:;
  }

  short damage_point = val - temp.vit_ / 2;
  hp.now_ -= damage_point > 1 ? damage_point : 1;

  animeChange(action::Damage);
}


// TIPS: 補正済みの攻撃力を受け取って計算を行う
void chara::magicalDamage(const u_short& val) {
  // TIPS: 自身のパラメータを取得
  sParameter temp = getStatus();

  // TIPS: 地形効果のボーナスを付与
  switch (map_id_) {
    case effect::Forest: temp.mag_ *= 1.2f; break;
    case effect::Mount : temp.mag_ *= 0.8f; break;
    default:;
  }

  short damage_point = val - temp.mag_ / 2;
  hp.now_ -= damage_point > 1 ? damage_point : 1;

  animeChange(action::Damage);
}


// TIPS: Priest による回復
void chara::recovery(const u_short& val) {
  hp.now_ += val;
  if (hp.now_ > hp.max_) { hp.now_ = hp.max_; }
}


Color chara::hpColor() const {
  Color temp;

  // TIPS: 残り HP の割合に応じてカラーを変更
  temp = hpGaugeRatio() > 0.5f ? win::color(paint::Green)
    : hpGaugeRatio() > 0.25f ? win::color(paint::Yellow)
    : win::color(paint::Red);

  // TIPS: 毒状態ならカラー変更
  if (bad.poison_time_ > 0) {
    temp = win::color(paint::Purple);
  }

  return temp;
}


float chara::hpGaugeRatio() const {
  return float(hp.now_) / hp.max_;
}


Color chara::timeColor() const {
  // TIPS: 罠状態の時のみ、タイムバーの色を変更
  return bad.trap_time_ > 0 ?
    win::color(paint::Purple) : win::color(paint::Orange);
}


float chara::timeGaugeRatio() const {
  return float(turn_ < UNIT_TURN ? turn_ : UNIT_TURN) / UNIT_TURN;
}


// TIPS: 戦闘終了後、全員の HP を回復する
void chara::battleFinish() {
  hp.now_ = hp.max_;
}


/* パラメータ操作（データ出力）関係 */
////////////////////////////////////////////////////////////


// TIPS: クラスごとのパラメータ補正を加えた状態で出力
sParameter chara::getStatus() const {
  sParameter temp;

  switch (info.type_) {
    case Fighter:
      temp.pow_ = status.pow_ * 1.1f;
      temp.vit_ = status.vit_ * 1.1f;
      temp.mag_ = status.mag_ * 0.8f;
      temp.spd_ = status.spd_ * 1.2f;
      temp.dex_ = status.dex_ * 1.0f;
      temp.luk_ = status.luk_ * 0.8f;
      break;

    case Scout:
      temp.pow_ = status.pow_ * 1.0f;
      temp.vit_ = status.vit_ * 1.0f;
      temp.mag_ = status.mag_ * 1.0f;
      temp.spd_ = status.spd_ * 1.1f;
      temp.dex_ = status.dex_ * 1.2f;
      temp.luk_ = status.luk_ * 0.7f;
      break;

    case Knight:
      temp.pow_ = status.pow_ * 1.2f;
      temp.vit_ = status.vit_ * 1.2f;
      temp.mag_ = status.mag_ * 0.9f;
      temp.spd_ = status.spd_ * 0.9f;
      temp.dex_ = status.dex_ * 1.1f;
      temp.luk_ = status.luk_ * 0.7f;
      break;

    case Mage:
      temp.pow_ = status.pow_ * 0.8f;
      temp.vit_ = status.vit_ * 0.9f;
      temp.mag_ = status.mag_ * 1.2f;
      temp.spd_ = status.spd_ * 1.0f;
      temp.dex_ = status.dex_ * 0.9f;
      temp.luk_ = status.luk_ * 1.2f;
      break;

    case Priest:
      temp.pow_ = status.pow_ * 0.9f;
      temp.vit_ = status.vit_ * 0.8f;
      temp.mag_ = status.mag_ * 1.1f;
      temp.spd_ = status.spd_ * 0.8f;
      temp.dex_ = status.dex_ * 0.8f;
      temp.luk_ = status.luk_ * 1.6f;
      break;

    case Soldier:
      temp.pow_ = status.pow_ * 0.8f;
      temp.vit_ = status.vit_ * 0.8f;
      temp.mag_ = status.mag_ * 0.8f;
      temp.spd_ = status.spd_ * 0.8f;
      temp.dex_ = status.dex_ * 0.8f;
      temp.luk_ = status.luk_ * 0.8f;
      break;

    case Boss:
      temp.pow_ = status.pow_ * 1.2f;
      temp.vit_ = status.vit_ * 1.2f;
      temp.mag_ = status.mag_ * 1.2f;
      temp.spd_ = status.spd_ * 1.2f;
      temp.dex_ = status.dex_ * 1.2f;
      temp.luk_ = status.luk_ * 1.2f;
  }

  return temp;
}


Vec2f chara::getPos() const {
  return pos_;
}


u_short chara::getLane() const {
  return lane_;
}


u_short chara::hitRatio() const {
  sParameter temp = getStatus();

  // TIPS: マップ特殊効果のボーナスを付与
  if (effect_ == effect::Unlucky) { temp.luk_ *= 0.5f; }
  
  // TIPS: 地形効果のボーナスを付与
  switch (map_id_) {
    case effect::Grass : temp.spd_ *= 1.2f; break;
    case effect::Forest: temp.spd_ *= 0.8f; break;
    case effect::Mount : temp.dex_ *= 0.8f; break;
    default:;
  }

  return temp.dex_ + (temp.spd_ + temp.luk_) / 10;
}


u_short chara::avoidRatio() const {
  sParameter temp = getStatus();
  if (effect_ == effect::Unlucky) { temp.luk_ *= 0.5f; }
  return (temp.spd_ + temp.luk_) / 10;
}


/* パラメータ操作（設定）関係 */
////////////////////////////////////////////////////////////

void chara::aliasTargetPos(const Vec2f& t) {
  target_ = t;
}


// TIPS: プレイヤー側ユニットの成長
void chara::growth() {
  if (info.level_ < LEVEL_LIMIT) { ++info.level_; }

  switch (info.type_) {
    case Fighter:
      status.pow_ += 4;
      status.vit_ += 4;
      status.mag_ += 1;
      status.spd_ += 6;
      status.dex_ += 3;
      status.luk_ += 2;
      break;

    case Scout:
      status.pow_ += 3;
      status.vit_ += 3;
      status.mag_ += 3;
      status.spd_ += 4;
      status.dex_ += 5;
      status.luk_ += 2;
      break;

    case Knight:
      status.pow_ += 5;
      status.vit_ += 5;
      status.mag_ += 2;
      status.spd_ += 2;
      status.dex_ += 4;
      status.luk_ += 2;
      break;

    case Mage:
      status.pow_ += 2;
      status.vit_ += 2;
      status.mag_ += 7;
      status.spd_ += 3;
      status.dex_ += 3;
      status.luk_ += 3;
      break;

    case Priest:
      status.pow_ += 2;
      status.vit_ += 1;
      status.mag_ += 6;
      status.spd_ += 2;
      status.dex_ += 2;
      status.luk_ += 7;
      break;

    default:;
  }

  // TIPS: 限界を超えて成長しないようにする
  if (status.pow_ >= PARAM_LIMIT) { status.pow_ = PARAM_LIMIT; }
  if (status.vit_ >= PARAM_LIMIT) { status.vit_ = PARAM_LIMIT; }
  if (status.mag_ >= PARAM_LIMIT) { status.mag_ = PARAM_LIMIT; }
  if (status.spd_ >= PARAM_LIMIT) { status.spd_ = PARAM_LIMIT; }
  if (status.dex_ >= PARAM_LIMIT) { status.dex_ = PARAM_LIMIT; }
  if (status.luk_ >= PARAM_LIMIT) { status.luk_ = PARAM_LIMIT; }

  initHpMax();
}


// クラスチェンジ
// TIPS: 兵士タイプに対応した画像に切り替える
void chara::classChange() {
  ++info.type_;

  // TIPS: 味方の時は、Priest から Fighter に戻す
  //     : 敵の時は、Soldier から Fighter に戻す
  if (info.type_ > (info.side_ ? Priest : Soldier)) { info.type_ = Fighter; }

  initImage();
  initHpMax();
}


void chara::genderChange() {
  info.gender_ = (info.gender_ == Male) ? Female : Male;
}


void chara::laneShift() {
  lane_ = (lane_ == Front) ? Back : Front;
}


/* アニメーション関係 */
////////////////////////////////////////////////////////////

void chara::animePlay() {
  ++anime.time_;
  if (anime.time_ >= design::Fps - 20) { anime.time_ = 0; }
  anime.blink_ = anime.time_ / (design::Fps / 6);
}


void chara::animeShift() {
  // TIPS: ダメージアニメが終了したら直前の描画アニメをやり直す
  if (anime.state[Now] == action::Damage && !time.motion_) {
    anime.state[Next] = anime.state[Last];
    anime.state[Last] = action::Stand;
  }

  // TIPS: 次の描画状態を指定されたらタイマーをリセットして、
  // * 現在の状態に上書きしてアニメを切り替える
  if (anime.state[Now] != anime.state[Next]) {
    anime.time_ = 0;
    anime.state[Now] = anime.state[Next];
  }
}


void chara::animeChange(const u_short& act) {
  // TIPS: 描画するアニメーションに合わせた画像に切り替える
  anime.state[Next] = act;
  anime.state[Last] = anime.state[Now];
  time.motion_ = design::Fps - 21;
}


void chara::draw(const float& camera_x) {
  // TIPS: 味方か敵かを判定してキャラクターの向きを変更
  cut_.x() = info.side_ ?
    (anime.blink_ + 1) * size::Unit_W + info.gender_ * (size::Unit_W * 4) :
    anime.blink_ * size::Unit_W + info.gender_ * (size::Unit_W * 4);

  cut_.y() = anime.state[Now] * size::Unit_H;

  drawTextureBox(pos_.x() + camera_x, pos_.y(), size::Unit_W, size::Unit_H,
    cut_.x(), cut_.y(), info.side_ ? -size::Unit_W : size::Unit_W, size::Unit_H,
    own_, Color(1, 1, 1),
    0, Vec2f(3.f, 3.f), Vec2f(size::Unit_W / 2, 0));
}


/* データ管理関係 */
////////////////////////////////////////////////////////////

// TIPS: 読み込んだセーブデータの反映
void chara::fileLoad(std::ifstream& fstr) {
  fstr
    >> info.side_
    >> info.name_
    >> info.type_
    >> info.gender_
    >> info.level_

    >> lane_

    >> hp.now_
    >> hp.max_

    >> status.pow_
    >> status.vit_
    >> status.mag_
    >> status.spd_
    >> status.dex_
    >> status.luk_;

  initImage();
}


sCharacterInfo chara::getInfo() const {
  return info;
}


// TIPS: 補正なしのパラメータを出力
sParameter chara::getBaseStatus() const {
  return status;
}


sHitPoint chara::getHp() const {
  return hp;
}


/* private */
// TIPS: 補正された自身のパラメータを基に、HP最大値の初期化を行う
void chara::initHpMax() {
  hp.max_ =
    getStatus().vit_ * 2
    + getStatus().pow_
    + getStatus().spd_;

  hp.now_ = hp.max_;
}


/* private */
// クラスチェンジによるキャラクター画像の切り替え
void chara::initImage() {

  // TIPS: 自分のクラスに対応した画像、クラス名を取得
  switch (info.type_) {
    case Fighter:
      own_ = Texture("res/png/unit_fighter.png");
      info.name_ = info.side_ ? "ファイター" : "敵：軽装兵";
      break;

    case Scout:
      own_ = Texture("res/png/unit_scout.png");
      info.name_ = info.side_ ? "スカウト" : "敵：偵察兵";
      break;

    case Knight:
      own_ = Texture("res/png/unit_knight.png");
      info.name_ = info.side_ ? "ナイト" : "敵：重装兵";
      break;

    case Mage:
      own_ = Texture("res/png/unit_mage.png");
      info.name_ = info.side_ ? "メイジ" : "敵：魔道士";
      break;

    case Priest:
      own_ = Texture("res/png/unit_priest.png");
      info.name_ = info.side_ ? "プリースト" : "敵：僧侶";
      break;

    case Boss:
      own_ = Texture("res/png/unit_boss.png");
      info.gender_ = Male;
      info.name_ = "敵：将軍";
      break;

    default:
      own_ = Texture("res/png/unit_soldier.png");
      info.gender_ = Male;
      info.name_ = "敵：一般兵";
  }
}


/* 敵専用 */
////////////////////////////////////////////////////////////

void chara::initEnemy(const short& type0, const short& gen0) {
  info.side_ = false;
  info.type_ = type0;
  info.gender_ = gen0;
  initImage();
}


void chara::growthEnemy(const short level) {
  info.level_ = level;

  switch (info.type_) {
    case Fighter:
      status.pow_ += 3 * level;
      status.vit_ += 3 * level;
      status.mag_ += 1 * level;
      status.spd_ += 5 * level;
      status.dex_ += 2 * level;
      status.luk_ += 1 * level;
      break;

    case Scout:
      status.pow_ += 2 * level;
      status.vit_ += 2 * level;
      status.mag_ += 2 * level;
      status.spd_ += 4 * level;
      status.dex_ += 4 * level;
      status.luk_ += 1 * level;
      break;

    case Knight:
      status.pow_ += 4 * level;
      status.vit_ += 5 * level;
      status.mag_ += 2 * level;
      status.spd_ += 2 * level;
      status.dex_ += 3 * level;
      status.luk_ += 1 * level;
      break;

    case Mage:
      status.pow_ += 1 * level;
      status.vit_ += 1 * level;
      status.mag_ += 6 * level;
      status.spd_ += 3 * level;
      status.dex_ += 2 * level;
      status.luk_ += 2 * level;
      break;

    case Priest:
      status.pow_ += 1 * level;
      status.vit_ += 1 * level;
      status.mag_ += 5 * level;
      status.spd_ += 1 * level;
      status.dex_ += 1 * level;
      status.luk_ += 6 * level;
      break;

    // Soldier
    default:
      status.pow_ += 3 * level;
      status.vit_ += 3 * level;
      status.mag_ += 1 * level;
      status.spd_ += 2 * level;
      status.dex_ += 4 * level;
      status.luk_ += 2 * level;
      break;

    case Boss:
      status.pow_ += 4 * level;
      status.vit_ += 4 * level;
      status.mag_ += 4 * level;
      status.spd_ += 3 * level;
      status.dex_ += 4 * level;
      status.luk_ += 1 * level;
  }

  initHpMax();
  hp.now_ = hp.max_;
  lane_ = info.gender_ == Male ? Front : Back;
}


Color chara::levelColor() {
  // TIPS: 名前とレベルの色を、自身のレベルに応じた色に変更
  return
    info.level_ > 100 ? win::color(paint::Red) :
    info.level_ > 60  ? win::color(paint::Pink) :
    info.level_ > 30  ? win::color(paint::Orange) :
    win::color(paint::White);
}


/* 状態異常設定 */
////////////////////////////////////////////////////////////

// 罠コマンドを発動
void chara::trap() {
  bad.trap_time_ = UNIT_TURN * 2;
}


// 毒矢コマンドを発動
void chara::poison() {
  bad.poison_time_ = UNIT_TURN * 2;
}


void chara::drawMinUnit(const Vec2f& pos) {
  drawTextureBox(pos.x() + lane_ * 30, pos.y(), size::Unit_W, size::Unit_H,
    info.gender_ == Male ? size::Unit_W : size::Unit_W * 5, 0, size::Unit_W, size::Unit_H,
    own_, Color(1, 1, 1),
    0, Vec2f(2.f, 2.f), Vec2f(size::Unit_W / 2, 0));
}
