
#pragma once
#include "../common.h"

//
// �����N���X
//

class cMusicResource {
  Media music_[bgm::All_BGM];
  Media se_[se::All_SE];

  // �Đ�����BGM�i���o�[
  short playing_music_;

  // �������t���O
  bool isInit_;

public:
  cMusicResource() : playing_music_(0), isInit_(false) {}

  // �Đ����� wav �t�@�C���̓W�J
  // TIPS: ��x�������s�A�ȍ~�̌Ăяo���͖�������
  void dataInit() {
    if (isInit_) { return; }

    music_[bgm::Title]       = Media("res/wav/title.wav");
    music_[bgm::Strategy]    = Media("res/wav/politics.wav");
    music_[bgm::DataSelect]  = Media("res/wav/slot_select.wav");
    music_[bgm::Result_Win]  = Media("res/wav/result_win.wav");
    music_[bgm::Result_Lose] = Media("res/wav/result_lose.wav");
    music_[bgm::Battle1]     = Media("res/wav/battle_normal1.wav");
    music_[bgm::Battle2]     = Media("res/wav/battle_normal2.wav");
    music_[bgm::Battle3]     = Media("res/wav/battle_normal3.wav");
    music_[bgm::Boss1]       = Media("res/wav/battle_boss1.wav");
    music_[bgm::Boss2]       = Media("res/wav/battle_boss2.wav");

    for (short i = 0; i < bgm::All_BGM; ++i) {
      music_[i].gain(0.5f);
      music_[i].looping(true);
    }

    se_[se::Click]      = Media("res/wav/click_1.wav");
    se_[se::Click_Menu] = Media("res/wav/click_2.wav");
    se_[se::MouseOn]    = Media("res/wav/mouse_on.wav");

    for (short i = 0; i < se::All_SE; ++i) { se_[i].gain(0.5f); }

    isInit_ = true;
  }

  // �Đ�����BGM���~����
  void stop() { music_[playing_music_].stop(); }

  // BGM�Đ�
  // TIPS: �Đ�������BGM�i���o�[�������Ƃ��Ďw�肷��
  // * ���łɕʂ�BGM���Đ����Ȃ�A�V�KBGM�̍Đ��������Ȃ�
  void play(const short& number) {
    if (music_[playing_music_].isPlaying()) { return; }
    playing_music_ = number;
    music_[number].play();
  }

  // SE�Đ�
  // TIPS: �Đ����������ʉ��������Ƃ��Ďw�肷��
  void sePlay(const short& number) { se_[number].play(); }
};
