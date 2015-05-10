//
// �{�҃Q�[�����[�h�I����ʂ̏���
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "system.hpp"


// �Q�[�����[�h�̑I��
int game_select(AppEnv& app_env) {
  // �摜�ǂݍ���
  Texture title("res/png/sys01_logo.png");      // �^�C�g�����S
  Texture button("res/png/sys03_button.png");   // �Q�[�����[�h
  Texture BG("res/png/bg01_gradation.png");     // �w�i
  Texture smoke1("res/png/bg02_smoke1.png");    // �w�i�F�G�t�F�N�g�P
  Texture smoke2("res/png/bg02_smoke2.png");    // �w�i�F�G�t�F�N�g�Q

  // ���̏����������Ŏg���ϐ�
  const short Game_Mode = 4;   // �Q�[�����[�h�̐�
  const short Mode_W = 192;    // �{�^���摜�F��
  const short Mode_H = 48;     // �{�^���摜�F����

  short i;                // �J��Ԃ������p
  float x, y;             // �{�^���摜�̕\���ʒu
  float angle1, angle2;   // �G�t�F�N�g�p
  Color button_color;     // �{�^���J���[�̕ύX
  Color effect_color;     // �G�t�F�N�g�p�J���[�ݒ�
  Vec2f mouse_pos;        // �}�E�X�J�[�\�����W

  effect_color = Color(1, 1, 1, 0.5);

  while (1) {
    if (!app_env.isOpen()) { return 0; }

    // �}�E�X���W�̎擾
    mouse_pos = app_env.mousePosition();

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Bg_W, Bg_H,
      BG, Color(0, 0, 0, 0.5));

    // �w�i�G�t�F�N�g�\��
    {
      angle1 += 1; if (angle1 >= Bg_W) { angle1 = 0.0; }
      angle2 += 2; if (angle2 >= Bg_W) { angle2 = 0.0; }

      // �G�t�F�N�g�P
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        angle1, 0, Bg_W, Bg_H,
        smoke1, effect_color);
      // �G�t�F�N�g�Q
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        angle2, 0, Bg_W, Bg_H,
        smoke2, effect_color);
    }

    // �^�C�g�����S�\��
    drawTextureBox((-Logo_W * 1), (Logo_Ha * 1), Logo_W * 2, Logo_Ha,
      0, 0, Logo_W, Logo_Ha,
      title, Color(1, 0.75, 0.5));

    // �I�����̏���
    for (i = 0; i < Game_Mode; i += 1) {
      x = (Mode_W * 2) * (i % 2) - (Mode_W * 1.75);
      y = (-Mode_H * 2) * (i / 2);

      // �{�^���̏�Ƀ}�E�X�J�[�\��������΃J���[�ύX
      if (CO_on_mouse(mouse_pos, x, y, Mode_W * 1.5, Mode_H * 1.5)) {
        button_color = Color(0.6, 0.8, 1);

        // ���N���b�N�Ń��[�v�I��
        if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); return i; }
      }
      else { button_color = Color(1, 1, 1); }

      // �{�^���̕\��
      drawTextureBox(x, y, Mode_W * 1.5, Mode_H * 1.5,
        0, Mode_H * i, Mode_W, Mode_H,
        button, button_color);
    }

    app_env.update();
  }

  return 0;
}
