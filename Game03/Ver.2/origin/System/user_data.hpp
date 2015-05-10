
#pragma once
#include "../Object/character.h"
#include <array>
#include <list>


class cUserData {
  struct MapInfo {
    bool territory_;
    bool lose_;
    short id_;
    short attribute_;
  };

public:
  cUserData() = default;

  // �Q�[���̓�Փx
  // TIPS: �V�K�Q�[���J�n�������̂ݕύX��������
  short dif_;

  // ���Y
  struct {
    u_int money_;   // ������
    short food_;    // �H�ƁF���m�h���̎c���
  } assets;

  // TIPS: �Q�[���̐i�s��ԂȂǁA�V�X�e���Ŏg�p����J�E���g
  struct {
    short game_clear_;   // �Q�[���̃N���A��
    short battle_;       // �퓬��
    short lose_;         // �s�k�A�P�ނ�����
    short battle_cmd_;   // �퓬���̃R�}���h�g�p��
    short work_;         // ���m�h���̉�
  } cnt;

  // TIPS: �J����������̎c��g�p��
  struct {
    short fire_;     // �Ή��r
    short bomb_;     // �C��
    short trap_;     // �
    short poison_;   // �Ŗ�
  } weapon_cnt;

  // �}�b�v�f�[�^
  // TIPS: �t���O�Ɠǂݍ��މ摜��ID�A�}�b�v�̌��ʂ�ێ�
  // * �V�K�Q�[���J�n�����Ƃ��̂ݕύX��������
  std::array<MapInfo, spec::All_Stage> map_info_;

  // ���j�b�g���
  std::list<cCharacter> player;
};
