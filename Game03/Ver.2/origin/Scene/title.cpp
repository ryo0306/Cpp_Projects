
#include "title.h"
#include "../../lib/utils.hpp"

typedef cTitle  title;
using namespace literal_title;
using namespace literal_effect;


cTitle::cTitle() :
telop_(FONT_STRING),
back_(Texture("res/png/back_title.png")),
back_sea_(Texture("res/png/world_sea.png")),
back_fog_(Texture("res/png/world_sea_fog.png")),
logo_(Texture("res/png/back_title_logo.png")),
button_size_(Vec2f(button::Large_W, button::Large_H)),
game_clear_(data::system.game_clear_),
now_scene_(data::system.scene_[play::Now]),
next_scene_(data::system.scene_[play::Next]),
last_scene_(data::system.scene_[play::Last]) {

  telop_.size(font::Large);

  const std::string textData[] = {
    "Playable Mouse Only ",
    "Click Start ",
    "RightClick Back ",

    "New Game ",
    "Continue ",
    "Quit ",

    "�d�`�r�x ",
    "�m�n�q�l�`�k ",
    "�g�`�q�c ",
  };

  for (index_ = 0; index_ < elemsof(textData); ++index_) {
    text[index_] = textData[index_];
  }
}


void title::update() {

  // TIPS: ���炩�̉��o���s���Ȃ���͂������Ȃ�
  anime.is_play_ = stateChange();
  if (anime.is_play_) { return; }

  // TIPS: �{�^���̏���
  buttonIndexTranslate();

  for (index_ = begin_; index_ < end_; ++index_) {
    buttonPosTranslate(index_);
    on_mouse_ = env::rectOnMouse(pos_, button_size_);

    // TIPS: �����p���X�^�[�g�̂݁AHard ��I���ł���悤�ɂ���
    if (index_ == str::Hard && !game_clear_) { on_mouse_ = false; }

    if (on_mouse_ && env::leftClick()) {
      click_button_ = index_;
      ++anime.state_;
      data::media.sePlay(se::Click);
    }
  }

  // TIPS: ��Փx�I������߂鏈��
  if (anime.state_ == Dif_Select &&
      env::rightClick()) {
    anime.state_ = FadeOut_2;
  }
}


bool title::stateChange() {
  switch (anime.state_) {
    case Start:
      clickAction(); break;

    case Blink_1:;
    case Blink_2:;
    case Blink_3:
      blinkMode(); break;

    case FadeIn_1:;
    case FadeIn_2:
      fadeInMode(); break;

    case FadeOut_1:;
    case FadeOut_2:
      fadeOutMode(); break;

    case SceneChange:
      nextSceneShift(); break;

    default:;
  }

  return (anime.state_ != Mode_Select &&
          anime.state_ != Dif_Select);
}


void title::clickAction() {
  if (env::leftClick()) {
    ++anime.state_;
    data::media.sePlay(se::Click);
  }
}


void title::blinkMode() {

  // TIPS: �_�ł��I���܂Ŏ��̏����Ɉڍs���Ȃ�
  ++anime.blink_;
  if (anime.blink_ < spec::Fps) { return; }

  switch (click_button_) {
    case Continue:
      anime.state_ = SceneChange;
      break;

    case Quit:
      next_scene_ = scene::Exit;
      break;

    default:
      ++anime.state_;
  }

  anime.blink_ = 0;
  if (anime.state_ < Dif_Select) { click_button_ = -1; }
}


void title::fadeInMode() {

  // TIPS: ���o���I���܂Ŏ��̏����Ɉڍs���Ȃ�
  anime.alpha_ += fadeSpeed;
  if (anime.alpha_ < buttonAlpha) { return; }

  ++anime.state_;
}


void title::fadeOutMode() {

  // TIPS: ���o���I���܂Ŏ��̏����Ɉڍs���Ȃ�
  anime.alpha_ -= fadeSpeed;
  if (anime.alpha_ > 0) { return; }

  anime.state_ = anime.state_ == FadeOut_2 ?
    FadeIn_1 : anime.state_ + 1;
}


void title::nextSceneShift() {
  last_scene_ = now_scene_;

  if (click_button_ == Continue) {
    next_scene_ = scene::Load;
    return;
  }

  next_scene_ = scene::Strategy;
  game_clear_ ? reStart() : newGame();
}


void title::newGame() {
  userDataInit();
  mapDataInit();

  data::user.assets.money_ = StartMoney;
  data::user.assets.food_ = 1;

  data::user.cnt.game_clear_ = 0;

  data::user.weapon_cnt.fire_ = 0;
  data::user.weapon_cnt.bomb_ = 0;
  data::user.weapon_cnt.trap_ = 0;
  data::user.weapon_cnt.poison_ = 0;

  data::user.player.clear();
  for (short i = 0; i < spec::BattleMember; ++i) {
    data::user.player.emplace_back();
  }
}


void title::reStart() {
  userDataInit();
  mapDataInit();

  ++data::user.cnt.game_clear_;
}


void title::userDataInit() {
  data::user.dif_ = click_button_ - str::Easy;

  data::user.cnt.battle_ = 0;
  data::user.cnt.battle_cmd_ = 0;
  data::user.cnt.work_ = 0;
}


void title::mapDataInit() {
  for (short i = 0; i < spec::All_Stage; ++i) {
    data::user.map_info_[i].territory_ = false;
    data::user.map_info_[i].lose_ = false;

    data::user.map_info_[i].id_
      = data::rand.fromZeroToLast(map_id::All_Map);

    data::user.map_info_[i].attribute_
      = data::rand.fromZeroToLast(effect::All_Effect);
  }

  short r = data::rand.fromZeroToLast(spec::All_Stage);
  data::user.map_info_[r].territory_ = true;
}


void title::init() {
  anime.state_ = Start;

  anime.time_ = 0;
  anime.angle_ = 0.0f;

  anime.logo_ = 0;
  anime.blink_ = 0;
  anime.alpha_ = 0.0f;
  anime.scroll_ = 0.0f;

  click_button_ = -1;

  data::media.musicStop();
  data::media.musicPlay(bgm::Title);
}


void title::draw() {
  effectPlay();
  disp_back();
  disp_telop();
  disp_button();
}


void title::effectPlay() {
  ++anime.time_;
  if (anime.time_ >= spec::Fps) { anime.time_ = 0; }

  anime.angle_ += AngleSpeed * 0.1f;
  if (anime.angle_ >= M_PI) { anime.angle_ = 0.0f; }

  ++anime.logo_;
  if (anime.logo_ >= LogoBlink_Time) { anime.logo_ = 0; }

  anime.scroll_ += scrollSpeed;
  if (anime.scroll_ >= ScrollEndPoint) { anime.scroll_ = 0.0f; }
}


void title::disp_back() {

  // TIPS: �F�������C�ɓ���Ȃ��̂ŁA�Ŕw�ʂɕʂ̐F����������`��
  drawFillBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
              draw::color(paint::White));

  // �C
  drawTextureBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
                 anime.scroll_ * 2, 0, Sea_Fog_WH, Sea_Fog_WH,
                 back_sea_,
                 draw::color(paint::White, backAlpha));

  // �g
  drawTextureBox(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT,
                 anime.scroll_, 0, Sea_Fog_WH, Sea_Fog_WH,
                 back_fog_,
                 draw::color(paint::SkyBlue, backAlpha));

  // �嗤
  drawTextureBox(0, 0, WIDTH, HEIGHT, 0, 0, Map_WH, Map_WH,
                 back_, draw::color(paint::Black),
                 0, mapScale, centerTranslate);
  drawTextureBox(0, 0, WIDTH, HEIGHT, 0, 0, Map_WH, Map_WH,
                 back_, draw::color(paint::Orange),
                 0, defaultScale, centerTranslate);

  // ���S
  const short blink = anime.logo_ / LogoBlink_Span;
  drawTextureBox(0, LogoPos_Y, Logo_W, Logo_H,
                 0, Logo_H * blink, Logo_W, Logo_H,
                 logo_, draw::color(paint::White),
                 0, logoScale, logoTranslate);
}


void title::disp_telop() {

  // ������@
  pos_.x() = -telop_.drawSize(text[str::Mouse]).x() / 2;
  pos_.y() = TelopPos_Y + std::abs(std::sin(anime.angle_)) * JumpHeight;
  telop_.draw(text[str::Mouse], pos_, draw::color(paint::Yellow));

  // TIPS: ��Փx�����肵����\�����Ȃ�
  if (anime.state_ > Dif_Select) { return; }

  // ����w��
  draw_switch_ = anime.state_ == Dif_Select;
  draw_telop_ = draw_switch_ ? str::Back : str::Click;
  telop_.draw(text[draw_telop_],
              Vec2f(-telop_.drawSize(text[draw_telop_]).x() / 2, Center),
              draw::color(draw_switch_ ? paint::Green : paint::Black,
              anime.state_ == Blink_1 ?
              (anime.blink_ / blinkSpeed) % 2 : anime.time_ / (spec::Fps / 2)));
}


void title::disp_button() {

  // TIPS: �P��N���b�N�����܂ŁA�܂��͓�Փx��I��������\�����Ȃ�
  if (anime.state_ < FadeIn_1 || anime.state_ == SceneChange) { return; }

  buttonIndexTranslate();

  for (index_ = begin_; index_ < end_; ++index_) {
    buttonPosTranslate(index_);
    on_mouse_ = env::rectOnMouse(pos_, button_size_);

    // TIPS: ���炩�̉��o���Ȃ�{�^���̐F��ς��Ȃ�
    if (anime.is_play_) { on_mouse_ = false; }

    // TIPS: �����p���X�^�[�g�̂݁AHard �̐F��ς���
    if (index_ == str::Hard && !game_clear_) { on_mouse_ = false; }

    // TIPS: �����p���j���[�Q�[���\�Ȃ�{�^���̐F��ς���
    take_over_ = (index_ == str::NewGame ||
                  index_ == str::Hard) &&
                  game_clear_;

    // �{�^��
    draw::window(pos_, button_size_,
                 draw::color(on_mouse_ ? paint::Orange :
                 take_over_ ? paint::Purple :
                 index_ == str::Hard ? paint::DarkGray : paint::Blue,
                 click_button_ == index_ ?
                 (anime.blink_ / blinkSpeed) % 2 : anime.alpha_));

    // �{�^���̕���
    buttonStringTranslate(index_);
    telop_.draw(text[index_], pos_,
                draw::color(on_mouse_ ? paint::Black : paint::White));
  }
}


void title::buttonIndexTranslate() {
  begin_ = anime.state_ < FadeIn_2 ? str::NewGame : str::Easy;
  end_ = begin_ + ButtonDrawMax;
}


void title::buttonPosTranslate(const short index) {
  pos_.x() = -button_size_.x() / 2;
  pos_.y() = ButtonPos_Y - ButtonPos_Span * (index - begin_);
}


void title::buttonStringTranslate(const short index) {
  pos_.x() = -telop_.drawSize(text[index]).x() / 2;
  pos_.y() = ButtonPos_Y - ButtonPos_Span * (index - begin_) + StringPos_Y;
}
