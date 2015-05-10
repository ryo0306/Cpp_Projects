//
// �����א�p �֐����X�g
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../system/system.hpp"


/* �}�N�� */
////////////////////////////////////////////////////////////////////////////////

#define DEF_PLAYER   0   // �v���C���[���


/* �Q�[����p�֐� */
////////////////////////////////////////////////////////////////////////////////

// �V�X�e�����b�Z�[�W�̕\��
void AS_disp_message(Texture& sys, Texture& frm, bool game_set, short& rank) {
  short i;      // �J��Ԃ������p
  float x, y;   // �V�X�e�����b�Z�[�W�̕\���ʒu�i�n�_�j

  static float angle1, angle2;   // ���b�Z�[�W�G�t�F�N�g�p

  // �Q�[�����[�h�̕\��
  {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (Window::HEIGHT / 2) - Telop_H * 2;
    angle1 += 0.5;
    if (angle1 > Telop_W) { angle1 = 0; }

    // �t���[���\��
    drawTextureBox(x - Number_W, y - (Telop_H / 2), Logo_W + (Number_W * 2), Logo_Hb,
      0, 0, Logo_W, Logo_Hb,
      frm, Color(1, 1, 1));

    // �e���b�v�\��
    drawTextureBox(x, y, Telop_W, Telop_H,
      angle1, Telop_H * 3, Telop_W, Telop_H,
      sys, Color(1, 1, 1));
  }

  // ������@�̕\��
  {
    x = (-Window::WIDTH / 2) + Card_W + Telop_W + Icon_W;

    if (!game_set) {
      // �J�[�h�̑I��
      drawTextureBox(x, y, Telop_W, Telop_H,
        0, Telop_H * 4, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
    else {
      if (angle2 == 180) { angle2 = 0; }
      else { angle2 += 0.1; }

      // �N���b�N�ŏI��
      drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
        0, Telop_H * 6, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // �X�R�A�̕\���F���l���̂��͕̂ʂ̊֐��ŕ\��
  // �Q�ƁFg_common.cpp -> CO_disp_number();
  if (!game_set) {
    x = (-Window::WIDTH / 2) + Icon_W * 0.5;
    y = (-Window::HEIGHT / 2) + Telop_H * 8;

    // �u��D�v
    drawTextureBox(x + Icon_W, y, Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // �u�p�X�񐔁v
    drawTextureBox(x + Icon_W * 3, y, Icon_W, Telop_H,
      Icon_W * 1, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // ���̕\��
    for (i = 0; i < PLAYER; i++) {
      if (i == DEF_PLAYER) {
        // �u�v���C���[�v
        drawTextureBox(x, y - (Telop_H + 2), Icon_W, Telop_H,
          Icon_W * 2, Telop_H * 0, Icon_W, Telop_H,
          sys, Color(1, 1, 1));
      }
      else {
        // �u����v
        drawTextureBox(x, y - (Telop_H + 2) * (1 + i), Icon_W, Telop_H,
          Icon_W * 3, Telop_H * 0, Icon_W, Telop_H,
          sys, Color(1, 1, 1));

        // �u�ΐ푊��̒N���v��\��
        drawTextureBox(x + (Number_W * 5), y - (Telop_H + 2) * (1 + i), Number_W, Telop_H,
          Number_W * (10 + i), Telop_H * 2, Number_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }
  }

  // ���U���g��ʂ̕\��
  if (game_set) {
    // �Q�[���I���̕\��
    {
      x = -(Icon_W + 2);
      y = 0;

      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 2, Telop_H * 1, Icon_W, Telop_H,
        sys, Color(1, 1, 1));
    }

    // ���ʂ̕\��
    {
      x = -(Icon_W * 1.5);
      y = -(Telop_H + 2);

      // �u���ʁv
      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 0, Telop_H * 1, Icon_W * 2, Telop_H,
        sys, Color(1, 1, 1));

      // �l�ɑΉ��������ʂ̕\��
      if (rank != 5) {
        drawTextureBox(x + (Icon_W * 3), y, Number_W * 2, Telop_H,
          Number_W * rank, Telop_H * 2, Number_W, Telop_H,
          sys, Color(1, 1, 1));
      }

      // ���^�C�A�����Ƃ�
      else {
        drawTextureBox(x + (Icon_W * 2), y, Icon_W, Telop_H,
          Icon_W * 3, Telop_H * 1, Icon_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }
  }
}

// �p�X�̕\���ƑI��
void AS_disp_pass(AppEnv& app_env, Texture& card, Vec2f& m_pos, short& turn, short& pass, short& rank) {

  float x, y;        // �摜��\��������W
  Color pass_card;   // �F���

  x = (Window::WIDTH / 2) - (Card_W + 2);
  y = (-Window::HEIGHT / 2) + 2;

  // �R�}���h���I���o������
  if (CO_on_mouse(m_pos, x, y, Card_W, Card_H)) {

    // �p�X�񐔂��O
    if (pass == 0) { pass_card = Color(1, 1, 0.75); }
    // �p�X�񐔂��O�ł͂Ȃ�
    else { pass_card = Color(1, 0.75, 0.75); }

    // ���N���b�N������p�X�񐔂𑝂₵�Ď��̃^�[���ɂ���
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      turn += 1;
      pass += 1;

      // ���^�C�A�ɂȂ�܂Ńp�X�����Ƃ�
      if (pass == RETIRE_P) { rank = RETIRE_G; }
    }
  }
  else { pass_card = Color(0.5, 0.5, 0.5); }

  // �u�p�X�v�̉摜��\��
  drawTextureBox(x, y, Card_W, Card_H,
    Card_W * 2, Card_H * 4, Card_W, Card_H,
    card, pass_card);
}

// �J�[�h�̃J���[�ύX
Color AS_set_cardcolor(bool used, bool& on_mouse, bool& is_E) {
  // �J�[�h���g�p�ς݂ł͂Ȃ��Ƃ�
  if (!used) {
    // �J�[�h�̏�Ƀ}�E�X�J�[�\�������邩����
    if (!on_mouse) { return Color(1, 1, 1); }   // �J�[�h�̏�ɂȂ�
    else {
      if (is_E) { return Color(1, 1, 0.5); }    // �J�[�h���g����
      return Color(1, 0.5, 0.5);                // �g���Ȃ�
    }
  }

  // �g�p�ς�
  else { return Color(1, 1, 1, 0.5); }
}

// �J�[�h�̃t���O����
void AS_enable_check(bool& is_L, bool& is_H, bool& is_E, short num, short left, short right) {
  // ��D�̃J�[�h�����[�A�E�[�̂ǂ���ɏo���邩����
  is_L = (num == left - 1);    // ���[�ɏo����� true
  is_H = (num == right + 1);   // �E�[�ɏo����� true

  // �ǂ��炩�ɏo����΁A�g�p�\�ɂ���
  is_E = (is_L || is_H);
}


////////////////////////////////////////////////////////////////////////////////
