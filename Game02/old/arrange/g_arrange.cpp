//
// ������
//

#pragma once
#include "f_arrange.hpp"   // �e�Q�[����p�̊֐��t�@�C���̓ǂݍ���


// ������ ���U���g
void result_arrange(AppEnv& app_env, Texture& sys, Texture& frm, short& rank) {
  while (1) {
    if (!app_env.isOpen()) { return; }

    // ���N���b�N�ŃQ�[���I��
    if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); break; }

    app_env.setupDraw();

    // �V�X�e�����b�Z�[�W�̕\��
    AS_disp_message(sys, frm, true, rank);

    app_env.update();
  }
}

// ������ �{��
void game_arrange(AppEnv& app_env) {
  // �����̊�l�ݒ�
  Random random; random.setSeed((u_int)time(nullptr));

  // �摜�̓ǂݍ���
  Texture t_card("res/png/c01_standard.png");   // �J�[�h�̉摜
  Texture system("res/png/m02_as.png");         // �V�X�e�����b�Z�[�W
  Texture wframe("res/png/sys04_frame.png");    // �e���b�v�p�t���[��

  // BGM �� SE �̓ǂݍ���
  Media thema(M_THEMA); thema.gain(0.5); thema.looping(true);
  Media intro(M_INTRO); intro.gain(0.5);   // �C���g��

  Media click(SE_CLICK); click.gain(0.25);         // �N���b�N�����Ƃ�
  Media finish(SE_FINISH); finish.gain(0.25);      // �Q�[���I��

  // ���̃Q�[������p�̕ϐ�
  short i, j;              // �J��Ԃ������p
  short current_mark;      // ���ݎQ�Ƃ��Ă���J�[�h�̋L��
  short game_rank = 1;     // ���݂̏���

  float card_x, card_y;    // �J�[�h�̕\���ʒu�i�n�_�j
  float tex_x, tex_y;      // �J�[�h�̐؂���ʒu�i�n�_�j
  float angle = 0.0;       // �J�[�h�摜�̉�]�G�t�F�N�g�p�F�ω���
  float scale = 1.0;       // �J�[�h�摜�̉�]�G�t�F�N�g�p�F���ۂ̕ω���
  float sys_x, sys_y;      // �V�X�e�����b�Z�[�W�̕\���ʒu

  bool isLow = false;      // ����菬�����������o���邩
  bool isHigh = false;     // �����傫���������o���邩
  bool isEnable = false;   // �I�������s�������s�ł��邩
  bool isCheck = false;    // �Q�[���I����������s������
  bool isAct = false;      // �ΐ푊���AI�p�F�s���������̔���

  // ���̃Q�[������p�̕ϐ��F�}�E�X����p
  Vec2f mouse_pos;   // �}�E�X�J�[�\���̍��W
  bool on_mouse;     // �}�E�X�J�[�\���̔���

  // ���̃Q�[������p�̕ϐ��F�v���C���[�֌W
  short player_turn = 0;           // �N�̃^�[����
  short card_count[PLAYER] = {};   // �J�[�h�̎c�薇��
  short pass_count[PLAYER] = {};   // �p�X������

  // ��D�̏��
  struct Card {
    short number;
    short mark;
    bool used;
  };

  // �J�[�h���̐���
  Card player[PLAYER][CARD_NUM] = {};   // �e�v���C���[�̎�D
  short num_left[CARD_MARK] = {};       // ��̃J�[�h�̍��[���
  short num_right[CARD_MARK] = {};      // ��̃J�[�h�̉E�[���

  // �Q�[���̏�����
  {
    bool isSwap;   // �\�[�g�m�F�p

    // �J�[�h�̃V���b�t���F�ꎞ�I�Ȕz����쐬�A�f�[�^���i�[���Ă��痐�����g���ē���ւ���
    Card cardinfo[JOKER_NG] = {};
    for (i = 0; i < JOKER_NG; i++) { cardinfo[i] = {i % CARD_NUM, i / CARD_NUM, false}; }
    for (i = 0; i < JOKER_NG; i++) { std::swap(cardinfo[i], cardinfo[random.fromZeroToLast(JOKER_NG)]); }

    // �e�v���C���[�̎�D���̏������F�����ŗp�ӂ����ꎞ�z��̓��e���e�v���C���[�̎�D�z��ɓ���ւ���
    for (i = 0; i < PLAYER; i++) {
      for (j = 0; j < CARD_NUM; j++) { std::swap(player[i][j], cardinfo[j + (i * CARD_NUM)]); }
    }

    // �v���C���[�̎�D���\�[�g
    {
      // �L�����ɑ�����
      while (1) {
        isSwap = false;
        for (i = 0; i < CARD_NUM - 1; i++) {
          if (player[DEF_PLAYER][i].mark > player[DEF_PLAYER][i + 1].mark) {
            std::swap(player[DEF_PLAYER][i], player[DEF_PLAYER][i + 1]);
            isSwap = true;
          }
        }
        if (!isSwap) { break; }   // ����ւ���K�v���Ȃ��Ȃ����烋�[�v�I��
      }

      // �L�����ƂɁA�������ɑ�����
      while (1) {
        isSwap = false;
        for (i = 0; i < CARD_NUM - 1; i++) {
          if (player[DEF_PLAYER][i].number > player[DEF_PLAYER][i + 1].number) {
            // ����ւ���J�[�h�̋L���������Ȃ����ւ������s
            if (player[DEF_PLAYER][i].mark == player[DEF_PLAYER][i + 1].mark) {
              std::swap(player[DEF_PLAYER][i], player[DEF_PLAYER][i + 1]);
              isSwap = true;
            }
          }
        }
        if (!isSwap) { break; }   // ����ւ���K�v���Ȃ��Ȃ����烋�[�v�I��
      }
    }

    // ��̃J�[�h���̏�����
    for (i = 0; i < CARD_MARK; i++) { num_left[i] = 6; num_right[i] = 6; }

    // �p�X�񐔂̏�����
    for (i = 0; i < PLAYER; i++) { pass_count[i] = 0; }

    // ��D�Ɂu�V�v�����邩�m�F
    for (i = 0; i < PLAYER; i++) {
      for (j = 0; j < CARD_NUM; j++) {
        // �u�V�v�����������ɏo������Ԃɂ���
        if (player[i][j].number == 6) { player[i][j].used = true; }
      }
    }

    intro.play();   // �C���g���Đ�
  }


  /* �{�� */
  ////////////////////////////////////////////////////////////////////////////////
  while (1) {
    if (!app_env.isOpen()) { return; }

    // BGM�Đ��F�C���g�����I������烋�[�v�����̍Đ�
    if (!intro.isPlaying()) { if (!thema.isPlaying()) { thema.play(); } }

    // �Q�[���̏I������
    if (!isCheck) {
      // �v���C���[�����^�C�A����
      if (pass_count[DEF_PLAYER] == RETIRE_P) { break; }

      // �v���C���[�̎�D���Ȃ��Ȃ���
      for (i = 0; i < CARD_NUM; i++) { if (!player[DEF_PLAYER][i].used) { break; } }
      if (i == CARD_NUM) { break; }

      isCheck = true;   // �Q�[���I�����Ȃ��Ƃ��A���̃^�[���܂Ŕ�����X�L�b�v����
    }

    app_env.setupDraw();

    // �V�X�e�����b�Z�[�W�̕\��
    {
      // ���b�Z�[�W�S��
      AS_disp_message(system, wframe, false, game_rank);

      // �v���C���[���̕\��
      for (i = 0; i < PLAYER; i++) {
        sys_x = (-Window::WIDTH / 2) + Icon_W * 2;
        sys_y = (-Window::HEIGHT / 2) + Telop_H * 8 - (Telop_H + 2);

        // ���^�C�A���Ă��Ȃ���΁A��D�̖����ƃp�X�񐔂�\��
        if (pass_count[i] < RETIRE_P) {
          // ��D�̎c�薇�����擾���āA���̒l��\��
          card_count[i] = 0;
          for (j = 0; j < CARD_NUM; j++) { if (!player[i][j].used) { card_count[i]++; } }
          CO_disp_number(system, card_count[i], sys_x, sys_y - (Telop_H + 2) * i);

          // �p�X�����񐔂�\��
          CO_disp_number(system, pass_count[i], sys_x + (Icon_W * 2), sys_y - (Telop_H + 2) * i);

          // �u�����v
          drawTextureBox(sys_x + Card_W, sys_y - (Telop_H + 2) * i, Icon_W, Telop_H,
            Icon_W * 2, Telop_H * 2, Icon_W, Telop_H,
            system, Color(1, 1, 1));

          // �u�񐔁v
          drawTextureBox(sys_x + Card_W + (Icon_W * 2), sys_y - (Telop_H + 2) * i, Icon_W, Telop_H,
            Icon_W * 3, Telop_H * 2, Icon_W, Telop_H,
            system, Color(1, 1, 1));
        }

        // ���^�C�A���Ă���Ƃ�
        else {
          // �u���^�C�A�v
          drawTextureBox(sys_x + Icon_W, sys_y - (Telop_H + 2) * i, Icon_W, Telop_H,
            Icon_W * 3, Telop_H * 1, Icon_W, Telop_H,
            system, Color(1, 1, 1));
        }
      }
    }

    // ��ɏo�Ă���J�[�h�̕\��
    for (i = 0; i < CARD_MARK; i++) {
      for (j = num_left[i]; j <= num_right[i]; j++) {
        // �\���ʒu�̐ݒ�
        card_x = (Card_W + 2) * (j - 6.5);
        card_y = (Card_H + 2) * (2 - i);

        // �J�[�h�̕\��
        drawTextureBox(card_x, card_y, Card_W, Card_H,
          Card_W * j, Card_H * i, Card_W, Card_H,
          t_card, Color(1, 1, 1));
      }
    }

    // ���^�C�A���ĂȂ���Ώ��������s
    if (pass_count[player_turn] < RETIRE_P) {

      // �v���C���[�Ƒΐ푊��ŏ����𕪊�
      switch (player_turn) {

          // �v���C���[�̏���
        case DEF_PLAYER:
          mouse_pos = app_env.mousePosition();   // �}�E�X���W�̎擾

          // ��D�̏���
          for (i = 0; i < CARD_NUM; i++) {
            // ��D�̕\���ʒu��ݒ�
            card_x = (-Window::WIDTH / 2) + 2 + (Card_W + 2) * i;
            card_y = (-Window::HEIGHT / 2) + 2;

            // �t���O�̃��Z�b�g
            isLow = false; isHigh = false; isEnable = false;

            // �J�[�h�̏�Ƀ}�E�X�J�[�\�������邩����
            on_mouse = CO_on_mouse(mouse_pos, card_x, card_y, Card_W, Card_H);
            if (on_mouse) {
              // �܂���ɏo���Ă��Ȃ��J�[�h�������Ƃ�
              if (!player[DEF_PLAYER][i].used) {
                // �I�����悤�Ƃ��Ă���J�[�h�̋L�����擾
                current_mark = player[DEF_PLAYER][i].mark;

                // ��̃J�[�h�Ǝ�D�̃J�[�h���r�A��ɏo���邩���肷��
                AS_enable_check(isLow, isHigh, isEnable,
                  player[DEF_PLAYER][i].number, num_left[current_mark], num_right[current_mark]);

                // �J�[�h���g������A�J�[�h����]������
                if (isEnable) {
                  scale = fabs(sin(angle));
                  angle = fmod(angle + 0.05, M_PI * 2);
                }
              }
            }

            // �J�[�h�̕\��
            if (isEnable) {
              // ��]�p�ŉ摜��؂�ւ���
              tex_x = (angle < M_PI) ? Card_W * player[DEF_PLAYER][i].number : Card_W * 1;
              tex_y = (angle < M_PI) ? Card_H * player[DEF_PLAYER][i].mark : Card_H * 4;

              // �g�p�\�ȃJ�[�h�̕\��
              drawTextureBox(card_x + (Card_W / 2), card_y, Card_W, Card_H,
                tex_x, tex_y, Card_W, Card_H,
                t_card, AS_set_cardcolor(player[DEF_PLAYER][i].used, on_mouse, isEnable),
                0, Vec2f(scale, 1.2), Vec2f(Card_W / 2, 0));
            }
            else {
              // �g�p�ł��Ȃ��J�[�h�̕\��
              drawTextureBox(card_x, card_y, Card_W, Card_H,
                Card_W * player[DEF_PLAYER][i].number, Card_H * player[DEF_PLAYER][i].mark, Card_W, Card_H,
                t_card, AS_set_cardcolor(player[DEF_PLAYER][i].used, on_mouse, isEnable));
            }

            // �J�[�h���g�����ԂŁA���N���b�N�������̏���
            if (isEnable && app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click.play();   // SE�Đ�

              // ��D����o�������Ƃɂ��āA��Ǝ�D�Ɍ��ʂ𔽉f������
              if (isLow) { num_left[current_mark]--; }     // �����ɏo����J�[�h�̏���
              if (isHigh) { num_right[current_mark]++; }   // �E���ɏo����J�[�h�̏���
              player[DEF_PLAYER][i].used = true;           // ��D����J�[�h���o������Ԃɂ���

              player_turn++;                // �ΐ푊��̃^�[���ɂ���
              pass_count[DEF_PLAYER] = 0;   // �p�X�񐔂����Z�b�g
              isCheck = false;              // �Q�[���I������t���O�����Z�b�g
            }

            // �I���ł���J�[�h���Ȃ��Ƃ��́u�p�X�v��\��
            AS_disp_pass(app_env, t_card, mouse_pos, player_turn, pass_count[DEF_PLAYER], game_rank);
          }

          break;   // �v���C���[�̏��� �I��

          // �ΐ푊��̏���
        default:
          // �t���O�����Z�b�g
          isAct = false; isLow = false; isHigh = false; isCheck = false;

          // ��D�̖������O�łȂ���Ώ��������s
          if (card_count[player_turn] != 0) {
            // ��D�����ɏo����J�[�h�������������_�ŏ�ɏo���Ď��̃^�[���ɂ���
            for (i = 0; i < CARD_NUM; i++) {
              // �J�[�h�̋L�����擾
              current_mark = player[player_turn][i].mark;

              // ���E�ǂ���ɏo���邩����
              isLow = (player[player_turn][i].number == num_left[current_mark] - 1);
              isHigh = (player[player_turn][i].number == num_right[current_mark] + 1);

              // ��ɏo����J�[�h��������A��ɔ��f�����ď������I��
              if (isLow || isHigh) {
                if (isLow) { num_left[current_mark]--; }     // �������X�V
                if (isHigh) { num_right[current_mark]++; }   // �E�����X�V
                player[player_turn][i].used = true;          // ��ɏo�������Ƃɂ���
                pass_count[player_turn] = 0;                 // �s�������̂Ńp�X�񐔂����Z�b�g

                isAct = true;   // ��ɃJ�[�h���o�����̂ŁA�p�X���Ȃ��悤�ɂ���
                break;
              }
            }

            // ��D�̎c�薇�����Ċm�F���āA�O�ɂȂ����珇�ʂ��J�E���g
            for (j = 0; j < CARD_NUM; j++) { if (!player[player_turn][j].used) { break; } }
            if (j == CARD_NUM) { game_rank++; }

            // ��ɏo����J�[�h���Ȃ�������p�X�񐔂��J�E���g
            if (!isAct) { pass_count[player_turn]++; }
          }

          // �s�������玟�̃^�[���ɐi�߂āA�S�����s��������v���C���[�̃^�[���ɖ߂�
          player_turn++;
          if (player_turn >= PLAYER) { player_turn = 0; }

          break;   // �ΐ푊��̏��� �I��
      }
    }

    // ���^�C�A�����v���C���[�̏���
    else {
      // �t���O�����Z�b�g
      isAct = false; isLow = false; isHigh = false; isCheck = false;

      // �J�[�h���o���Ȃ��Ȃ�܂ŏo���؂�
      for (i = 0; i < CARD_NUM; i++) {
        // �Q�Ƃ��Ă���J�[�h�̋L�����擾
        current_mark = player[player_turn][i].mark;

        // ���E�ǂ���ɏo���邩����
        isLow = (player[player_turn][i].number == num_left[current_mark] - 1);
        isHigh = (player[player_turn][i].number == num_right[current_mark] + 1);

        // ��ɏo����J�[�h��������A��ɔ��f�����ď������I��
        if (isLow || isHigh) {
          if (isLow) { num_left[current_mark]--; }     // �������X�V
          if (isHigh) { num_right[current_mark]++; }   // �E�����X�V
          player[player_turn][i].used = true;          // ��ɏo�������Ƃɂ���
        }
      }

      // ���̃^�[���ɂ���
      player_turn++;
      if (player_turn >= PLAYER) { player_turn = 0; }
    }

    app_env.update();
  } // �{�ҁ@�I��

  // ���U���g���
  thema.stop();
  finish.play();
  result_arrange(app_env, system, wframe, game_rank);
}
