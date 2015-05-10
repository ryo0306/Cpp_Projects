
#pragma once


namespace literal_map {

  // �}�b�v���
  enum MapInfo {
    Grass,
    Forest,
    Mount,

    All_Map,
  };

  // �}�b�v�̓������
  enum MapEffect {
    Plain,

    Fierce,       // �O��̍U���́E�A�b�v�A�h��́E�_�E��
    Fortress,     // ���̖h��́E�A�b�v�A�U���́E�_�E��
    Slowly,       // �S�̂̕q���E�_�E��
    Unlucky,      // �S�̂̍K�^�E�_�E��
    Steep,        // �X�J�E�g�ȊO�̕q���E��_�E��

    Weapon_Up,    // ����̌��ʂ��A�b�v

    Bad_Fire,     // ���܂ɖ�����l�Ƀ_���[�W
    Bad_Bomb,     // ���܂ɖ����S�̂Ƀ_���[�W
    Bad_Trap,     // �����S�̂�㩏�ԂŃX�^�[�g
    Bad_Poison,   // �����S�̂��ŏ�ԂŃX�^�[�g

    All_Effect,
  };
}

typedef literal_map::MapInfo    map_id;
typedef literal_map::MapEffect  effect;
