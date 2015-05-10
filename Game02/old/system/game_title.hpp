//
// �^�C�g����ʂ̏���
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "system.hpp"


// �^�C�g�����
void game_title(AppEnv& app_env) {
  // �摜�ǂݍ���
  Texture title("res/png/sys01_logo.png");    // �^�C�g�����S
  Texture start("res/png/sys02_start.png");   // �X�^�[�g�{�^��
  Texture BG("res/png/bg01_gradation.png");   // �w�i

  // ���̏����������Ŏg���ϐ�
  const short One_Second = 60;   // 1�b = 60.frame
  const short Logo_Line = 8;     // �`��T�C�Y

  const short Draw_Limit = Logo_Ha - Logo_Line;   // �`��A�j���̏I������

  short time_count = 0;   // �^�C�}�[
  short blink = 0;        // �_�ŃA�j���[�V����
  float draw_y = 0;       // ���S�ω��A�j���F�`��ʒu

  while (1) {
    if (!app_env.isOpen()) { return; }

    // ���N���b�N�Ń��[�v�I��
    if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); return; }

    // �摜�̓_�ŏ���
    {
      blink = (time_count / 30) % 2;
      time_count++;
      if (time_count == One_Second) { time_count = 0; }   // 1�b�J�E���g�����烊�Z�b�g
    }

    // �^�C�g�����S�ɕω������鏈��
    {
      draw_y += 1.5;
      if (draw_y >= Draw_Limit) { draw_y = 0; }   // �`��ʒu�����Z�b�g
    }

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Bg_W, Bg_H,
      BG, Color(0, 0, 0, 0.5));

    // �^�C�g�����S�\��
    drawTextureBox((-Logo_W * 1), (Logo_Ha * 1), Logo_W * 2, Logo_Ha,
      0, 0, Logo_W, Logo_Ha,
      title, Color(1, 0.75, 0.5));
    drawTextureBox((-Logo_W * 1), (Logo_Ha * 1) + draw_y, Logo_W * 2, Logo_Line,
      0, (Logo_Ha - Logo_Line) - draw_y, Logo_W, Logo_Line,
      title, Color(1, 1, 1));

    // �N���b�N�X�^�[�g
    drawTextureBox((-Logo_W * 0.5), (-Logo_Hb * 2), Logo_W, Logo_Hb,
      0, 0, Logo_W, Logo_Hb,
      start, Color(1, 1, 0.5, blink));

    app_env.update();
  }
}
