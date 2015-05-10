
#pragma once
#include "../System/base.hpp"
#include <array>


// 音楽ファイル関係
namespace literal_media {

  // BGM
  enum Music {
    Title,
    Strategy,
    DataSelect,
    Result_Win,
    Result_Lose,
    Battle1,
    Battle2,
    Battle3,
    Boss1,
    Boss2,

    All_BGM,
  };

  // SE
  enum SoundEffect {
    Click,
    Click_Menu,
    MouseOn,

    All_SE,
  };
}

typedef literal_media::Music        bgm;
typedef literal_media::SoundEffect  se;


class cMediaResource {
  std::array<Media, bgm::All_BGM> music;
  std::array<Media, se::All_SE>   se;

  bool is_init_;
  short playing_music_;

public:
  cMediaResource() : playing_music_(0), is_init_(false) {}

  // 音楽ファイルの初期化
  // TIPS: ゲーム起動時に１回だけ呼ぶ
  void mediaDataInit() {
    if (is_init_) { return; }

    music[bgm::Title]       = Media("res/wav/title.wav");
    music[bgm::Strategy]    = Media("res/wav/politics.wav");
    music[bgm::DataSelect]  = Media("res/wav/slot_select.wav");
    music[bgm::Result_Win]  = Media("res/wav/result_win.wav");
    music[bgm::Result_Lose] = Media("res/wav/result_lose.wav");
    music[bgm::Battle1]     = Media("res/wav/battle_normal1.wav");
    music[bgm::Battle2]     = Media("res/wav/battle_normal2.wav");
    music[bgm::Battle3]     = Media("res/wav/battle_normal3.wav");
    music[bgm::Boss1]       = Media("res/wav/battle_boss1.wav");
    music[bgm::Boss2]       = Media("res/wav/battle_boss2.wav");

    for (short i = 0; i < bgm::All_BGM; ++i) {
      music[i].gain(0.5f);
      music[i].looping(true);
    }

    se[se::Click]      = Media("res/wav/click_1.wav");
    se[se::Click_Menu] = Media("res/wav/click_2.wav");
    se[se::MouseOn]    = Media("res/wav/mouse_on.wav");

    for (short i = 0; i < se::All_SE; ++i) { se[i].gain(0.5f); }

    is_init_ = true;
  }

  // 再生中のBGMを停止する
  void musicStop() { music[playing_music_].stop(); }

  // BGM再生
  // TIPS: 再生したいBGMナンバーを引数として指定する
  // * すでに別のBGMを再生中なら、新規BGMの再生を許可しない
  void musicPlay(const short& number) {
    if (music[playing_music_].isPlaying()) { return; }
    playing_music_ = number;
    music[number].play();
  }

  // SE再生
  // TIPS: 再生したい効果音を引数として指定する
  void sePlay(const short& number) { se[number].play(); }
};
