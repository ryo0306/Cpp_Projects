//
// High & Low
//

#pragma once
#include "f_highlow.hpp"   // �e�Q�[����p�̊֐��t�@�C���̓ǂݍ���
#include <fstream>


// High & Low ���U���g
void result_highlow(AppEnv& app_env, Texture& sys, Texture& frm,
  bool& upd, short& chain, int& new_sc, int& old_sc) {

  // �V�X�e�����b�Z�[�W�̕\���ʒu�i�n�_�j
  float sys_x, sys_y;

  // �N���b�N���󂯕t���Ȃ����ԁF�P�b
  short stop_time = TIME_FPS;

  while (1) {
    if (!app_env.isOpen()) { return; }

    // �P�b�Ԃ̓N���b�N������󂯕t���Ȃ�
    if (stop_time != 0) { --stop_time; }
    else {
      // ���N���b�N�ŃQ�[���I��
      if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); break; }
    }

    app_env.setupDraw();

    // �V�X�e�����b�Z�[�W�̕\��
    {
      // ���b�Z�[�W�S��
      HL_disp_message(sys, frm, upd, true, chain);

      // �\���ʒu�̐ݒ�
      sys_x = (-Window::WIDTH / 2) + Icon_W * 3;
      sys_y = (-Window::HEIGHT / 2) + Telop_H * 4.5;

      // �X�R�A�ƃn�C�X�R�A�̕\��
      CO_disp_number(sys, new_sc, sys_x, sys_y);
      CO_disp_number(sys, old_sc, sys_x, sys_y - (Telop_H + 2));
    }

    app_env.update();
  }
}

// High & Low �{��
void game_highlow(AppEnv& app_env) {
  // �����̊�l�ݒ�
  Random r; r.setSeed((u_int)time(nullptr));

  // �摜�̓ǂݍ���
  Texture t_card("res/png/c01_standard.png");   // �J�[�h�̉摜
  Texture system("res/png/m03_hl.png");         // �V�X�e�����b�Z�[�W
  Texture wframe("res/png/sys04_frame.png");    // �e���b�v�p�t���[��

  // BGM �� SE �̓ǂݍ���
  Media thema(M_THEMA); thema.gain(0.5); thema.looping(true);
  Media intro(M_INTRO); intro.gain(0.5);   // �C���g��

  Media success(SE_SUCCESS); success.gain(0.25);   // �N���b�N����
  Media failure(SE_FAILURE); failure.gain(0.25);   // �N���b�N���s
  Media finish(SE_FINISH); finish.gain(0.25);      // �Q�[���I��

  // ���̃Q�[������p�̕ϐ�
  short i;                  // �J��Ԃ������p
  short card_count = 0;     // ���ݎQ�Ƃ��Ă���J�[�h�z��̏ꏊ
  short click_chain = 0;    // �J�[�h��I�������Ƃ��̘A��������
  short time_bonus = 0;     // �N���b�N���������Ƃ��̎c�莞�ԉ񕜗�
  short isAct, reAct;       // �N���b�N�����Ƃ��̐��۔���
  short c_left, c_right;    // �J�[�h�̐���

  int game_score = 0;       // ���݂̃X�R�A
  int hi_score = 0;         // ����܂ł̍ō����_�i�f�[�^������ΊJ�n���ɓǂݍ��݁j

  float sys_x, sys_y;       // �V�X�e�����b�Z�[�W�̕\���ʒu�i�n�_�j

  bool isReset = true;      // �J�[�h��S�Ďg���؂����Ƃ��̃��Z�b�g����
  bool update = false;      // �n�C�X�R�A�X�V�t���O

  // ���̃Q�[������p�̕ϐ��F�}�E�X����p
  Vec2f mouse_pos;   // �}�E�X�J�[�\���̍��W

  // ���̃Q�[������p�̕ϐ��F�G�t�F�N�g�����p
  short isClick;         // �N���b�N����̕ۑ��p
  short fade_time = 0;   // �t�F�[�h�A�E�g�����𑱂��鎞��

  float fade_out;        // �e���b�v�̃t�F�[�h�A�E�g
  float efc_x, efc_y;    // �G�t�F�N�g�p�e���b�v�̐؂���ʒu
  float efc_scale;       // �G�t�F�N�g�p�e���b�v�̏k��

  Color efc_color;       // �G�t�F�N�g�p�J���[

  // �Q�[���I���܂ł̐�������
  short time_count = TIME_FPS * TIME_LIM;

  // ��̃J�[�h���
  short cardinfo[JOKER_NG] = {};

  // �Q�[���̏�����
  {
    // �n�C�X�R�A�̓ǂݍ���
    std::ifstream fstr("res/score.txt");
    if (fstr) { fstr >> hi_score; }
    else { hi_score = 0; }

    intro.play();   // �C���g���Đ�
  }


  /* �{�� */
  ////////////////////////////////////////////////////////////////////////////////
  while (1) {
    if (!app_env.isOpen()) { return; }

    // BGM�Đ��F�C���g�����I������烋�[�v�����̍Đ�
    if (!intro.isPlaying()) { if (!thema.isPlaying()) { thema.play(); } }

    // �J�[�h���g���؂����������x�V���b�t�����čŏ��̃J�[�h����n�߂�
    if (isReset) {
      for (i = 0; i < JOKER_NG; i++) { cardinfo[i] = i; }
      for (i = 0; i < JOKER_NG; i++) { std::swap(cardinfo[i], cardinfo[r.fromZeroToLast(JOKER_NG)]); }
      isReset = false;
      card_count = 0;
    }

    // �������Ԃ̏���
    {
      if (time_count <= 0) { thema.stop(); break; }   // ���Ԑ؂�Ȃ�Q�[���I��
      time_count--;
    }

    mouse_pos = app_env.mousePosition();   // �}�E�X���W�̎擾

    app_env.setupDraw();

    // �V�X�e�����b�Z�[�W�̕\��
    {
      // ���b�Z�[�W�S��
      HL_disp_message(system, wframe, update, false, click_chain);

      // �\���ʒu�̐ݒ�
      sys_x = (-Window::WIDTH / 2) + Icon_W * 3;
      sys_y = (-Window::HEIGHT / 2) + Telop_H * 4.5;

      // �X�R�A�ƃn�C�X�R�A�̕\��
      CO_disp_number(system, game_score, sys_x, sys_y);
      CO_disp_number(system, hi_score, sys_x, sys_y - (Telop_H + 2));
    }

    // �c�莞�Ԃ̕\��
    {
      // �\���ʒu�̐ݒ�
      sys_x = (-Window::WIDTH / 2) + Card_W;
      sys_y = (-Window::HEIGHT / 2) + Telop_H * 1.5;

      // �u�c�莞�ԁv
      drawTextureBox(sys_x, sys_y, Icon_W, Telop_H,
        Icon_W * 3, Telop_H * 2, Icon_W, Telop_H,
        system, Color(1, 1, 1));

      // �c�莞�Ԃ̕\��
      drawFillBox(sys_x + Icon_W, sys_y, (TIME_LIM * 40), Telop_H,
        Color(0.5, 0.5, 0.5));
      drawFillBox(sys_x + Icon_W, sys_y, ((float)time_count / TIME_FPS) * 40, Telop_H,
        HL_set_timecolor(time_count));
    }

    // ���E���ꂼ��̐������擾
    if (card_count < (JOKER_NG - 1)) {
      c_left = cardinfo[card_count];
      c_right = cardinfo[card_count + 1];
    }

    // �J�[�h�̏���
    {
      isAct = 0; reAct = 0;
      if (HL_disp_card(t_card, mouse_pos, c_left, -(Card_W * 3))) { isAct = DEF_LEFT; }
      if (HL_disp_card(t_card, mouse_pos, c_right, (Card_W * 3))) { isAct = DEF_RIGHT; }

      // �J�[�h�̏�Ƀ}�E�X�J�[�\��������΁A�N���b�N�ł���悤�ɂ���
      if (isAct) {
        // ���N���b�N�������̏���
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();

          // �N���b�N����F������������X�R�A�l��
          reAct = HL_click_check(isAct, c_left % CARD_NUM, c_right % CARD_NUM);
          if (reAct == SUCCESS) {
            // �N���b�N�����񐔂��J�E���g�A�X�R�A�����_����
            click_chain++;
            game_score += 1 + (click_chain / 10);

            // �����񐔂ɉ������A�c�莞�Ԃ���
            time_bonus = click_chain;
            if (time_bonus > TIME_REP) { time_bonus = TIME_REP; }
            time_count += time_bonus;

            success.play();   // SE�Đ�
          }

          // �������J�[�h�������Ƃ�
          else {
            click_chain = 0;          // �����񐔂̃J�E���g�����Z�b�g
            time_count -= TIME_REP;   // �c�莞�Ԃ����炷

            failure.play();   // SE�Đ�
          }

          fade_time = 2 * TIME_FPS;   // �G�t�F�N�g���Q�b�ԕ\��
          isClick = reAct;            // ���茋�ʂ����̃N���b�N�܂ŕۑ�
          card_count += HIGHLOW;      // ���̃J�[�h��\������
        }
      }
    }

    // �p�X�̏���
    {
      // �p�X���N���b�N�����Ƃ��A���������s
      reAct = HL_disp_pass(app_env, t_card, mouse_pos, c_left % CARD_NUM, c_right % CARD_NUM);
      if (reAct) {
        switch (reAct) {
            // �p�X����
          case SUCCESS:
            // �N���b�N�����񐔂��J�E���g�A�X�R�A�����_����
            click_chain++;
            game_score += 1 + (click_chain / 10);

            // �����񐔂ɉ������A�c�莞�Ԃ���
            time_bonus = click_chain;
            if (time_bonus > TIME_REP) { time_bonus = TIME_REP; }
            time_count += time_bonus;

            success.play();   // SE�Đ�
            break;

            // �p�X���s
          case FAILURE:
            click_chain = 0;          // �����񐔂̃J�E���g�����Z�b�g
            time_count -= TIME_REP;   // �c�莞�Ԃ����炷

            failure.play();   // SE�Đ�
            break;
        }

        fade_time = 2 * TIME_FPS;   // �G�t�F�N�g���Q�b�ԕ\��
        isClick = reAct;            // ���茋�ʂ����̃N���b�N�܂ŕۑ�
        card_count += HIGHLOW;      // ���̃J�[�h��\������
      }
    }

    // �J�[�h���g���؂�����V���b�t���ł���悤�ɂ���
    if (card_count >= (JOKER_NG - HIGHLOW)) { isReset = true; }

    // �J�[�h���N���b�N�����Ƃ��ɃG�t�F�N�g�����J�n
    if (fade_time != 0) {
      --fade_time;
      fade_out = ((float)fade_time / TIME_FPS) / 2;

      // �G�t�F�N�g�̐ݒ�
      switch (isClick) {
          // ����
        case SUCCESS:
          efc_x = Icon_W; efc_y = 0;

          if (click_chain < EFC_GOOD) {
            efc_color = Color(1, 1, 1, fade_out);
            efc_scale = 1.0;
          }
          else if (click_chain < EFC_GREAT) {
            efc_color = Color(1, 1, 0.5, fade_out);
            efc_scale = 1.25;
          }
          else {
            efc_x = Icon_W * 2;
            efc_color = Color(1, 0.5, 0.5, fade_out);
            efc_scale = 1.5;
          }
          break;

          // ���s
        case FAILURE:
          efc_x = Icon_W * 3; efc_y = Telop_H;
          efc_color = Color(0.8, 0.8, 1, fade_out);
          efc_scale = 1;
          break;
      }

      // �G�t�F�N�g�\��
      drawTextureBox(Number_W, (Telop_H * 4) - (Telop_H * fade_out), Icon_W, Telop_H,
        efc_x, efc_y, Icon_W, Telop_H,
        system, efc_color,
        0, Vec2f(efc_scale, efc_scale), Vec2f(Icon_W / 2, Telop_H / 2));
    }

    app_env.update();
  } // �{�� �I��

  // �n�C�X�R�A�̕ۑ�
  update = false;
  if (game_score > hi_score) {
    hi_score = game_score;
    update = true;
    std::ofstream fstr("res/score.txt");
    if (fstr) { fstr << hi_score; }
  }

  // ���U���g��ʂ̏���
  finish.play();
  result_highlow(app_env, system, wframe, update, click_chain, game_score, hi_score);
}
