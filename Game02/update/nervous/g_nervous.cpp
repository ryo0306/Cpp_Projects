//
// �_�o����
//

#pragma once
#include "f_nervous.hpp"   // �e�Q�[����p�̊֐��t�@�C���̓ǂݍ���


// �_�o���� ���U���g
void result_nervous(AppEnv& app_env, Texture& sys, Texture& frm,
  bool& isCont, bool& g_set, short& chain, short p_score, short r_score) {

  // �V�X�e�����b�Z�[�W�̕\���ʒu
  float sys_x, sys_y;

  // �X�R�A���ꎞ�I�ɕۑ�
  short score[] = {p_score, r_score};

  while (1) {
    if (!app_env.isOpen()) { return; }

    // ���N���b�N�ŏI��
    if (app_env.isPushButton(Mouse::LEFT)) { app_env.flushInput(); break; }

    app_env.setupDraw();

    // �V�X�e�����b�Z�[�W�̕\��
    NV_disp_message(sys, frm, isCont, g_set, chain, score);

    // �X�R�A�̕\��
    {
      sys_x = (-Window::WIDTH / 2) + Card_W + Icon_W * 2;
      sys_y = (-Window::HEIGHT / 2) + (Telop_H * 4) - 2;

      CO_disp_number(sys, p_score, sys_x, sys_y);                   // �v���C���[�̓_��
      CO_disp_number(sys, r_score, sys_x, sys_y - (Telop_H + 2));   // �ΐ푊��̓_��
    }

    app_env.update();
  }
}

// �_�o���� �{��
void game_nervous(AppEnv& app_env) {
  // �����̊�l�ݒ�
  Random random; random.setSeed((u_int)time(nullptr));

  // �摜�̓ǂݍ���
  Texture t_card("res/png/c01_standard.png");   // �J�[�h�̉摜
  Texture system("res/png/m01_nv.png");         // �V�X�e�����b�Z�[�W
  Texture wframe("res/png/sys04_frame.png");    // �e���b�v�p�t���[��

  // BGM �� SE �̓ǂݍ���
  Media thema(M_THEMA); thema.gain(0.5); thema.looping(true);
  Media intro(M_INTRO); intro.gain(0.5);   // �C���g��

  Media click(SE_CLICK); click.gain(0.5);         // �N���b�N�����Ƃ�
  Media success(SE_SUCCESS); success.gain(0.25);   // �J�[�h�̃y�A����ꂽ
  Media failure(SE_FAILURE); failure.gain(0.25);   // �J�[�h�̃y�A�����Ȃ�����
  Media finish(SE_FINISH); finish.gain(0.25);      // �Q�[���I��

  // ���̃Q�[������p�̕ϐ�
  short i;                  // �J��Ԃ������p
  short open_count = 0;     // �J�[�h���߂�������
  short open_chain = 0;     // �A���Ńy�A����������
  short hist_count = 0;     // �����ɏ���ۑ�������
  short field_count = 0;    // ��Ɏc���Ă���J�[�h�̖���

  float card_x, card_y;     // �J�[�h�̕\���ʒu�i�n�_�j
  float sys_x, sys_y;       // �V�X�e�����b�Z�[�W�̕\���ʒu

  bool isContinue = true;   // �����ăJ�[�h���߂���邩
  bool isCheck = false;     // �ΐ푊���AI�p�F�����̊m�F
  bool isRandom = false;    // �ΐ푊���AI�p�F�����Ō�����Ȃ��Ƃ��A�����_���őI��
  bool game_turn = true;    // �N�̃^�[�����Ftrue = �v���C���[�Afalse = �ΐ푊��
  bool game_set = true;     // �Q�[���I������

  // ���̃Q�[������p�̕ϐ��F�}�E�X����p
  Vec2f mouse_pos;   // �}�E�X�J�[�\���̍��W
  bool on_mouse;     // �}�E�X�J�[�\���̔���

  // ��ɔz�u�����J�[�h�̏��
  struct Card {
    short number;   // �J�[�h�̐���
    short mark;     // �J�[�h�̋L��
    bool isOpen;    // �\�����ɂȂ��Ă��邩
  };

  // �J�[�h���̐���
  Card field[JOKER_NG] = {};        // ��̃J�[�h���
  short open_hist[MAX_HIST] = {};   // �\�ɂ����J�[�h�̗���
  short prev_1, prev_2;             // �\�ɂ����J�[�h�̔�r�p
  short game_score[] = {0, 0};      // �v���C���[�Ƒΐ푊��̓_��

  // �Q�[���̏�����
  {
    // �����̏�����
    for (i = 0; i < MAX_HIST; i++) { open_hist[i] = -1; }

    // �J�[�h���̏�����
    for (i = 0; i < JOKER_NG; i++) { field[i] = {i % CARD_NUM, i / CARD_NUM, false}; }

    // �J�[�h�̃V���b�t��
    for (i = 0; i < JOKER_NG; i++) { std::swap(field[i], field[random.fromZeroToLast(JOKER_NG)]); }

    intro.play();   // �C���g���Đ�
  }


  /* �{�� */
  ////////////////////////////////////////////////////////////////////////////////
  while (1) {
    if (!app_env.isOpen()) { return; }

    // BGM�Đ��F�C���g�����I������烋�[�v�����̍Đ�
    if (!intro.isPlaying()) { if (!thema.isPlaying()) { thema.play(); } }

    // �J�[�h�̎c�薇�����m�F�A�Q�������Ȃ狭���I�ɏI������
    if (game_set) {
      open_count = 0;    // �߂������񐔂����Z�b�g
      field_count = 0;   // �J�[�h�̎c�薇�������Z�b�g

      // �c�薇�����J�E���g
      for (i = 0; i < JOKER_NG; i++) { if (!field[i].isOpen) { field_count++; } }

      // �Q�������Ȃ狭���I��
      if (field_count < OPEN_ACT) { thema.stop(); break; }
      game_set = false;
    }

    app_env.setupDraw();

    // �V�X�e�����b�Z�[�W�̕\��
    {
      // ���b�Z�[�W�S��
      NV_disp_message(system, wframe, isContinue, game_set, open_chain, game_score);

      // �X�R�A�̕\��
      {
        sys_x = (-Window::WIDTH / 2) + Card_W + Icon_W * 2;
        sys_y = (-Window::HEIGHT / 2) + (Telop_H * 4) - 2;

        CO_disp_number(system, game_score[DEF_PLAYER], sys_x, sys_y);                  // �v���C���[�̓_��
        CO_disp_number(system, game_score[DEF_RIVAL], sys_x, sys_y - (Telop_H + 2));   // �ΐ푊��̓_��
      }
    }

    switch (game_turn) {
        // �v���C���[�̃^�[��
      case true:
        // �}�E�X���W�̎擾
        mouse_pos = app_env.mousePosition();

        // �J�[�h���Ƃ̏���
        for (i = 0; i < JOKER_NG; i++) {
          // �J�[�h�̕\���ʒu�ݒ�
          card_x = (Card_W + 8) * ((i % CARD_NUM) - 6.5);
          card_y = (Card_H + 8) * ((i / CARD_NUM) - 1);

          // �J�[�h�̏�Ƀ}�E�X�J�[�\�������邩����
          on_mouse = CO_on_mouse(mouse_pos, card_x, card_y, Card_W, Card_H);
          if (on_mouse && open_count < OPEN_ACT) {
            // �܂��\�����ɂȂ��ĂȂ��J�[�h�������Ƃ�
            if (!field[i].isOpen) {
              // ���N���b�N�����Ƃ��̏���
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                if (open_count == 0) { click.play(); }   // �P���ڂ��߂������Ƃ����� SE�Đ�

                field[i].isOpen = true;      // �I�񂾃J�[�h��\�ɂ���
                open_count++;                // �߂������񐔂��J�E���g
                open_hist[hist_count] = i;   // �߂������J�[�h�̏ꏊ�𗚗��ɕۑ�
                hist_count++;                // ������ۑ������񐔂��J�E���g
              }
            }
          }
        }

        // �J�[�h���Q���߂������Ƃ�
        if (open_count == OPEN_ACT) {
          // �J�[�h�𑱂��đI���ł���Ƃ�
          if (isContinue) {
            // �\�ɂ����J�[�h�̏����擾
            prev_2 = open_hist[hist_count - OPEN_ACT];       // �ŏ��ɂ߂������J�[�h
            prev_1 = open_hist[hist_count - OPEN_ACT + 1];   // ���ɂ߂������J�[�h

            // �\�ɂ����J�[�h�̐����������������Ƃ�
            if (field[prev_2].number == field[prev_1].number) {
              success.play();   // SE�Đ�

              game_score[DEF_PLAYER]++;   // �_�������Z
              open_chain++;               // �y�A���������񐔂��J�E���g
              hist_count -= OPEN_ACT;     // �����ɕۑ������񐔂��߂������񐔂����߂�
              game_set = true;            // �c�薇�����ω�����̂ŁA������x�m�F����
            }

            // �Ⴄ�����������Ƃ��A�J�[�h��I���ł��Ȃ��悤�ɂ���
            else {
              failure.play();   // SE�Đ�
              isContinue = false;

              // ���������܂�����J�E���^�����Z�b�g
              if (hist_count >= MAX_HIST) { hist_count = 0; }
            }
          }

          // �J�[�h���I���ł��Ȃ��Ȃ����Ƃ�
          else {
            // ���N���b�N�ő���̃^�[���ɂ���
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click.play();
              game_turn = false;   // �ΐ푊��̃^�[���ɐݒ�

              // �������Ⴄ�̂ŁA�\�ɂ����J�[�h�����ɖ߂�
              field[prev_2].isOpen = false;
              field[prev_1].isOpen = false;

              // �e��J�E���^�̃��Z�b�g
              open_chain = 0;
              isContinue = true;
              game_set = true;
            }
          }
        }

        break;   // �v���C���[�̃^�[�� �I��

        // �ΐ푊��̃^�[��
      case false:
        // �J�[�h��I���ł��Ȃ��Ȃ�܂ŏ��������s
        if (isContinue) {
          switch (open_count) {
              // �P���ځF�������̃J�[�h�������_���ɑI��
            case 0:
              prev_2 = random.fromZeroToLast(JOKER_NG);

              // �����_���ɑI�����ꂽ�J�[�h���������Ȃ猈��
              if (!field[prev_2].isOpen) {
                open_count++;   // �Q���ڂ̑I�����J�n

                // �t���O�����Z�b�g
                isCheck = false;
                isRandom = true;
              }
              break;

              // �Q���ځF�����������A����������������Ȃ���΃����_���ɑI��
            case 1:
              // �P�񂾂���������������
              if (!isCheck) {
                for (i = 0; i < MAX_HIST; i++) {
                  // �����ɂP���ڂƓ�������������΁u�Ƃ肠�����v����A�����_���I�������
                  if (field[open_hist[i]].number == field[prev_2].number) {
                    prev_1 = open_hist[i];
                    isRandom = false;
                    break;
                  }
                }

                isCheck = true;   // �������m�F�����̂ŁA���̎����̃^�[���܂Ŋm�F���Ȃ�
              }

              // ���������Ɏ��s���Ă�����A�����_���ɃJ�[�h��I��
              if (isRandom) { prev_1 = random.fromZeroToLast(JOKER_NG); }

              // �I�����ꂽ�J�[�h���������A���P���ڂƈႤ�J�[�h�Ȃ�m��A�����łȂ���΃����_���I���J�n
              if (!field[prev_1].isOpen && prev_1 != prev_2) { open_count++; }
              else { isRandom = true; }
              break;

              // �J�[�h���Q���I�������Ƃ�
            case 2:
              // �I�����ꂽ�J�[�h��\�����ɂ���
              field[prev_2].isOpen = true;
              field[prev_1].isOpen = true;

              // �I�����ꂽ�Q��������������������_�������Z���āA������x�I������
              if (field[prev_1].number == field[prev_2].number) {
                game_score[DEF_RIVAL]++;   // �_�������Z
                game_set = true;           // �c�薇�����ω�����̂ŁA������x�m�F����
              }

              // ���������ł͂Ȃ������Ƃ��A�J�[�h��I���ł��Ȃ��悤�ɐݒ�
              else { isContinue = false; }
              break;
          }
        }

        // �J�[�h��I���ł��Ȃ��Ȃ����Ƃ�
        else {
          // ���N���b�N�Ńv���C���[�̃^�[���ɂ���
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            click.play();

            // �I�����ꂽ�J�[�h�����ɖ߂�
            field[prev_2].isOpen = false;
            field[prev_1].isOpen = false;

            // �e��t���O�̃��Z�b�g
            game_turn = true;
            game_set = true;
            isContinue = true;
            isRandom = false;
          }
        }

        break;   // �ΐ푊��̃^�[�� �I��
    }

    // �J�[�h�摜�̕\��
    for (i = 0; i < JOKER_NG; i++) {
      // �J�[�h�̕\���ʒu�ݒ�
      card_x = (Card_W + 8) * ((i % CARD_NUM) - 6.5);
      card_y = (Card_H + 8) * ((i / CARD_NUM) - 1);

      // �J�[�h�̏�Ƀ}�E�X�J�[�\�������邩����
      on_mouse = CO_on_mouse(mouse_pos, card_x, card_y, Card_W, Card_H);
      if (field[i].isOpen || (app_env.isPressKey('O') && app_env.isPressKey('P'))) {
        // �J�[�h���\���̂Ƃ�
        drawTextureBox(card_x, card_y, Card_W, Card_H,
          Card_W * field[i].number, Card_H * field[i].mark, Card_W, Card_H,
          t_card, Color(1, 1, 1));
      }
      else {
        // �J�[�h�������̂Ƃ�
        drawTextureBox(card_x, card_y, Card_W, Card_H,
          Card_W * 1, Card_H * 4, Card_W, Card_H,
          t_card, NV_set_cardcolor(on_mouse));
      }
    }

    app_env.update();
  } // �{�� �I��

  // ���U���g���
  finish.play();
  result_nervous(app_env, system, wframe,
    isContinue, game_set, open_chain,
    game_score[DEF_PLAYER], game_score[DEF_RIVAL]);
}
