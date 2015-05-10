
#pragma once
#include "../common.h"
#include "../Object/character.h"
#include <list>
#include <queue>

//
// �V�X�e���f�[�^
// TIPS: �O���ɕۑ����Ȃ��f�[�^��
//     : �f�[�^�̊Ǘ��� cDataManager �N���X�ōs��
//

class cSystem {
public:
  cSystem() = default;

  // �|�[�Y��ʔ���
  bool pause_;

  // �Q�[���̃N���A�t���O
  // TIPS: ������������؂�ւ���
  bool clear_;

  // �`��V�[���̏��
  // TIPS: �N�����̏������� cGameManager �N���X�ōs��
  // * �ȍ~�� scene_manager �N���X���ŊǗ�
  // * play::Now, Last, Next �Ŋe�i�K�̕`���Ԃ��Q�Ƃ���
  state::SceneMode scene[3];

  // �헪��ʂ̏��
  struct {
    mode mode_;          // �R�}���h�I�����
    u_short draw_state_;   // �{�^���`����
    u_short stage_id_;     // �N�U�R�}���h�I���}�X
  } strategy;

  // �퓬���Ɏg�p���ꂽ�R�}���h�̃t���O
  struct {
    bool fire_;
    bool cannon_;
    bool trap_;
    bool poison_;
  } command;

  // �퓬�V�[���̏��
  // TIPS: �퓬�A���U���g�̊e�V�[�����ŏ�����؂�ւ���
  // * phase::Win, Lose �Ń��U���g�̏�����؂�ւ���
  state::BattlePhase phase_;

  // ���j�b�g���
  std::list<cCharacter> all_enemy;   // �퓬�ɎQ���ł���S�Ă̓G
};
