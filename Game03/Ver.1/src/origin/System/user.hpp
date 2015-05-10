
#pragma once
#include "../common.h"
#include "../Object/character.h"
#include <list>

//
// ���[�U�[�f�[�^
// TIPS: �O���ɕۑ�����f�[�^��
//     : �f�[�^�̊Ǘ��� cDataManager �N���X�ōs��
//

class cUser {
public:
  cUser() = default;

  // �Q�[���̓�Փx
  // TIPS: �V�K�Q�[���J�n�������̂ݒl��ݒ�
  short dif_;

  // �ǂݍ��ރ}�b�v�f�[�^�Ɗl���ςݔ���
  // TIPS: �V�K�Q�[���J�n�������̂݁A�����_���ɒl��ݒ�
  struct {
    bool flag_;           // ���L�t���O
    bool lose_;           // ���łɐ퓬�ς݁��P��͔s�k���Ă���
    u_short id_;          // �ǂݍ��ރf�[�^�̔ԍ�
    u_short attribute_;   // �������
  } map_info[design::AllStage];

  // �e��J�E���g
  // TIPS: �Q�[���̓�Փx�A�G���f�B���O�̕]���Ŏg�p
  struct {
    u_short clear_;     // �Q�[���̃N���A��
    u_short battle_;    // �퓬��
    u_short command_;   // �헪�R�}���h�����s������
    u_short worker_;    // �����J��F���m�h���̉�
  } count;

  // �헪�R�}���h
  // TIPS: �|�[�Y��ʂŎg�p����
  struct {
    u_int money_;      // �J���p����

    u_short fire_;     // �Ή��r
    u_short cannon_;   // �C��
    u_short trap_;     // �
    u_short poison_;   // �Ŗ�
  } weapon;

  // ���j�b�g���
  std::list<cCharacter> player;   // �v���C���[
};
