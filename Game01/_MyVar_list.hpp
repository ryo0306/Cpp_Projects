//
// Revival of Kingdom �̕ϐ����X�g
//

#pragma once
#include "appEnv.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////

// �E�B���h�E�T�C�Y
enum Window {
  WIDTH = 512,
  HEIGHT = 512
};

// �e�摜�̐؂�o���T�C�Y
// �L�����Fchara (W, H)
// �@�����Ftelop (W, H)
// �@�����Fnumber (W)
// �@���Fstatus (W)
// �{�^���Fmenu (W, H)
// �@���Y�Ffund (W)
// �퓬�p�Ficon (W, H)
// W = width, H = height
enum Size {
  chara_W = 20, chara_H = 32,
  telop_W = 472, telop_H = 20,
  number_W = 14,
  status_W = 56,
  menu_W = 112, menu_H = 32,
  fund_W = 240,
  icon_W = 16, icon_H = 16
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �ڎ�
  �E�p�����[�^�֌W
  �E�t���O�֌W
  �E�t�h�֌W
  �E�퓬�֌W
 */

// �ȗ��L���ɂ���
// P_�F�v���C���[
// E_�F����

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �p�����[�^�֌W
  struct status {};
    status P, E;
  short P_pop, E_pop;
  const unsigned short fundinfo[], iteminfo[];
  short P_fund[], P_item[];
  short val[];
 */

struct status {
  int money;
  int fund;
  short territory;
  int soldier;
  int mercenary;
  short fame;
  short infamy;
};

// �v���C���[�FP �� ����FE �̃X�e�[�^�X���
// �������Fmoney
// �@���Y�Ffund
// �@�̒n�Fterritory
// �@���m�Fsoldier
// �`�E���Fmercenary
// �@�����Ffame
// �@�����Finfamy
status P = {0}, E = {0};

// �e�w�c�̐l�]�l�Fpop = popularity
// �l�] = ���� - ����
short P_pop, E_pop;

// �������Y�̉��i���
const unsigned short fundinfo[14] = {
  2000,     // ���q�̏�
  500,      // �_�n
  1000,     // �q��
  1000,     // �_�n�{�P
  2000,     // �q��{�P
  5000,     // �D���H��
  10000,    // �S�̍z�R
  20000,    // ��̍z�R
  30000,    // ���̍z�R
  50000,    // ���p��
  20000,    // �ǎ��ȓS�z�R
  15000,    // ����̒b�艮
  15000,    // �h��̒b�艮
  65535,    // �X
};

// �h���b�v���Y�̉��i���
const unsigned short iteminfo[7] = {
  1000,     // �����ȑ����i
  1500,     // �E�܂�������
  3000,     // �������C��
  10000,    // ���h�Ȃ���ނ�
  5000,     // �S�e�S�e������
  10000,    // �p�Y�̕���
  20000,    // �q�~�c�̓��L
};

// ���Y�̏��L���
// fund�F�������Y�i���p�s�j
// item�F�h���b�v���Y�i���p�j
short P_fund[14] = {0}, P_item[7] = {1, 0};

// �p�����[�^�̌�����ۑ�
short val[6] = {0};


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �t���O�֌W
  short stage_flag;
  bool stage_clear;
  short ex_flag;
  short spy;
  short sabo;
  bool rumor;
  bool disclos;
  bool rebel;
  short button_flag;
  bool add_attack;
 */

// ���݂̃G�s�\�[�h�i�́j���
short stage_flag = 0;

// �X�e�[�W�̃N���A�t���O
bool stage_clear = false;

// �G�N�X�g���X�e�[�W�˓��t���O�F 100 �𒴂�����s��
short ex_flag = 0;

// �X�p�C���s�����F 10 ��s��������I��
short spy = 0;

// �W�Q�H����s�����F�J�E���g�� 0�ɂȂ�����I���B�A�����s�ŃJ�E���g�����i+3�j
short sabo = 0;

// �����̗��z�����s�������F�X�e�[�W�N���A�Ńt���O���Z�b�g
bool rumor = false;

// ���������o������
bool disclos = false;

// �����̐��۔���
bool rebel = false;

// �ǂ̃{�^���������ꂽ��
short button_flag;

// �ǌ���˓�
bool add_attack = false;

// ����̍s���J�E���^
short E_time = 0;

// ����s���J�n�܂ł̃^�C���o�[�\���p
float E_timeGauge = 0;

// ���肪�s���J�n����
bool E_action = false;

// �^�C���o�[�̐F
Color timeColor;


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �t�h�֌W
  Vec2f mouse_pos;
 */

// �}�E�X�J�[�\���̍��W
Vec2f mouse_pos;

// �p�����[�^���l�̃X�L�b�v�\��
short skip = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �퓬�֌W
  int P_pow, P_attack, E_pow, E_attack;
  int P_pow0, E_pow0;
  int P_start, P_end, E_start, E_end;
  int P_damage, E_damage;

  float P_powGauge, E_powGauge;
  Color P_GaugeColor, E_GaugeColor;
  float P_Mgauge, E_Mgauge;
  float P_M0, E_M0;

  short P_lastcmd;
  short E_command;
 */

// �퓬�p�f�[�^
// pow = ���́Aattack = �U����
int P_pow, P_attack, E_pow, E_attack;

// �퓬�J�n���̃f�[�^�ۑ�
int P_pow0, E_pow0;

// �^�[���J�n���ƏI�����̕��͕ۑ�
int P_start, P_end, E_start, E_end;

// �퓬���̃_���[�W�v�Z
int P_damage, E_damage;

// ���̓Q�[�W�\���p
float P_powGauge, E_powGauge;

// ���̓Q�[�W�̐F
Color P_GaugeColor, E_GaugeColor;

// �`�E���Q�[�W�\���p
float P_Mgauge, E_Mgauge;

// �퓬�J�n���̋`�E���l���̕ۑ�
float P_M0, E_M0;

// �v���C���[�̒��O�̍s��
short P_lastcmd;

// ����̑I�������s��
short E_command;


////////////////////////////////////////////////////////////////////////////////////////////////////

