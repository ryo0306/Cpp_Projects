//
// High & Low��p �֐����X�g
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "../system/system.hpp"


/* �}�N�� */
////////////////////////////////////////////////////////////////////////////////

enum HL {
  HIGHLOW = 2,      // �\��������J�[�h�̖���
  SUCCESS = 1,      // �J�[�h�I���̐���
  FAILURE = 2,      // �J�[�h�I���̎��s
  DEF_LEFT = 1,     // ���̃J�[�h��I�������Ƃ�
  DEF_RIGHT = 2,    // �E�̃J�[�h��I�������Ƃ�

  EFC_GOOD = 10,    // �G�t�F�N�g�\���F���]��
  EFC_GREAT = 20,   // �G�t�F�N�g�\���F����ɍ��]��

  TIME_FPS = 60,    // �P�b������̏����񐔁iFrame Per Second�j
  TIME_LIM = 20,    // �������ԁi�b�j�FTIME_FPS * TIME_LIM
  TIME_REP = 20     // �c�莞�ԉ񕜗ʂ̏��
};


/* �Q�[����p�֐� */
////////////////////////////////////////////////////////////////////////////////

// �V�X�e�����b�Z�[�W�̕\��
void HL_disp_message(Texture& sys, Texture& frm, bool& update, bool g_set, short& chain) {
  // �V�X�e�����b�Z�[�W�̕\���ʒu�i�n�_�j
  float x, y;

  // ���b�Z�[�W�G�t�F�N�g�p
  static float angle1, angle2, angle3;

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

    if (!g_set) {
      // �J�[�h�̑I��
      drawTextureBox(x, y, Telop_W, Telop_H,
        0, Telop_H * 4, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
    else {
      // �Q�[���I��
      drawTextureBox(x, y + abs(sin(angle2)) * 10, Telop_W, Telop_H,
        0, Telop_H * 7, Telop_W, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // �Q�[���̃q���g�\��
  if (!g_set) {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (Window::HEIGHT / 2) - (Telop_H * 4) - 2;
    angle3 += 0.5;
    if (angle3 > (Telop_W * 2)) { angle3 = 0; }

    // �t���[���\��
    drawTextureBox(x - Number_W, y - (Telop_H / 2), Logo_W + (Number_W * 2), Logo_Hb,
      0, 0, Logo_W, Logo_Hb,
      frm, Color(1, 1, 1));

    // �e���b�v�\��
    if (angle3 < Telop_W) {
      drawTextureBox(x + (Telop_W - angle3), y, angle3, Telop_H,
        0, Telop_H * 5, angle3, Telop_H,
        sys, Color(1, 1, 1));
      drawTextureBox(x, y, Telop_W - angle3, Telop_H,
        angle3, Telop_H * 6, Telop_W - angle3, Telop_H,
        sys, Color(1, 1, 1));
    }
    else {
      drawTextureBox(x + (Telop_W * 2) - angle3, y, angle3 - Telop_W, Telop_H,
        0, Telop_H * 6, angle3 - Telop_W, Telop_H,
        sys, Color(1, 1, 1));
      drawTextureBox(x, y, (Telop_W * 2) - angle3, Telop_H,
        angle3 - Telop_W, Telop_H * 5, (Telop_W * 2) - angle3, Telop_H,
        sys, Color(1, 1, 1));
    }
  }

  // �X�R�A�̕\���F���l���̂��͕̂ʂ̊֐��ŕ\��
  // �Q�ƁFg_common.cpp -> CO_disp_number();
  {
    x = (-Window::WIDTH / 2) + Card_W;
    y = (-Window::HEIGHT / 2) + Telop_H * 4.5;

    // �u�X�R�A�v
    drawTextureBox(x + (Number_W * 3), y, Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 1));

    // �u�n�C�X�R�A�v
    drawTextureBox(x, y - (Telop_H + 2), Icon_W, Telop_H,
      Icon_W * 0, Telop_H * 1, Icon_W, Telop_H,
      sys, Color(1, 1, 1));
  }

  // �A�������񐔂̕\��
  if (chain != 0) {
    x = (-Window::WIDTH / 2) + Icon_W * 6;
    y = (-Window::HEIGHT / 2) + (Telop_H * 4.5);

    // �uCHAIN!!�v
    drawTextureBox(x, y, Icon_W, Telop_H,
      Icon_W * 3, Telop_H * 0, Icon_W, Telop_H,
      sys, Color(1, 1, 0.5));

    // �񐔂̕\��
    CO_disp_number(sys, chain, x - (Icon_W / 2), y);
  }

  // ���U���g��ʂ̕\��
  if (g_set) {
    // �Q�[���I���̕\��
    {
      x = -(Icon_W + 2);
      y = 0;

      drawTextureBox(x, y, Icon_W * 2, Telop_H,
        Icon_W * 2, Telop_H * 1, Icon_W, Telop_H,
        sys, Color(1, 1, 1));
    }

    // �n�C�X�R�A�X�V�̕\��
  }
}

// �J�[�h�̃J���[�ύX
Color HL_set_cardcolor(bool& on_mouse) {
  if (!on_mouse) { return Color(1, 1, 1); }   // �J�[�h�̏�ɃJ�[�\�����Ȃ�
  else { return Color(1, 1, 0.5); }           // ����
}

// ���ԃQ�[�W�̃J���[�ύX
Color HL_set_timecolor(short& time) {
  float temp = (float)time / (TIME_FPS * TIME_LIM);
  short time0 = temp * 100;

  if (time0 > 40) { return Color(0.5, 1, 0.5); }
  else if (time0 > 20) { return Color(1, 1, 0.5); }
  else { return Color(1, 0.5, 0.5); }
}

// �J�[�h�̕\���ƑI��
bool HL_disp_card(Texture& card, Vec2f& pos, short& val, float x) {
  float scale = 1.0;
  Color card_color = Color(1, 1, 1);
  bool on_card = false;

  // �}�E�X����
  if (CO_on_mouse(pos, x - (Card_W * 0.75), -(Card_H * 0.75), Card_W * 1.5, Card_H * 1.5)) {
    scale = 1.2;
    card_color = Color(1, 1, 0.5);
    on_card = true;
  }

  // �J�[�h�̕\��
  drawTextureBox(x, 0, Card_W * 1.5, Card_H * 1.5,
    Card_W * (val % CARD_NUM), Card_H * (val / CARD_NUM), Card_W, Card_H,
    card, card_color,
    0, Vec2f(scale, scale), Vec2f(Card_W * 0.75, Card_H * 0.75));

  return on_card;
}

// �p�X�̕\���ƑI��
int HL_disp_pass(AppEnv& app, Texture& card, Vec2f& m_pos, short left, short right) {
  float x, y;        // �J�[�h�̕\���ʒu
  float scale;       // �摜�̏k��
  Color pass_card;   // �F���

  x = (Card_W / 2); y = (Card_H / 2);

  // �R�}���h���I���o������
  if (CO_on_mouse(m_pos, -x, -y, Card_W, Card_H)) {
    // ���N���b�N�����Ƃ�
    if (app.isPushButton(Mouse::LEFT)) {
      app.flushInput();

      // ���E�̃J�[�h�����������Ȃ�X�R�A�ǉ�
      return (left == right) ? SUCCESS : FAILURE;
    }
    pass_card = Color(1, 1, 0.5);
    scale = 1.2;
  }
  else { pass_card = Color(1, 1, 1); scale = 1.0; }

  // �u�p�X�v�̉摜��\��
  drawTextureBox(0, 0, Card_W, Card_H,
    Card_W * 2, Card_H * 4, Card_W, Card_H,
    card, pass_card, 0, Vec2f(scale, scale), Vec2f(x, y));

  return 0;
}

// �J�[�h��I�������Ƃ��̐��۔���
int HL_click_check(short choice, short left, short right) {
  switch (choice) {
    case  DEF_LEFT: return (left > right) ? SUCCESS : FAILURE;
    case DEF_RIGHT: return (left < right) ? SUCCESS : FAILURE;
    default: return FAILURE;
  }
}


////////////////////////////////////////////////////////////////////////////////
