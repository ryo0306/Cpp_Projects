//
// �Q�[���S�̂Ŏg���ϐ��A�萔���X�g
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../App_Lib/random.hpp"
#include <ctime>


/* BGM,SE �̐ݒ�p�}�N�� */
////////////////////////////////////////////////////////////////////////////////

#define  M_THEMA      "res/wav/main_thema.wav"         // ���C���e�[�}�̃��[�v����
#define  M_INTRO      "res/wav/main_thema_intro.wav"   // ���C���e�[�}�̃C���g��
#define  SE_CLICK     "res/wav/se_click.wav"           // �N���b�NSE
#define  SE_SUCCESS   "res/wav/se_success.wav"         // ����SE
#define  SE_FAILURE   "res/wav/se_failure.wav"         // ���sSE
#define  SE_FINISH    "res/wav/se_gameover.wav"        // �Q�[���I��SE


/* �Q�[���S�̂Ŏg���萔���X�g */
////////////////////////////////////////////////////////////////////////////////

// �E�B���h�E�T�C�Y
enum Window {
  WIDTH = 1200,
  HEIGHT = 800
};

// *�摜�̐؂�o���T�C�Y*
// W = Width, H = Height
enum Size {
  Bg_W = 512,      // �w�i�摜�F��
  Bg_H = 512,      // �w�i�摜�F����

  Card_W = 72,     // �J�[�h�摜�F��
  Card_H = 96,     // �J�[�h�摜�F����

  Logo_W = 512,    // �^�C�g�����S�F��
  Logo_Ha = 128,   // �^�C�g�����S�F�����i128�j
  Logo_Hb = 64,    // �^�C�g�����S�F�����i64�j

  Icon_W = 128,    // �A�C�R���F��
  Number_W = 16,   // �����F��
  Telop_W = 512,   // ������F��
  Telop_H = 32     // ������F����
};

// �Q�[���̃V�X�e�����
enum System {
  PLAYER = 4,      // �v���C���[���܂߂��l���i�����Q���̃Q�[���j
  RIVAL = 3,       // �ΐ푊��̐l���i�����Q���̃Q�[���j

  JOKER_OK = 53,   // �W���[�J�[�L��̃J�[�h�����i�{���� 2�����邪�A�Q�[���ł� 1�������g���j
  JOKER_NG = 52,   // �W���[�J�[�����̃J�[�h����

  ALL_CARD = 14,   // �W���[�J�[�L�胋�[���̎�D�̍ő喇��
  CARD_NUM = 13,   // �J�[�h�̐����i�`�`�j�܂ŁF0 �` 12�j
  CARD_MARK = 4,   // �J�[�h�̋L���i�W���[�J�[���܂܂Ȃ��j

  MARK_SPD = 0,    // �X�y�[�h
  MARK_HRT = 1,    // �n�[�g
  MARK_CLB = 2,    // �N���u
  MARK_DIA = 3,    // �_�C��

  RETIRE_P = 2,    // ���^�C�A�ɂȂ�p�X�̉�
  RETIRE_G = 5,    // �Q�[�������^�C�A�����Ƃ��̏���

  OPEN_ACT = 2,    // �\�ɂł����
  MAX_HIST = 8     // �\�ɂ����J�[�h�̍ő�L������
};


////////////////////////////////////////////////////////////////////////////////
/* g_common.cpp */

bool CO_on_mouse(Vec2f, float, float, float, float);
void CO_disp_number(Texture&, short, float, float);


////////////////////////////////////////////////////////////////////////////////
