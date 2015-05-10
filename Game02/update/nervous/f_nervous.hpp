//
// �_�o�����p �֐����X�g
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../system/system.hpp"


/* �}�N�� */
////////////////////////////////////////////////////////////////////////////////

#define DEF_PLAYER   0   // �v���C���[���
#define DEF_RIVAL    1   // �ΐ푊��̏��


/* �Q�[����p�֐� */
////////////////////////////////////////////////////////////////////////////////

// �V�X�e�����b�Z�[�W�̕\��
void NV_disp_message(Texture& sys, Texture& frm, bool& isCont, bool& g_set, short& chain, short score[]) {
  float x, y;   // �V�X�e�����b�Z�[�W�\���ʒu�i�n�_�j
  short z;      // �����N�\���p

  // ���b�Z�[�W�G�t�F�N�g�p
  static float angle1, angle2;
  static short blink;

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

    if (angle2 == 180) { angle2 = 0; }
    else { angle2 += 0.1; }

    blink += 1;
    if (blink == 60) { blink = 0; }

    if (!g_set) {
      if (isCont) {
        // �J�[�h�̑I��
        drawTextureBox(x, y, Telop_W, Telop_H,
          0, Telop_H * 4, Telop_W, Telop_H,
          sys, Color(1, 1, 1, (blink / 30)));
      }
      else {
        // ���̃^�[���ɂ���
        drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
          0, Telop_H * 5, Telop_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }
    else {
      // �Q�[���I��
      drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
        0, Telop_H * 6, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // �X�R�A�̕\���F���l���̂��͕̂ʂ̊֐��ŕ\��
  // �Q�ƁFg_common.cpp -> CO_disp_number();
  {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (-Window::HEIGHT / 2) + Telop_H * 5;

    // �u�_���v
    drawTextureBox(x, y, Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // �u�v���C���[�v
    drawTextureBox(x + Icon_W * 0.25, y - (Telop_H + 2) * 1, Icon_W, Telop_H,
      Icon_W * 1, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // �u����v
    drawTextureBox(x + Icon_W * 0.25, y - (Telop_H + 2) * 2, Icon_W, Telop_H,
      Icon_W * 2, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));
  }

  // �A�������񐔂̕\��
  if (chain != 0 && !g_set) {
    x = (-Window::WIDTH / 2) + Card_W + Icon_W * 5;
    y = (-Window::HEIGHT / 2) + (Telop_H * 4) - 2;

    // �uCHAIN!!�v
    drawTextureBox(x, y, Icon_W, Telop_H,
      Icon_W * 3, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 0.5));

    // �񐔂̕\��
    CO_disp_number(sys, chain, x - Icon_W, y);
  }

  // ���U���g��ʂ̏���
  if (g_set) {
    // ���ʂ̕\��
    {
      x = (-Window::WIDTH / 2) + Card_W + Telop_W + Icon_W;
      y = (-Window::HEIGHT / 2) + (Telop_H * 5) - (Telop_H + 2);

      // �u���ʁv
      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 0, Telop_H * 1, Icon_W * 2, Telop_H,
        sys, Color(1, 1, 1));

      // �_�����r���āA����̂ق����悩������Q�ʂ�\��
      {
        z = 1;
        if (score[DEF_PLAYER] < score[DEF_RIVAL]) { z += 1; }

        drawTextureBox(x + (Icon_W * 2) + Number_W, y, Number_W * 2, Telop_H,
          Number_W * z, Telop_H * 2, Number_W, Telop_H,
          sys, Color(1, 1, 1));
      }
    }

    // �Q�[���I���̕\��
    {
      x = -Icon_W;
      y = 0;

      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 2, Telop_H * 1, Icon_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }
}

// �J�[�h�̃J���[�ύX
Color NV_set_cardcolor(bool& on_mouse) {
  if (!on_mouse) { return Color(1, 1, 1); }   // �J�[�h�̏�ɃJ�[�\�����Ȃ�
  else { return Color(1, 1, 0.5); }           // ����
}


////////////////////////////////////////////////////////////////////////////////
