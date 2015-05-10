
#pragma once


namespace literal_common {

  // �d�l
  enum Specification {
    Fps = 60,

    BattleMember = 5,   // �퓬�ɎQ���ł���l���̏��
    Reward = 100,       // �G��|�������̏܋��̊�{�z

    All_Stage = 12,     // �X�e�[�W�̐�
    Boss_Stage = 11,    // �{�X���o���������
  };

  // ��Փx
  enum Difficulty {
    EASY,
    NORMAL,
    HARD,
  };
}

typedef literal_common::Specification  spec;
typedef literal_common::Difficulty     dif;
