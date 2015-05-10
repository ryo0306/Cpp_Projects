
#pragma once
#include "../Object/character.h"
#include "../Literal/literal_scene.hpp"
#include <array>
#include <queue>


class cSystemData {
public:
  cSystemData() = default;

  // �Q�[���̃N���A�t���O
  bool game_clear_;

  // �`���Ԃƒi�K
  // TIPS: play::Now, Next, Last �Ŋe�i�K�̕`���Ԃ��Q�Ƃ���
  std::array<scene, play::All_State> scene_;
  // NOTICE: �������̂� game_player �ōs��
  //       : �ȍ~�� scene_manager �ŊǗ�

  // �|�[�Y��ʂ̃t���O
  // TIPS: �|�[�Y���Ȃ� true ��ێ�
  bool pause_;

  // �퓬�������
  struct {
    short draw_;   // ���o�̕`����
    short mode_;   // �R�}���h�̑I�����
  } strategy_state;

  // �G�L�����֌W
  struct {
    short reward_;
    std::queue<cCharacter> order_;
  } enemy;
};
