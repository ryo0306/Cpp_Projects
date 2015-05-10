//
// Revival of Kingdom
//

#include "appEnv.hpp"
#include "_MyVar_list.hpp"    // �ϐ����X�g
#include "_MyFunc_list.hpp"   // �O�������֐�
#include <time.h>


// �֐��i�Q�[���{�җp�j
////////////////////////////////////////////////////////////////////////////////////////////////////

// �͂��E������
//  0�F�^�C�g���F�R���e�B�j���[
//  1�F�Z�[�u
//  2�F�N�U
//  3�F���p
//  4�F�X�p�C
//  5�F�W�Q�H��
//  6�F�����̗��z
//  7�F�ł𐷂�
//  8�F����
//  9�F�S�V�b�v
// 10�F����
// 11�F����
int yes_or_no(AppEnv& app_env, short flag) {
  // �w�i�A�{�^���A�e�L�X�g�摜�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture blackBG("res/png/BG5_Black.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");

  // �{�^���摜�\���ʒu�̏��
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float y0 = (Window::HEIGHT / 2) - 360;

  // �{�^���̃J�[�\������A�F
  bool on_mouse;
  Color angle;

  while (1) {
    if (!app_env.isOpen()) return 0;
    app_env.setupDraw();

    // ����̍s���J�n�܂ł̃J�E���g
    if (E_time < 7200) {
      E_time += 1;
    }

    // �J�[�\�����W�̎擾
    mouse_pos = app_env.mousePosition();

    // �w�i�\��
    if (flag == 0) {
      // �^�C�g���F�R���e�B�j���[�I��
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        blackBG, Color(1, 1, 1));
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
        0, 365, Window::WIDTH, Window::HEIGHT - 365,
        mainBG, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
    }

    // �e���b�v�\��
    switch (flag) {

      // �^�C�g���F�R���e�B�j���[�I��
      case 0:
        y = y0 + (menu_H + 2) * 3;
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 0, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�Z�[�u�v�I��
      case 1:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 5, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 5, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�N�U�v�I��
      case 2:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 4, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 85, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u���p�v�I��
      case 3:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 8, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 20, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�X�p�C�v�I��
      case 4:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 12, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 57, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�W�Q�H��v�I��
      case 5:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 13, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 59, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�����̗��z�v�I��
      case 6:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 14, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 61, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�ł𐷂�v�I��
      case 7:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 15, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 63, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�����v�I��
      case 8:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 16, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 66, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�S�V�b�v�v�I��
      case 9:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 17, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 67, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�����v�I��
      case 10:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 18, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 68, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // �u�����v�I��
      case 11:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 3, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 81, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;
    }

    // �{�^���̕\��
    {
      // �͂�
      {
        // �}�E�X����ɂ��摜�J���[�̐ݒ�
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);

        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 19, menu_W, menu_H,
          button, angle);

        // �}�E�X�N���b�N�̉ۂƃN���b�N�����Ƃ��̏���
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            return 1;
          }
        }
      }

      // ������
      {
        // �}�E�X����ɂ��摜�J���[�̐ݒ�
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 3, menu_W, menu_H);
        angle = setButtonColor(on_mouse);

        drawTextureBox(x, y - (menu_H + 2) * 3, menu_W, menu_H,
          0, menu_H * 20, menu_W, menu_H,
          button, angle);

        // �}�E�X�N���b�N�̉ۂƃN���b�N�����Ƃ��̏���
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            return 2;
          }
        }
      }
    }

    app_env.update();
  }

  return 0;
}

// ���p
void item_sale(AppEnv& app_env) {
  // �w�i�A�e�L�X�g�摜�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture f_list("res/png/Me3_FundList.png");
  Texture system("res/png/Me4_System.png");
  Texture buytxt("res/png/Me6_BuyTxt.png");

  // �{�^���摜�\���ʒu�̏��
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // �{�^���̃J�[�\������A�F
  bool on_mouse;
  Color angle;

  // �t���O���Z�b�g
  button_flag = -1;

  // �{�^���\���ʒu�w��
  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // ����̍s���J�n�܂ł̃J�E���g
    if (E_time < 7200) {
      E_time += 1;
    }

    // �J�[�\�����W�̎擾
    mouse_pos = app_env.mousePosition();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �{�^���\��
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 8, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // ���p���X�g�̕\��
    if (button_flag == -1) {
      // �ǂ�𔄋p���܂����H
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
        0, telop_H * 18, telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // ��߂�
      {
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 21, menu_W, menu_H,
          button, angle);
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            break;
          }
        }
      }

      // �{�^���̕\��
      for (i = 0; i < 7; i += 1) {
        if (P_item[i] != 0) {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * (i + 1), fund_W, menu_H);
          if (P_item[i] == 2) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * (i + 1), fund_W, menu_H,
            0, menu_H * i, fund_W, menu_H,
            f_list, angle);

          if (on_mouse) {
            if (P_item[i] == 2) {
              // ���p�ς�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 19, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �A�C�e���̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * (27 + i * 2), telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();

                // �͂���I�Ԃƃ��[�v�I��
                if (yes_or_no(app_env, 3) == 1) {
                  button_flag = i;
                  break;
                }
              }
            }
          }
        }
      }
    }

    // �A�C�e����I�������Ƃ�
    else {
      // ���p���܂���
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
        0, telop_H * 21, telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // �}�E�X�N���b�N�Ń��[�v�I��
      if (app_env.isPushButton(Mouse::LEFT)) {
        P_item[button_flag] = 2;
        P.money += iteminfo[button_flag];
        app_env.flushInput();
        break;
      }
    }

    app_env.update();
  }
}

// �����J��
void add_money(AppEnv& app_env) {
  // �w�i�A�e�L�X�g�摜�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");

  // �{�^���摜�\���ʒu�̏��
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // �{�^���̃J�[�\������A�F
  bool on_mouse;
  Color angle;

  // ����
  int income;

  // �t���O���Z�b�g
  button_flag = -1;

  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // ����̍s���J�n�܂ł̃J�E���g
    if (E_time < 7200) {
      E_time += 1;
    }

    // �J�[�\�����W�̎擾
    mouse_pos = app_env.mousePosition();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �{�^���\��
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 0, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // �R�}���h���X�g�̕\��
    if (button_flag == -1) {

      // ��߂�
      {
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 21, menu_W, menu_H,
          button, angle);
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            break;
          }
        }
      }

      // �A���o�C�g
      {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 1, menu_W, menu_H);
        if (P.soldier == 0) {
          angle = Color(0.5, 0.5, 0.5);
        }
        else {
          angle = setButtonColor(on_mouse);
        }
        drawTextureBox(x0, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 6, menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          if (P.soldier == 0) {
            // ���m�����Ȃ��̂Ŏ��s�ł��܂���
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W / 2, telop_H * 1,
              status_W * 2, telop_H * 86, telop_W / 2, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else {
            // �R�}���h�̉��
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
              0, telop_H * 10, telop_W, telop_H * 2,
              system, Color(0, 0, 0));

            // �N���b�N�Ŋm�F��ʕ\��
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();

              // �����z�̌v�Z
              income = P.soldier * 2;

              // �����𓾂�ƁA�����̏���𒴂���ꍇ
              if (P.money + income > 999999) {
                income = 999999 - P.money;
              }

              // �������擾
              numberGet(income);

              P.money += income;
              P.fame -= 1;
              button_flag = 0;
            }
          }
        }
      } // command 1 end

      // ����
      {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x0, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 7, menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // �R�}���h�̉��
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
            0, telop_H * 13, telop_W, telop_H * 2,
            system, Color(0, 0, 0));

          // �N���b�N�Ŋm�F��ʕ\��
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();

            // �����z�̌v�Z
            income = P.fund / 10 + P.territory * 100 + (P.fame - P.infamy) * 10;

            // �����𓾂�ƁA�����̏���𒴂���ꍇ
            if (P.money + income > 999999) {
              income = 999999 - P.money;
            }

            // �������擾
            numberGet(income);

            P.money += income;
            button_flag = 1;
          }
        }
      } // command 2 end

      // ���p
      {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 3, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x0, y - (menu_H + 2) * 3, menu_W, menu_H,
          0, menu_H * 8, menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // �R�}���h�̉��
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
            0, telop_H * 16, telop_W, telop_H * 2,
            system, Color(0, 0, 0));

          // �N���b�N�Ŋm�F��ʕ\��
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            item_sale(app_env);
            ex_flag += 1;
            break;
          }
        }
      } // command 3 end
    }

    // �R�}���h�I�������Ƃ��̏���
    else {
      // ���s���e��\��
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
        0, telop_H * (12 + button_flag * 3), telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // ����
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
        0, telop_H * 9, telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // �����z�̕\��
      for (i = 0; i < 6; i += 1) {
        if (i < skip) {
          drawTextureBox((-telop_W / 2) + status_W + number_W * (i + 1), (-Window::HEIGHT / 2) + telop_H * 4, number_W, telop_H,
            number_W * val[i], telop_H * 98, number_W, telop_H,
            system, Color(0, 0, 0));
        }
        else {
          drawTextureBox((-telop_W / 2) + status_W + number_W * (i + 1), (-Window::HEIGHT / 2) + telop_H * 4, number_W, telop_H,
            number_W * val[i], telop_H * 97, number_W, telop_H,
            system, Color(0, 0, 0));
        }
      }

      // �}�E�X�N���b�N�Ń��[�v�I��
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        enemy_reaction();
        if (spy > 0) spy -= 1;
        ex_flag += 1;
        break;
      }
    }

    app_env.update();
  }
}

// ���͑���
void add_soldier(AppEnv& app_env) {
  // �w�i�A�e�L�X�g�摜�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");

  // �{�^���摜�\���ʒu�̏��
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // �{�^���̃J�[�\������A�F
  bool on_mouse;
  Color angle;

  // �t���O
  short button_flag0 = -1;
  short guara;

  // �t���O���Z�b�g
  button_flag = -1;

  // �{�^���̕\���ʒu�w��
  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // ����̍s���J�n�܂ł̃J�E���g
    if (E_time < 7200) {
      E_time += 1;
    }

    // �J�[�\�����W�̎擾
    mouse_pos = app_env.mousePosition();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �{�^���\��
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 1, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // �R�}���h���X�g�̕\��
    if (button_flag == -1) {

      // ��߂�
      {
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 21, menu_W, menu_H,
          button, angle);
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            break;
          }
        }
      }

      // �{�^���̕\��
      for (i = 0; i < 3; i += 1) {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H,
          0, menu_H * (9 + i), menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // �R�}���h�̉��
          if (i == 0) {
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
              0, telop_H * 24, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else {
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
              0, telop_H * (23 + i * 2), telop_W, telop_H * 2,
              system, Color(0, 0, 0));
          }

          // �N���b�N�Ŋm�F��ʕ\��
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            button_flag = i;
          }
        }
      }
    }

    // �{�^�����I�����ꂽ�Ƃ�
    else {

      // �l���I���{�^���̕\��
      if (button_flag0 == -1) {

        // �R�}���h�̉��
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 29, telop_W, telop_H * 1,
          system, Color(0, 0, 0));

        // �I�����ꂽ�{�^���Ɖ���̕\��
        switch (button_flag) {
          case 0:
            // ���m
            drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
              0, menu_H * 9, menu_W, menu_H,
              button, Color(1, 0.8, 0.4));
            // �R�}���h�̉��
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
              0, telop_H * 30, telop_W, telop_H * 1,
              system, Color(0, 0, 0));

            // �����̐ݒ�
            guara = 10;
            break;

          case 1:
            // �`�E��
            drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
              0, menu_H * 10, menu_W, menu_H,
              button, Color(1, 0.8, 0.4));
            // �R�}���h�̉��
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
              0, telop_H * 31, telop_W, telop_H * 1,
              system, Color(0, 0, 0));

            // �����̐ݒ�
            guara = 5;
            break;

          case 2:
            // ��������
            drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
              0, menu_H * 11, menu_W, menu_H,
              button, Color(1, 0.8, 0.4));

            // �����̐ݒ�
            guara = 0;
            break;
        }

        // ��߂�
        {
          on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 3, menu_W, menu_H);
          angle = setButtonColor(on_mouse);
          drawTextureBox(x, y - (menu_H + 2) * 3, menu_W, menu_H,
            0, menu_H * 21, menu_W, menu_H,
            button, angle);
          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              break;
            }
          }
        }

        // �l���{�^���̕\��
        for (i = 0; i < 4; i += 1) {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H);
          if (button_flag == 0 && P.soldier + std::pow(10, (i + 1)) > 65535) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else if (button_flag == 1 && P.mercenary + std::pow(10, (i + 1)) > 65535) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else if (P.money < guara * std::pow(10, (i + 1))) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H,
            0, menu_H * (27 + i), menu_W, menu_H,
            button, angle);

          if (on_mouse) {
            if (button_flag == 0 && P.soldier + std::pow(10, (i + 1)) > 65535) {
              // ����ȏ��W�ł��܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                0, telop_H * 33, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (button_flag == 1 && P.mercenary + std::pow(10, (i + 1)) > 65535) {
              // ����ȏ��W�ł��܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                0, telop_H * 33, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < guara * std::pow(10, (i + 1))) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag0 = i;
              }
            }
          }
        }
      }

      // �l�����m�肵���Ƃ�
      else {

        // ��W���܂����B
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 32, telop_W, telop_H * 1,
          system, Color(0, 0, 0));

        // �}�E�X�N���b�N�Ń��[�v�I��
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();

          // ���ꂼ��̑I����e���p�����[�^�ɔ��f
          switch (button_flag) {

              // ���m
            case 0:
              P.money -= guara * std::pow(10, (button_flag0 + 1));
              P.soldier += std::pow(10, (button_flag0 + 1));
              break;

              // �`�E��
            case 1:
              P.money -= guara * std::pow(10, (button_flag0 + 1));
              P.mercenary += std::pow(10, (button_flag0 + 1));
              break;

              // ��������
            case 2:
              P.soldier += std::pow(10, (button_flag0 + 1));
              if (button_flag0 > 0) {
                P.infamy += std::pow(10, (button_flag0 - 1));
              }
              break;
          }

          enemy_reaction();
          if (spy > 0) spy -= 1;
          ex_flag += 1;
          break;
        }
      } // 2nd loop end
    } // 1st loop end

    app_env.update();
  }
}

// �v��
void trick(AppEnv& app_env) {
  // �w�i�A�e�L�X�g�摜�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");
  Texture telop("res/png/T05_3rdStage.png");

  // �{�^���摜�\���ʒu�̏��
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // �{�^���̃J�[�\������A�F
  bool on_mouse;
  Color angle;

  // ���s�ۂ̃t���O
  bool cmd = true;

  // ���۔���̃t���O
  bool cmd_ok = false;

  // ����
  int r;

  // �t���O���Z�b�g
  button_flag = -1;

  // �{�^���̕\���ʒu�w��
  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // ����̍s���J�n�܂ł̃J�E���g
    if (E_time < 7200) {
      E_time += 1;
    }

    // ��������
    srand((unsigned)time(NULL));

    // �J�[�\�����W�̎擾
    mouse_pos = app_env.mousePosition();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �{�^���\��
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 2, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // �R�}���h���X�g�̕\��
    if (button_flag == -1) {

      // ��߂�
      {
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 21, menu_W, menu_H,
          button, angle);
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            break;
          }
        }
      }

      // �{�^���̕\��
      for (i = 0; i < 7; i += 1) {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H);
        cmd = true;

        // �{�^���̐F�ύX
        angle = setButtonColor(on_mouse);
        switch (i) {

            // �X�p�C
          case 0:
            if (spy > 0 || P.money < 100) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // �W�Q�H��
          case 1:
            if (P.money < 150) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // �����̗��z
          case 2:
            if (rumor || P.money < 100) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // �ł𐷂�
          case 3:
            if (P.money < 200) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // ����
          case 4:
            if (P.money < 50) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // �S�V�b�v
          case 5:
            if (P.money < 150) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // ����
          case 6:
            if (P.money < 200) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;
        }

        // �{�^���̕\��
        drawTextureBox(x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H,
          0, menu_H * (12 + i), menu_W, menu_H,
          button, angle);

        if (on_mouse) {

          // ���s�s�������Ƃ�
          if (!cmd) {

            // �X�p�C�F���s�ς݂�
            if (i == 0 && spy > 0) {
              // ���łɎ��s���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 56, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }

            // �����̗��z�F���s�ς݂�
            else if (i == 2 && rumor) {
              // ���łɎ��s���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 56, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }

            // ����ȊO
            else {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
          }

          // ���s�\�������Ƃ�
          else {
            // �R�}���h�̉��
            switch (i) {

              // �X�p�C
              case 0:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 40, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // ���������F100 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 37, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // �W�Q�H��
              case 1:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 42, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // ���������F150 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 38, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // �����̗��z
              case 2:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                  0, telop_H * 44, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));

                // ���������F100 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 37, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // �ł𐷂�
              case 3:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 45, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // ���������F200 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 39, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // ����
              case 4:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 47, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // ���������F50 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 36, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // �S�V�b�v
              case 5:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 49, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // ���������F150 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 38, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // ����
              case 6:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 51, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // ���������F200 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 39, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;
            }

            // �N���b�N�Ŋm�F��ʕ\��
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              button_flag = i;
            }
          }
        } // on_mouse end
      }
    }

    // �R�}���h�I�������Ƃ��̏���
    else {

      // �͂��ȊO��I���ŃR�}���h�I��
      if (yes_or_no(app_env, (4 + button_flag)) != 1) {
        return;
      }

      // �R�}���h���s
      switch (button_flag) {

          // �X�p�C
        case 0:
          P.money -= 100;
          spy = 10;

          // �R�͂������Ƃ�
          if (stage_flag == 3)
            _3rdstage_spy_act(app_env);

          break;

          // �W�Q�H��
        case 1:
          P.money -= 150;
          sabo += 3;

          // �X�p�C���s���Ȃ���ʃA�b�v
          if (spy > 0) {
            sabo += 2;
            spy -= 1;
          }
          break;

          // �����̗��z
        case 2:
          P.money -= 100;

          // �����𔭐�
          r = rand() % 100;

          // �X�p�C���s���Ȃ琬�����A�b�v�i+20%�j
          if (spy > 0) {
            r -= 20;
            spy -= 1;
          }

          // ���۔���i75% �̊m���Ő����j
          if (r < 75) {
            cmd_ok = true;
            rumor = true;
            E.fund -= E.fund / 10;
          }
          else {
            cmd_ok = false;
          }
          break;

          // �ł𐷂�
        case 3:
          P.money -= 200;

          // �����𔭐�
          r = rand() % 100;

          // �X�p�C���s���Ȃ琬�����A�b�v�i+20%�j
          if (spy > 0) {
            r -= 20;
            spy -= 1;
          }

          // ���۔���i75% �̊m���Ő����j
          if (r < 75) {
            cmd_ok = true;
            E.mercenary += (E.soldier / 10) / 10;
            E.soldier -= E.soldier / 10;
          }
          else {
            cmd_ok = false;
          }

          // �X�p�C���s���ł͂Ȃ��Ƃ�
          if (spy <= 0) {
            // �����𔭐�
            r = rand() % 100;

            // ���۔���i40% �̊m���ň��������o�j
            if (r < 40) {
              disclos = true;
              P.infamy += 10;
            }
          }
          break;

          // ����
        case 4:
          P.money -= 50;

          // �����𔭐�
          r = rand() % 100;

          // ���۔���i85% �̊m���Ő����j
          if (r < 85) {
            cmd_ok = true;

            // �������O
            if (P.infamy == 0) {
              P.fame += 5;
            }

            // �O�ł͂Ȃ��Ƃ�
            else {
              P.fame += 2;
              P.infamy -= 2;
            }
          }
          else {
            cmd_ok = false;
          }
          break;

          // �S�V�b�v
        case 5:
          P.money -= 150;

          // �����𔭐�
          r = rand() % 100;

          // �X�p�C���s���Ȃ琬�����A�b�v�i+20%�j
          if (spy > 0) {
            r -= 20;
            spy -= 1;
          }

          // ���۔���i75% �̊m���Ő����j
          if (r < 75) {
            cmd_ok = true;
            E.infamy += 10;
          }
          else {
            cmd_ok = false;
            P.infamy += 5;
          }
          break;

          // ����
        case 6:
          P.money -= 200;

          // �R�͂������Ƃ�
          if (stage_flag == 3) {

            // �u���h�Ȃ���ނ�v���������Ă��邩
            if (P_item[3] != 0) {
              cmd_ok = true;
              rebel = true;
            }
            else {
              cmd_ok = false;
            }
          }

          // �R�͈ȊO
          else {

            // �����𔭐�
            r = rand() % 100;

            // �X�p�C���s���Ȃ琬�����A�b�v�i+20%�j
            if (spy > 0) {
              r -= 20;
              spy -= 1;
            }

            // ���۔���i����̐l�]���Ⴂ�قǐ������A�b�v�j
            if (r < (E.fame - E.infamy)) {
              cmd_ok = true;
            }
            else {
              cmd_ok = false;
            }

            // �����������m�肵���Ƃ�
            if (cmd_ok) {
              // �����𔭐�
              r = rand() % 100;

              // ���۔���i����̐l�]���Ⴍ�A�v���C���[�̐l�]�������قǐ������A�b�v�j
              if (r < (E.fame - E.infamy) + (P.fame - P.infamy) / 2) {
                rebel = true;
              }

              // �������������Ȃ������Ƃ�
              else {
                E.money /= 2;
                E.soldier /= 2;
                E.mercenary = 0;
              }
            }
          }
          break;
      }

      while (1) {
        if (!app_env.isOpen()) return;

        // �}�E�X�N���b�N�Ń��[�v�I��
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          disclos = false;
          ex_flag += 1;
          return;
        }

        app_env.setupDraw();

        // �w�i�\��
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));

        // ���s���ʂ̕\��
        switch (button_flag) {

            // �X�p�C
          case 0:
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
              0, telop_H * 58, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
            break;

            // �W�Q�H��
          case 1:
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
              0, telop_H * 60, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
            break;

            // �����̗��z
          case 2:
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
              0, telop_H * 62, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
            break;

            // �ł𐷂�
          case 3:
            // ����
            if (cmd_ok) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 64, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            // ���s
            else {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 55, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }

            // ���������o
            if (disclos) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
                0, telop_H * 65, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            break;

            // ����
          case 4:
            // ����
            if (cmd_ok) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 54, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            // ���s
            else {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 55, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            break;

            // �S�V�b�v
          case 5:
            // ����
            if (cmd_ok) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 54, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            // ���s
            else {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 55, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            break;

            // ����
          case 6:
            // ����
            if (cmd_ok) {
              // ��������
              if (rebel) {
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                  0, telop_H * 69, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              // �����s����
              else {
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 70, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));
              }
            }
            // ���s
            else {
              // �R��
              if (stage_flag == 3) {
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 4,
                  0, telop_H * 38, telop_W, telop_H * 4,
                  telop, Color(0, 0, 0));
              }
              // �R�͈ȊO
              else {
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                  0, telop_H * 72, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
            }
            break;
        }

        app_env.update();
      }
    }

    app_env.update();
  }
}

// ����
void buy_fund(AppEnv& app_env) {
  // �w�i�A�e�L�X�g�摜�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture b_list("res/png/Me2_BuyList.png");
  Texture system("res/png/Me4_System.png");
  Texture buytxt("res/png/Me6_BuyTxt.png");

  // �{�^���摜�\���ʒu�̏��
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // �{�^���̃J�[�\������A�F
  bool on_mouse;
  Color angle;

  // ����̐��۔���
  bool buy_flag = true;

  // �t���O���Z�b�g
  button_flag = -1;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // ����̍s���J�n�܂ł̃J�E���g
    if (E_time < 7200) {
      E_time += 1;
    }

    // �J�[�\�����W�̎擾
    mouse_pos = app_env.mousePosition();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �{�^���\��
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 3, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // �ǂ�𔃎��܂����H
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
      0, telop_H * 75, telop_W, telop_H * 1,
      system, Color(0, 0, 0));

    // �R�}���h���X�g�̕\��
    if (button_flag == -1) {

      // ��߂�
      {
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 21, menu_W, menu_H,
          button, angle);
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            break;
          }
        }
      }

      // �̒n
      if (stage_flag < 5) {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 1, fund_W, menu_H);
        angle = setButtonColor(on_mouse);
        buy_flag = true;

        // �X�e�[�W���Ƃ̔��承�i
        switch (stage_flag) {
          case 1:
            if (P.money < (6 - E.territory) * 1000) {
              angle = Color(0.5, 0.5, 0.5);
              buy_flag = false;
            }
            break;

          case 2:
            if (P.money < (6 - E.territory) * 2000) {
              angle = Color(0.5, 0.5, 0.5);
              buy_flag = false;
            }
            break;

          case 3:
            if (P.money < (11 - E.territory) * 2500 || E.territory < 4) {
              angle = Color(0.5, 0.5, 0.5);
              buy_flag = false;
            }
            break;

          case 4:
            if (P.money < (11 - E.territory) * 5000 || E.territory < 4) {
              angle = Color(0.5, 0.5, 0.5);
              buy_flag = false;
            }
            break;
        }
        drawTextureBox(x0, y - (menu_H + 2) * 1, fund_W, menu_H,
          0, menu_H * 14, fund_W, menu_H,
          b_list, angle);

        if (on_mouse) {
          if (E.territory < 4 && stage_flag > 2) {
            // �������܂���
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 80, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else if (!buy_flag) {
            // ����������܂���
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 77, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else {
            // �R�}���h�̉��
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 79, telop_W, telop_H * 1,
              system, Color(0, 0, 0));

            // �N���b�N�Ŋm�F��ʕ\��
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              button_flag = 14;
            }
          }
        }
      } // button 14 end

      // �P�͂���
      if (stage_flag == 1) {
        // ���q�̏�
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 2, fund_W, menu_H);
        if (P.money < fundinfo[0] || P_fund[0] != 0) {
          angle = Color(0.5, 0.5, 0.5);
        }
        else {
          angle = setButtonColor(on_mouse);
        }
        drawTextureBox(x0, y - (menu_H + 2) * 2, fund_W, menu_H,
          0, menu_H * 0, fund_W, menu_H,
          b_list, angle);

        if (on_mouse) {
          if (P_fund[0] != 0) {
            // ���łɏ��L���Ă��܂�
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 76, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else if (P.money < fundinfo[0]) {
            // ����������܂���
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 77, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else {
            // �R�}���h�̉��
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
              0, telop_H * 0, telop_W, telop_H * 2,
              buytxt, Color(0, 0, 0));

            // �N���b�N�Ŋm�F��ʕ\��
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              button_flag = 0;
            }
          }
        }
      } // button 1 end

      // �P�͂���R�͂̊�
      if (stage_flag > 0 && stage_flag < 4) {
        // �_�n
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 3, fund_W, menu_H);
          if (P.money < fundinfo[1] || P_fund[1] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 3, fund_W, menu_H,
            0, menu_H * 1, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[1] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[1]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 2, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 1;
              }
            }
          }
        } // button 1 end

        // �q��
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 4, fund_W, menu_H);
          if (P.money < fundinfo[2] || P_fund[2] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 4, fund_W, menu_H,
            0, menu_H * 2, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[2] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[2]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 4, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 2;
              }
            }
          }
        } // button 2 end
      }

      // �Q�͂���
      if (stage_flag > 1) {
        // �R�͂܂�
        if (stage_flag < 4) {
          // �_��{�P
          {
            on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 5, fund_W, menu_H);
            if (P.money < fundinfo[3] || P_fund[3] != 0) {
              angle = Color(0.5, 0.5, 0.5);
            }
            else {
              angle = setButtonColor(on_mouse);
            }
            drawTextureBox(x0, y - (menu_H + 2) * 5, fund_W, menu_H,
              0, menu_H * 3, fund_W, menu_H,
              b_list, angle);

            if (on_mouse) {
              if (P_fund[3] != 0) {
                // ���łɏ��L���Ă��܂�
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 76, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else if (P.money < fundinfo[3]) {
                // ����������܂���
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 77, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else {
                // �R�}���h�̉��
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                  0, telop_H * 6, telop_W, telop_H * 2,
                  buytxt, Color(0, 0, 0));

                // �N���b�N�Ŋm�F��ʕ\��
                if (app_env.isPushButton(Mouse::LEFT)) {
                  app_env.flushInput();
                  button_flag = 3;
                }
              }
            }
          } // button 3 end

          // �q��{�P
          {
            on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 6, fund_W, menu_H);
            if (P.money < fundinfo[4] || P_fund[4] != 0) {
              angle = Color(0.5, 0.5, 0.5);
            }
            else {
              angle = setButtonColor(on_mouse);
            }
            drawTextureBox(x0, y - (menu_H + 2) * 6, fund_W, menu_H,
              0, menu_H * 4, fund_W, menu_H,
              b_list, angle);

            if (on_mouse) {
              if (P_fund[4] != 0) {
                // ���łɏ��L���Ă��܂�
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 76, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else if (P.money < fundinfo[4]) {
                // ����������܂���
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 77, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else {
                // �R�}���h�̉��
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                  0, telop_H * 8, telop_W, telop_H * 2,
                  buytxt, Color(0, 0, 0));

                // �N���b�N�Ŋm�F��ʕ\��
                if (app_env.isPushButton(Mouse::LEFT)) {
                  app_env.flushInput();
                  button_flag = 4;
                }
              }
            }
          } // button 4 end
        }

        // �H��
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 2, fund_W, menu_H);
          if (P.money < fundinfo[5] || P_fund[5] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 2, fund_W, menu_H,
            0, menu_H * 5, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[5] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[5]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 10, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 5;
              }
            }
          }
        } // button 5 end
      }

      // �R�͂���
      if (stage_flag > 2) {
        // �S�̍z�R
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 7, fund_W, menu_H);
          if (P.money < fundinfo[6] || P_fund[6] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 7, fund_W, menu_H,
            0, menu_H * 6, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[6] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[6]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 12, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 6;
              }
            }
          }
        } // button 6 end
        
        // ��̍z�R
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 8, fund_W, menu_H);
          if (P.money < fundinfo[7] || P_fund[7] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 8, fund_W, menu_H,
            0, menu_H * 7, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[7] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[7]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 14, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 7;
              }
            }
          }
        } // button 7 end

        // ���̍z�R
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 9, fund_W, menu_H);
          if (P.money < fundinfo[8] || P_fund[8] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 9, fund_W, menu_H,
            0, menu_H * 8, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[8] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[8]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 16, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 8;
              }
            }
          }
        } // button 8 end

        // ���p��
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 10, fund_W, menu_H);
          if (P.money < fundinfo[9] || P_fund[9] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 10, fund_W, menu_H,
            0, menu_H * 9, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[9] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[9]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 18, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 9;
              }
            }
          }
        } // button 9 end
      }

      // �S�͂���
      if (stage_flag > 3) {

        // �ǎ��ȓS�z�R
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 3, fund_W, menu_H);
          if (P.money < fundinfo[10] || P_fund[10] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 3, fund_W, menu_H,
            0, menu_H * 10, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[10] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[10]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 20, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 10;
              }
            }
          }
        } // button 10 end

        // ����̒b�艮
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 4, fund_W, menu_H);
          if (P.money < fundinfo[11] || P_fund[11] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 4, fund_W, menu_H,
            0, menu_H * 11, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[11] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[11]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 22, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 11;
              }
            }
          }
        } // button 11 end

        // �h��̒b�艮
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 5, fund_W, menu_H);
          if (P.money < fundinfo[12] || P_fund[12] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 5, fund_W, menu_H,
            0, menu_H * 12, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[12] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[12]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 22, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 12;
              }
            }
          }
        } // button 12 end

        // �X
        {
          on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 6, fund_W, menu_H);
          if (P.money < fundinfo[13] || P_fund[13] != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x0, y - (menu_H + 2) * 6, fund_W, menu_H,
            0, menu_H * 13, fund_W, menu_H,
            b_list, angle);

          if (on_mouse) {
            if (P_fund[13] != 0) {
              // ���łɏ��L���Ă��܂�
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[13]) {
              // ����������܂���
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // �R�}���h�̉��
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 24, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // �N���b�N�Ŋm�F��ʕ\��
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 13;
              }
            }
          }
        } // button 13 end
      }
    }

    // �R�}���h�I�������Ƃ��̏���
    else {
      // �m�F��ʂ�\��
      if (yes_or_no(app_env, 11) != 1) {
        if (!app_env.isOpen()) return;
        return;
      }

      // ���������̏���
      if (button_flag != 14) {
        P_fund[button_flag] = 1;
        P.money -= fundinfo[button_flag];
        E.money += fundinfo[button_flag];
        P.fund += fundinfo[button_flag];
        E.fund -= fundinfo[button_flag];
      }

      // �̒n�̔�������
      else {
        switch (stage_flag) {
          case 1:
            P.money -= (6 - E.territory) * 1000;
            E.money += (6 - E.territory) * 1000;
            P.territory += 1;
            E.territory -= 1;
            break;

          case 2:
            P.money -= (6 - E.territory) * 2000;
            E.money += (6 - E.territory) * 2000;
            P.territory += 1;
            E.territory -= 1;
            break;

          case 3:
            P.money -= (11 - E.territory) * 2500;
            E.money += (11 - E.territory) * 2500;
            P.territory += 1;
            E.territory -= 1;
            break;

          case 4:
            P.money -= (11 - E.territory) * 5000;
            E.money += (11 - E.territory) * 5000;
            P.territory += 1;
            E.territory -= 1;
            break;
        }
      }

      while (1) {
        if (!app_env.isOpen()) return;

        // �}�E�X�N���b�N�Ń��[�v�I��
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          ex_flag += 1;
          return;
        }

        app_env.setupDraw();

        // �w�i�\��
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));

        // �������܂���
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 82, telop_W, telop_H * 1,
          system, Color(0, 0, 0));

        app_env.update();
      }
    }

    app_env.update();
  }
}

// �N�U
void duel(AppEnv& app_env) {
  // �w�i�A�e�L�X�g�A�L�����N�^�[�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");
  Texture battle("res/png/Me5_Battle.png");
  Texture PowerG("res/png/Me9_Gauge.png");
  Texture chara6("res/png/06_Rival_C.png");

  // �a�f�l�A�r�d�̓ǂݍ���
  // �ʏ��
  Media bgm_a = setBGM_a(stage_flag);
  bgm_a.looping(true);
  bgm_a.gain(0.2);

  // �ǌ���
  Media bgm_b = setBGM_b(stage_flag);
  bgm_b.looping(true);
  bgm_b.gain(0.2);

  // �퓬�J�n
  Media se_start("res/wav/se_battle.wav");
  se_start.gain(0.2);

  // �����F�A�C�e���l��
  Media se_get("res/wav/se_itemget.wav");
  se_get.gain(0.2);

  // ����
  Media se_win("res/wav/se_win.wav");
  se_win.gain(0.2);

  // �s�k
  Media se_lose("res/wav/se_lose.wav");
  se_lose.gain(0.2);

  // �P��
  Media se_away("res/wav/se_away.wav");
  se_away.gain(0.2);

  // �{�^������A�F
  bool on_mouse;
  Color angle;

  // �摜�\���p
  short i = 0, j;         // �L�����N�^�[�J�E���^
  float x, y;             // �{�^�����W�p
  float x0, y0;   // �L�����N�^�[�\���p

  // �s���񐔐ݒ�
  srand((unsigned)time(NULL));
  short count = (rand() % 3) + 3;

  // �A�C�e���擾�t���O
  bool item_get = false;

  // �퓬�O�̏���
  {
    while (i < 90) {
      if (!app_env.isOpen()) return;

      // �퓬�J�n�r�d�Đ�
      if (i == 0) se_start.play();

      app_env.setupDraw();
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
      // �퓬�J�n
      if (!E_action) {
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 0, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));
      }
      // ����ɍU�ߍ��܂ꂽ
      else {
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 100, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
      }
      app_env.update();

      i += 1;
    }

    // �{�^���̕\���ʒu�w��
    {
      x = (-Window::WIDTH / 2) + 2;
      y = (Window::HEIGHT / 2) - 358;
    }

    // �U���͊�l�ƕ��͏����l���擾
    duel_standby();
  }

  // �t���O���Z�b�g
  button_flag = -1;

  // �퓬����
  while (1) {
    if (!app_env.isOpen()) return;

    // �퓬�a�f�l�Đ�
    if (!add_attack) {
      if (!bgm_a.isPlaying()) bgm_a.play();
    }
    else {
      if (!bgm_b.isPlaying()) bgm_b.play();
    }

    // �퓬�I�������m�F
    {
      // ���͂��O
      if (P_pow <= 0 || E_pow <= 0) {
        bgm_a.stop();
        bgm_b.stop();
        break;
      }

      // �s���񐔂��O
      if (count == 0) {
        // �ǌ���ōs�������Ƃ�
        if (add_attack) {
          button_flag = 0;
          bgm_b.stop();
          break;
        }

        button_flag = -1;
        // �ǌ���˓��m�F
        while (1) {
          if (!app_env.isOpen()) return;

          app_env.setupDraw();

          // �}�E�X���W���擾
          mouse_pos = app_env.mousePosition();

          // �w�i�\��
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            mainBG, Color(1, 1, 1));

          // ����͓P�ނ���悤��
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
            0, telop_H * 1, telop_W, telop_H * 1,
            battle, Color(0, 0, 0));

          // �{�^���\��
          {
            for (i = 0; i < 2; i += 1) {
              on_mouse = on_button(mouse_pos, x + (menu_W + 2) * i, y, menu_W, menu_H);
              angle = setButtonColor(on_mouse);
              drawTextureBox(x + (menu_W + 2) * i, y, menu_W, menu_H,
                0, menu_H * (25 + i), menu_W, menu_H,
                button, angle);

              if (on_mouse) {
                if (app_env.isPushButton(Mouse::LEFT)) {
                  app_env.flushInput();

                  // �ǌ���˓�
                  if (i == 0) {
                    add_attack = true;
                    bgm_a.stop();
                    button_flag = 0;
                    break;
                  }
                  // �ǌ����Ȃ�
                  else {
                    button_flag = 5;
                    break;
                  }
                }
              }
            }
          }

          // �{�^�����I�����ꂽ�Ƃ�
          if (button_flag != -1) {
            break;
          }

          app_env.update();
        }

        // �ǌ����Ȃ��Ƃ�
        if (!add_attack) {
          bgm_a.stop();
          break;
        }
        // �ǌ���˓�
        else {
          button_flag = -1;
          count = 1;
        }
      } // count 0 end
    }

    app_env.setupDraw();

    // �}�E�X���W���擾
    mouse_pos = app_env.mousePosition();

    // �w�i�\��
    if (!add_attack) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 0.6, 0.2));
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 360,
        0, 360, Window::WIDTH, Window::HEIGHT - 360,
        mainBG, Color(1, 1, 1));
    }

    // ���̓Q�[�W�̌v�Z�ƕ\��
    {
      // �ǂ���̃Q�[�W���\��
      // �v���C���[��
      drawTextureBox((-Window::WIDTH / 2) + 20, (Window::HEIGHT / 2) - (34 + telop_H), status_W * 2, telop_H,
        0, telop_H * 23, status_W * 2, telop_H,
        battle, Color(0, 0, 0));
      // ���葤
      drawTextureBox((Window::WIDTH / 2) - status_W * 2, (Window::HEIGHT / 2) - (34 + telop_H), status_W * 2, telop_H,
        0, telop_H * 24, status_W * 2, telop_H,
        battle, Color(0, 0, 0));

      // �Q�[�W�̌v�Z�ƃJ���[�ݒ�
      setPowGauge();
      P_GaugeColor = setGaugeColor(P_powGauge);
      E_GaugeColor = setGaugeColor(E_powGauge);

      // �v���C���[���Q�[�W�̕\��
      {
        // ����
        // �Q�[�W�����̕\���i���j
        drawTextureBox((-Window::WIDTH / 2) + 20, (Window::HEIGHT / 2) - 32, 200, 32,
          0, 32, 200, 32,
          PowerG, Color(0, 0, 0));
        // ���݂̕��̓Q�[�W
        drawTextureBox((-Window::WIDTH / 2) + 20 + (200 - P_powGauge * 2), (Window::HEIGHT / 2) - 32, P_powGauge * 2, 32,
          0, 32, P_powGauge * 2, 32,
          PowerG, P_GaugeColor);

        // �`�E��
        // �Q�[�W�����̕\���i���j
        drawTextureBox((-Window::WIDTH / 2) + 180, (Window::HEIGHT / 2) - 32, 50, 32,
          400, 32, 50, 32,
          PowerG, Color(0, 0, 0));
        // ���݂̋`�E���Q�[�W
        drawTextureBox((-Window::WIDTH / 2) + 180 + (50 - P_Mgauge), (Window::HEIGHT / 2) - 32, P_Mgauge, 32,
          400, 32, P_Mgauge, 32,
          PowerG, Color(1, 0.5, 1));
      }

      // ���葤�Q�[�W�̕\��
      {
        // ����
        // �Q�[�W�����̕\���i���j
        drawTextureBox((Window::WIDTH / 2) - 220, (Window::HEIGHT / 2) - 32, 200, 32,
          200, 32, 200, 32,
          PowerG, Color(0, 0, 0));
        // ���݂̕��̓Q�[�W
        drawTextureBox((Window::WIDTH / 2) - 220, (Window::HEIGHT / 2) - 32, E_powGauge * 2, 32,
          200 + (100 - E_powGauge) * 2, 32, E_powGauge * 2, 32,
          PowerG, E_GaugeColor);

        // �`�E��
        // �Q�[�W�����̕\���i���j
        drawTextureBox((Window::WIDTH / 2) - 230, (Window::HEIGHT / 2) - 32, 50, 32,
          450, 32, 50, 32,
          PowerG, Color(0, 0, 0));
        // ���݂̋`�E���Q�[�W
        drawTextureBox((Window::WIDTH / 2) - 230, (Window::HEIGHT / 2) - 32, E_Mgauge, 32,
          450 + (50 - E_Mgauge), 32, E_Mgauge, 32,
          PowerG, Color(1, 0.5, 1));
      }

      // �t���[���̕\��
      drawTextureBox((-Window::WIDTH / 2), (Window::HEIGHT / 2) - 32, Window::WIDTH, 32,
        0, 0, Window::WIDTH, 32,
        PowerG, Color(1, 1, 1));
    }

    // �c��s���񐔂̕\��
    if (!add_attack) {
      drawTextureBox((-number_W / 2), (Window::HEIGHT / 2) - 28, number_W, telop_H,
        number_W * count, telop_H * 22, number_W, telop_H,
        battle, Color(1, 1, 1));
    }

    // �L�����N�^�[�̕\��
    {
      // �v���C���[��
      x0 = -chara_W * 4;
      y0 = -chara_H * 2;

      for (i = 0; i < 4; i += 1) {
        for (j = 0; j < 4; j += 1) {
          drawTextureBox(x0 - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j), y0 + chara_H * (2 - i * 0.5), chara_W, chara_H,
            0, 0, chara_W, chara_H,
            chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
        }
      }

      // ���葤
      x0 = chara_W * 3;
      y0 = -chara_H * 2;

      for (i = 0; i < 4; i += 1) {
        for (j = 0; j < 4; j += 1) {
          drawTextureBox(x0 + ((chara_W * (1 + i * 0.25)) * j), y0 + chara_H * (2 - i * 0.5), chara_W, chara_H,
            0, 0, chara_W, chara_H,
            chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
        }
      }
    }

    // �{�^���\��
    {
      // �P��
      if (!add_attack) {
        on_mouse = on_button(mouse_pos, x + (menu_W + 2) * 1, y, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x + (menu_W + 2) * 1, y, menu_W, menu_H,
          0, menu_H * 26, menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // �R�}���h���
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
            0, telop_H * 6, telop_W, telop_H * 1,
            battle, Color(0, 0, 0));

          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            bgm_a.stop();
            button_flag = 4;
            break;
          }
        }
      } // run away end

      // �ˌ��A���A���
      for (i = 0; i < 3; i += 1) {
        on_mouse = on_button(mouse_pos, x, y + (menu_H + 2) * (2 - i), menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x, y + (menu_H + 2) * (2 - i), menu_W, menu_H,
          0, menu_H * (22 + i), menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // �R�}���h�̉��
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
            0, telop_H * (3 + i), telop_W, telop_H * 1,
            battle, Color(0, 0, 0));

          // �N���b�N�Ŋm�F��ʕ\��
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            button_flag = i;
          }
        }
      }
    }

    // �퓬���s
    if (button_flag != -1) {
      duel_turnStart();
      E_command = duel_enemyCommand();
      duel_fight(button_flag, E_command);
      duel_anime(app_env, button_flag, E_command);
      duel_turnEnd();

      // �J�E���^�A�t���O�̃��Z�b�g
      button_flag = -1;
      count -= 1;
    }

    app_env.update();
  }

  // �퓬�I����̏���
  {
    // �P�ވȊO
    if (button_flag != 4) {
      // �v���C���[�Ƒ���̎c�蕺�͍����m�F
      if (P_powGauge >= E_powGauge) {
        button_flag = 0;
        P.territory += 1;
        E.territory -= 1;

        if (P.infamy > 0) {
          P.fame += 1;
          P.infamy -= 1;
        }
        else {
          P.fame += 2;
        }

        // ����̕��͂��O
        if ((E.soldier + E.mercenary) == 0) {
          P.territory += 1;
          E.territory -= 1;
          P.fame += 2;
          enemy_reaction();
        }

        // �A�C�e���l������
        switch (stage_flag) {

            // �P��
          case 1:
            // ���蕺�͂� 25% �ȏ�̃_���[�W��^����
            if ((E_pow * 100) / E_pow0 <= 75) {
              item_get = true;
              P_item[1] = 1;
            }
            break;

            // �Q��
          case 2:
            // ���蕺�͂� 50% �ȏ�̃_���[�W��^����
            if ((E_pow * 100) / E_pow0 <= 50) {
              item_get = true;
              P_item[2] = 1;
            }
            break;

            // �R��
          case 3:
            // ����� 10000 �ȏ�̃_���[�W��^����
            if (P_item[3] != 1 && (E_pow0 - E_pow) >= 10000) {
              item_get = true;
              P_item[3] = 1;
            }
            // ���蕺�͂� 25% �ȏ�̃_���[�W��^����
            else if ((E_pow * 100) / E_pow0 <= 75) {
              item_get = true;
              P_item[4] = 1;
            }
            break;

            // �S��
          case 4:
            // ���蕺�͂� 50% �ȏ�̃_���[�W��^����
            if ((E_pow * 100) / E_pow0 <= 50) {
              item_get = true;
              P_item[6] = 1;
            }
            // ���蕺�͂� 25% �ȏ�̃_���[�W��^����
            else if ((E_pow * 100) / E_pow0 <= 75) {
              item_get = true;
              P_item[5] = 1;
            }
            break;

            // �T��
          case 5:
            // ���蕺�͂� 50% �ȏ�̃_���[�W��^����
            if ((E_pow * 100) / E_pow0 <= 50) {
              item_get = true;
              P_item[5] = 1;
            }
            // ���蕺�͂� 40% �ȏ�̃_���[�W��^����
            else if ((E_pow * 100) / E_pow0 <= 60) {
              item_get = true;
              P_item[4] = 1;
            }
            // ���蕺�͂� 30% �ȏ�̃_���[�W��^����
            else if ((E_pow * 100) / E_pow0 <= 70) {
              item_get = true;
              P_item[2] = 1;
            }
            // ���蕺�͂� 20% �ȏ�̃_���[�W��^����
            else if ((E_pow * 100) / E_pow0 <= 80) {
              item_get = true;
              P_item[1] = 1;
            }
            // ���蕺�͂� 10% �ȏ�̃_���[�W��^����
            else if ((E_pow * 100) / E_pow0 <= 90) {
              item_get = true;
              P_item[0] = 1;
            }
            break;
        }
      }
      else {
        button_flag = 1;
        se_lose.play();

        // �v���C���[�̗̒n���O�ł͂Ȃ�
        if (P.territory != 0) {
          P.territory -= 1;
          E.territory += 1;
          P.fame -= 1;
        }
      }

      // �������̂r�d�Đ�
      if (button_flag == 0) {
        if (item_get)
          se_get.play();
        else
          se_win.play();
      }
    }

    // �P��
    else {
      se_away.play();
    }
  }

  // �퓬�I����ʂ̕\��
  while (1) {
    if (!app_env.isOpen()) return;

    // �N���b�N�Ń��[�v�I��
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      P.mercenary = 0;
      E.mercenary = 0;
      add_attack = false;
      ex_flag += 1;
      break;
    }

    app_env.setupDraw();

    // �w�i�\��
    if (!add_attack) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 0.6, 0.2));
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 360,
        0, 360, Window::WIDTH, Window::HEIGHT - 360,
        mainBG, Color(1, 1, 1));
    }

    // �퓬���ʃe�L�X�g
    switch (button_flag) {

        // ����
      case 0:
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 9, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));

        // �A�C�e���擾
        if (item_get) {
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
            0, telop_H * 10, telop_W, telop_H * 1,
            battle, Color(0, 0, 0));
        }
        break;

        // �s�k
      case 1:
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 11, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));
        break;

        // �P��
      case 4:
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 8, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));
        break;
    }

    app_env.update();
  }
}

// �Z�[�u
void save(AppEnv& app_env) {
  // �w�i�A�e�L�X�g�摜�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture system("res/png/Me4_System.png");

  if (yes_or_no(app_env, 1) != 1) {
    return;
  }
  else {
    std::ofstream save("res/Savedata.txt");
    if (save) {
      save << stage_flag << std::endl;
      save << ex_flag << std::endl;

      save << E.money << std::endl;
      save << E.fund << std::endl;
      save << E.territory << std::endl;
      save << E.soldier << std::endl;
      save << E.mercenary << std::endl;
      save << E.fame << std::endl;
      save << E.infamy << std::endl;

      save << P.money << std::endl;
      save << P.fund << std::endl;
      save << P.territory << std::endl;
      save << P.soldier << std::endl;
      save << P.mercenary << std::endl;
      save << P.fame << std::endl;
      save << P.infamy << std::endl;

      save << P_fund[0] << std::endl;
      save << P_fund[1] << std::endl;
      save << P_fund[2] << std::endl;
      save << P_fund[3] << std::endl;
      save << P_fund[4] << std::endl;
      save << P_fund[5] << std::endl;
      save << P_fund[6] << std::endl;
      save << P_fund[7] << std::endl;
      save << P_fund[8] << std::endl;
      save << P_fund[9] << std::endl;
      save << P_fund[10] << std::endl;
      save << P_fund[11] << std::endl;
      save << P_fund[12] << std::endl;
      save << P_fund[13] << std::endl;

      save << P_item[0] << std::endl;
      save << P_item[1] << std::endl;
      save << P_item[2] << std::endl;
      save << P_item[3] << std::endl;
      save << P_item[4] << std::endl;
      save << P_item[5] << std::endl;
      save << P_item[6] << std::endl;
    }
  }

  while (1) {
    if (!app_env.isOpen()) return;

    // ���N���b�N�Ń��[�v�I��
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      break;
    }

    app_env.setupDraw();

    // �w�i�̕\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �e�L�X�g�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
      0, telop_H * 6, telop_W, telop_H * 1,
      system, Color(0, 0, 0));

    app_env.update();
  }
}


// �Q�[���{��
////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  AppEnv app_env(Window::WIDTH, Window::HEIGHT);

  // �摜�̓ǂݍ���
  Texture blackBG("res/png/BG5_Black.png");
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");
  Texture event2("res/png/BG3_Castle_out.png");
  Texture system("res/png/Me4_System.png");
  Texture button("res/png/Me1_MenuList.png");

  // �{�^���̕\���ʒu�A�J�[�\������A�F
  float x, y;
  bool on_mouse;
  Color angle;

  // �^�C�g�� //--------------------------------------------------------------------------------
  {
    // �J�n��ʂ̉摜�ǂݍ���
    Texture title_bg("res/png/Sp3_TitleBack.png");
    Texture title_OP("res/png/Sp4_Opening.png");
    Texture title_LG("res/png/Sp1_TitleLogo_b.png");
    Texture select("res/png/Sp2_Select.png");

    // �a�f�l�̐ݒ�
    Media BGM("res/wav/Event_Opening.wav");
    BGM.looping(true);
    BGM.gain(0.2);
    BGM.play();

    // �{�^���̃T�C�Y
    short select_w = 208, select_h = 56;

    // �^�C�g���̃T�C�Y
    short title_h = 64;

    while (1) {
      if (!app_env.isOpen()) return 0;
      app_env.setupDraw();

      // �J�[�\�����W�̎擾
      mouse_pos = app_env.mousePosition();

      // �w�i�\��
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        title_bg, Color(1, 1, 1));

      // �^�C�g���\��
      drawTextureBox((-Window::WIDTH / 2), (Window::HEIGHT / 4), Window::WIDTH, title_h,
        0, 0, Window::WIDTH, title_h,
        title_LG, Color(1, 1, 0.5));

      // �{�^���\��
      {
        // Continue
        {
          // �{�^���摜�̕\�����W�ݒ�ƃ}�E�X����ɂ��摜�J���[�̐ݒ�
          x = (-select_w / 2);
          y = (-Window::HEIGHT / 6);
          on_mouse = on_button(mouse_pos, x, y, select_w, select_h);
          angle = setButtonColor(on_mouse);

          drawTextureBox(x, y, select_w, select_h,
            0, select_h * 1, select_w, select_h,
            select, angle);

          // �}�E�X�N���b�N�̉ۂƃN���b�N�����Ƃ��̏���
          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();

              // �Z�[�u�f�[�^�̊m�F
              std::ifstream save("res/Savedata.txt");

              // �Z�[�u�f�[�^������ꍇ
              if (save) {
                // �Z�[�u�f�[�^�̓ǂݍ��݊m�F
                button_flag = yes_or_no(app_env, 0);

                // �������ُ�I�������Ƃ�
                if (button_flag == 0) return 0;

                // �Z�[�u�f�[�^�𐳂����ǂݍ��߂��Ƃ�
                if (button_flag == 1) stage_flag = 6;
              }

              // �Z�[�u�f�[�^���Ȃ������ꍇ
              else {
                while (1) {
                  if (!app_env.isOpen()) return 0;

                  // �}�E�X�N���b�N�Ń^�C�g���ɖ߂�
                  if (app_env.isPushButton(Mouse::LEFT)) {
                    app_env.flushInput();
                    break;
                  }

                  app_env.setupDraw();

                  // �w�i�̕\��
                  drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
                    0, 0, Window::WIDTH, Window::HEIGHT,
                    title_bg, Color(1, 1, 1));
                  drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
                    0, 365, Window::WIDTH, Window::HEIGHT - 365,
                    event1, Color(1, 1, 1));

                  // �G���[���b�Z�[�W�̕\��
                  drawTextureBox(-telop_W / 2, (-Window::HEIGHT / 2) + (telop_H * 5), telop_W, telop_H * 1,
                    0, telop_H * 1, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));

                  app_env.update();
                }
              } // No Data end
            }
          } // on_mouse end
        }

        // New Game
        {
          // �{�^���摜�̕\�����W�ݒ�ƃ}�E�X����ɂ��摜�J���[�̐ݒ�
          x = (-select_w / 2);
          y = (-Window::HEIGHT / 3);
          on_mouse = on_button(mouse_pos, x, y, select_w, select_h);
          angle = setButtonColor(on_mouse);

          drawTextureBox(x, y, select_w, select_h,
            0, select_h * 0, select_w, select_h,
            select, angle);

          // �}�E�X�N���b�N�̉ۂƃN���b�N�����Ƃ��̏���
          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();

              // �I�[�v�j���O����
              _BG_slide(app_env, title_bg, title_OP, Color(1, 1, 1), Color(1, 1, 1));
              _BG_fade(app_env, title_OP, 1.0, 0.5, 0.02);
              _Opening(app_env);
              _BG_fade(app_env, title_OP, 0.5, 0, 0.02);

              button_flag = 1;
            }
          }
        }
      } // button end

      // �ǂ��炩������ɏ������ꂽ��{�ҊJ�n
      if (button_flag == 1) {
        BGM.stop();
        break;
      }

      app_env.update();
    }
  } // title Loop end

  // �摜�̓ǂݍ���
  Texture titleEP("res/png/Sp6_BG_Title.png");
  Texture s_window("res/png/Me7_StatusWindow.png");
  Texture target("res/png/Me8_target.png");

  // �Q�[���{�� //--------------------------------------------------------------------------------
  while (1) {
    if (!app_env.isOpen()) return 0;

    // �X�g�[���[ //--------------------------------------------------------------------------------
    {
      // �e���b�v�\���̐ݒ�
      switch (stage_flag) {

          // �v�����[�O
        case 0:
          // �p�����[�^�̏�����
          P = {500, 10, 0, 10, 0, 0, 0};
          E = {0, 0, 0, 10, 0, 0, 0};

          // �C�x���g�J�n
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _Prologue_event(app_env);
          break;

          // �P��
        case 1:
          // �p�����[�^�̏�����
          E = {2000, 5000, 5, 100, 0, 0, 0};

          // �C�x���g�J�n
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _1ststage_event(app_env);
          break;

          // �Q��
        case 2:
          // �p�����[�^�̏�����
          E = {5000, 15000, 5, 200, 100, 0, 0};

          // �C�x���g�J�n
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _2ndstage_event(app_env);
          break;

          // �R��
        case 3:
          // �p�����[�^�̏�����
          E = {300000, 200000, 10, 20000, 0, 0, 80};

          // �C�x���g�J�n
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _3rdstage_event(app_env);
          break;

          // �S��
        case 4:
          // �p�����[�^�̏�����
          E = {150000, 300000, 10, 15000, 1000, 80, 0};

          // �C�x���g�J�n
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _4thstage_event(app_env);
          _BG_slide(app_env, event1, mainBG, Color(1, 1, 1), Color(1, 1, 1));
          break;

          // �T��
        case 5:
          // �p�����[�^�̏�����
          E = {300000, 300000, 10, 20000, 0, 80, 0};

          // �C�x���g�J�n
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _5thstage_event(app_env);
          _BG_slide(app_env, event2, mainBG, Color(1, 1, 1), Color(1, 1, 1));
          break;

          // �f�[�^�ǂݍ��ݎ��̏���
        default:
          // �Z�[�u�f�[�^�̓��e���Q�[���ɔ��f
          std::ifstream save("res/Savedata.txt");
          save >> stage_flag
            >> ex_flag

            // ����̏����擾
            >> E.money
            >> E.fund
            >> E.territory
            >> E.soldier
            >> E.mercenary
            >> E.fame
            >> E.infamy

            // �v���C���[�̏����擾
            >> P.money
            >> P.fund
            >> P.territory
            >> P.soldier
            >> P.mercenary
            >> P.fame
            >> P.infamy

            // ���L�A�C�e�������擾
            >> P_fund[0]
            >> P_fund[1]
            >> P_fund[2]
            >> P_fund[3]
            >> P_fund[4]
            >> P_fund[5]
            >> P_fund[6]
            >> P_fund[7]
            >> P_fund[8]
            >> P_fund[9]
            >> P_fund[10]
            >> P_fund[11]
            >> P_fund[12]
            >> P_fund[13]

            // ���p�\�ȃA�C�e�������擾
            >> P_item[0]
            >> P_item[1]
            >> P_item[2]
            >> P_item[3]
            >> P_item[4]
            >> P_item[5]
            >> P_item[6];

          // �ǂݍ��݌�̃X�e�[�W�^�C�g����\��
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, mainBG, Color(1, 1, 1), Color(1, 1, 1));
          break;
      }
    } // story event end

    // ���C�����[�v //--------------------------------------------------------------------------------
    {
      // �a�f�l�̐ݒ�
      Media bgm_main = setBGM_Main(stage_flag);
      bgm_main.looping(true);
      bgm_main.gain(0.2);

      // �摜�\���p�J�E���^
      short i = 0;

      // �e��t���O�̃��Z�b�g
      spy = 0;
      sabo = 0;
      rumor = false;
      rebel = false;
      disclos = false;
      add_attack = false;
      E_time = 0;
      E_action = false;

      while (1) {
        if (!app_env.isOpen()) return 0;

        // �a�f�l�Đ�
        if (!bgm_main.isPlaying()) bgm_main.play();

        // ���������𖞂����Ă��邩�m�F
        {
          // �v�����[�O
          if (stage_flag == 0) {
            bgm_main.stop();
            _tutorial(app_env);
            stage_clear = true;
          }

          // ����̗̒n�����ׂĐ�̂���
          if (E.territory <= 0) {
            stage_clear = true;
          }

          // ��������
          if (rebel) {
            // �R�͂̂Ƃ�
            if (stage_flag == 3) {
              bgm_main.stop();
              _BG_slide(app_env, mainBG, event1, Color(1, 1, 1), Color(1, 1, 1));
              _3rdstage_clear(app_env);
              _BG_fade(app_env, event1, 1.0, 0, 0.02);
            }
            stage_clear = true;
          }

          // �ړI�̕����𔃎�����
          if (stage_flag == 1 && P_fund[0] == 1) {
            stage_clear = true;
          }
          else if (stage_flag == 2) {
            if (P_fund[3] == 1 && P_fund[4] == 1 && P_fund[5] == 1) {
              stage_clear = true;
            }
          }

          // �S�́F�A�C�e���𔄋p����
          if (stage_flag == 4 && P_item[6] == 2) {
            bgm_main.stop();
            _BG_slide(app_env, mainBG, event1, Color(1, 1, 1), Color(1, 1, 1));
            _4thstage_clear(app_env);
            _BG_fade(app_env, event1, 1.0, 0, 0.02);
            stage_clear = true;
          }
        }

        // ���������B���Ȃ烋�[�v�I��
        if (stage_clear) {
          bgm_main.stop();
          break;
        }

        // �p�����[�^�����E�l�𒴂��ĂȂ����m�F
        {
          // �v���C���[
          {
            // ����
            if (P.money > 999999) P.money = 999999;
            else if (P.money < 0) P.money = 0;

            // �����Y�z
            if (P.fund > 999999) P.fund = 999999;
            else if (P.fund < 0) P.fund = 0;

            // �̒n
            if (P.territory > 100) P.territory = 100;
            else if (P.territory < 0) P.territory = 0;

            // ���m
            if (P.soldier > 65535) P.soldier = 65535;
            else if (P.soldier < 0) P.soldier = 0;

            // �`�E��
            if (P.mercenary > 65535) P.mercenary = 65535;
            else if (P.mercenary < 0) P.mercenary = 0;

            // ����
            if (P.fame > 100) P.fame = 100;
            else if (P.fame < 0) P.fame = 0;

            // ����
            if (P.infamy > 100) P.infamy = 100;
            else if (P.infamy < 0) P.infamy = 0;
          }

          // ����
          {
            // ����
            if (E.money > 999999) E.money = 999999;
            else if (E.money < 0) E.money = 0;

            // �����Y�z
            if (E.fund > 999999) E.fund = 999999;
            else if (E.fund < 0) E.fund = 0;

            // �̒n
            if (E.territory > 100) E.territory = 100;
            else if (E.territory < 0) E.territory = 0;

            // ���m
            if (E.soldier > 65535) E.soldier = 65535;
            else if (E.soldier < 0) E.soldier = 0;

            // �`�E��
            if (E.mercenary > 65535) E.mercenary = 65535;
            else if (E.mercenary < 0) E.mercenary = 0;

            // ����
            if (E.fame > 100) E.fame = 100;
            else if (E.fame < 0) E.fame = 0;

            // ����
            if (E.infamy > 100) E.infamy = 100;
            else if (E.infamy < 0) E.infamy = 0;
          }
        } // status limit check end

        // ����̍s���J�n�܂ł̃J�E���g
        if (E_time < 7200) {
          E_time += 1;
        }
        // ����̍s��
        else {
          switch (stage_flag) {

            case 1:
              enemy_reaction();
              break;

            case 2:
              duel_standby();
              if (E_pow > P_pow * 2) {
                bgm_main.stop();
                E_action = true;
                duel(app_env);
                E_action = false;
              }
              else {
                enemy_reaction();
              }
              break;

            case 3:
              duel_standby();
              if (E_pow > P_pow * 3) {
                bgm_main.stop();
                E_action = true;
                duel(app_env);
                E_action = false;
              }
              else {
                enemy_reaction();
              }
              break;

            case 4:
              duel_standby();
              if (E_pow > P_pow * 2) {
                bgm_main.stop();
                E_action = true;
                duel(app_env);
                E_action = false;
              }
              else {
                enemy_reaction();
              }
              break;

            case 5:
              enemy_reaction();

              // ���͍��ŕ����Ă���Ƃ�
              duel_standby();
              if (E_pow < P_pow) {
                enemy_reaction();
              }

              bgm_main.stop();
              E_action = true;
              duel(app_env);
              E_action = false;
              break;
          }

          // �^�C���o�[�̃��Z�b�g
          E_time = 0;
        }

        // �^�C���o�[�̒����ƐF�ݒ�
        if (E_time < 7200) {
          E_timeGauge = (7200 - E_time) / 60;
          timeColor = setTimeColor(E_timeGauge);
        }

        app_env.setupDraw();

        // �J�[�\�����W�̎擾
        mouse_pos = app_env.mousePosition();

        // �w�i�̕\��
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));

        // �^�C���o�[�̕\��
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
          0, telop_H * 99, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        drawFillBox(-30, (-Window::HEIGHT / 2) + telop_H * 2, E_timeGauge * 2, 20, timeColor);

        // �p�����[�^�E�B���h�E�̕\��
        drawTextureBox(0, (-Window::HEIGHT / 2), (Window::WIDTH / 2), Window::HEIGHT,
          0, 0, (Window::WIDTH / 2), Window::HEIGHT,
          s_window, Color(1, 1, 1));

        // ���������̕\��
        drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 14, (Window::WIDTH / 2), telop_H,
          0, telop_H * 0, (Window::WIDTH / 2), telop_H,
          target, Color(0, 0, 0));
        drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 15, (Window::WIDTH / 2), telop_H,
          0, telop_H * 5, (Window::WIDTH / 2), telop_H,
          target, Color(0, 0, 0));
        if (stage_flag > 0 && stage_flag < 5) {
          drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 16, (Window::WIDTH / 2), telop_H,
            0, telop_H * stage_flag, (Window::WIDTH / 2), telop_H,
            target, Color(0, 0, 0));
        }

        // �p�����[�^�̕\��
        {
          // �v���C���[
          {
            // ���
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 2, status_W * 2, telop_H,
              0, telop_H * 88, status_W * 2, telop_H,
              system, Color(0, 0.4, 0.8));

            // ����
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 3, status_W * 1, telop_H,
              0, telop_H * 90, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(P.money);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 3, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 3, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }

            // ���m
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 4, status_W * 1, telop_H,
              0, telop_H * 91, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(P.soldier);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 4, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 4, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }

            // �`���i�`�E���j
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 5, status_W * 1, telop_H,
              0, telop_H * 92, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(P.mercenary);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 5, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 5, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }

            // ����
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 6, status_W * 1, telop_H,
              0, telop_H * 93, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(P.fame);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 6, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 6, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }

            // ����
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 7, status_W * 1, telop_H,
              0, telop_H * 94, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(P.infamy);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 7, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 7, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }
          } // player end

          // ����
          if (spy > 0) {
            // ���
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 9, status_W * 2, telop_H,
              0, telop_H * 89, status_W * 2, telop_H,
              system, Color(0.8, 0.2, 0.2));

            // ����
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 10, status_W * 1, telop_H,
              0, telop_H * 90, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(E.money);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 10, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 10, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }

            // ���m
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 11, status_W * 1, telop_H,
              0, telop_H * 91, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(E.soldier);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 11, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 11, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }

            // �̒n
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 12, status_W * 1, telop_H,
              0, telop_H * 95, status_W * 1, telop_H,
              system, Color(0, 0, 0));
            numberGet(E.territory);
            for (i = 0; i < 6; i += 1) {
              if (i < skip) {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 12, number_W, telop_H,
                  number_W * val[i], telop_H * 98, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
              else {
                drawTextureBox(status_W + number_W * (-skip + i + 1), (Window::HEIGHT / 2) - telop_H * 12, number_W, telop_H,
                  number_W * val[i], telop_H * 97, number_W, telop_H,
                  system, Color(0, 0, 0));
              }
            }
          } // enemy end
        } // status window end

        // �{�^���̕\��
        {
          // �{�^���̕\���ʒu�ݒ�
          x = (-Window::WIDTH / 2) + 2;
          y = (Window::HEIGHT / 2);

          for (i = 0; i < 6; i += 1) {
            on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * (i + 1), menu_W, menu_H);
            if (i == 4 && P.soldier + P.mercenary == 0) {
              angle = Color(0.5, 0.5, 0.5);
            }
            else {
              angle = setButtonColor(on_mouse);
            }
            drawTextureBox(x, y - (menu_H + 2) * (i + 1), menu_W, menu_H,
              0, menu_H * i, menu_W, menu_H,
              button, angle);

            if (on_mouse) {
              // �R�}���h���
              switch (i) {

                  // �����J��
                case 0:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 8, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // ���͑���
                case 1:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 23, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // �v��
                case 2:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 35, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // ����
                case 3:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 74, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // �N�U
                case 4:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 84, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));

                  // ���͂O�Ȃ���s�s��
                  if (P.soldier + P.mercenary == 0) {
                    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
                      0, telop_H * 86, telop_W, telop_H * 1,
                      system, Color(0, 0, 0));
                  }
                  break;

                  // �Z�[�u
                case 5:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                    0, telop_H * 3, telop_W, telop_H * 2,
                    system, Color(0, 0, 0));
                  break;
              }
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();

                // �R�}���h���s�J�n
                switch (i) {

                    // �����J��
                  case 0:
                    add_money(app_env);
                    break;

                    // ���͑���
                  case 1:
                    add_soldier(app_env);
                    break;

                    // �v��
                  case 2:
                    trick(app_env);
                    break;

                    // ����
                  case 3:
                    buy_fund(app_env);
                    break;

                    // �N�U
                  case 4:
                    // ���͂O�Ȃ���s�s��
                    if ((P.soldier + P.mercenary) != 0) {
                      // �͂���I������Ɛ퓬�J�n
                      if (yes_or_no(app_env, 2) == 1) {
                        bgm_main.stop();
                        duel(app_env);
                      }
                    }
                    break;

                    // �Z�[�u
                  case 5:
                    save(app_env);
                    break;
                }
              }
            }
          }
        }

        app_env.update();
      }
    } // main Loop end

    // �N���A��ʂ̕\�� //--------------------------------------------------------------------------------
    {
      // �T�̓N���A
      if (stage_flag == 5) {
        _5thstage_clear(app_env);
      }

      _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
      _titleCall(app_env);
      _BG_fade(app_env, titleEP, 1.0, 0, 0.02);
      stage_clear = false;
      stage_flag += 1;

      // �P�̓N���A���̂݁A�T�͓˓��t���O����x���Z�b�g
      if (stage_flag == 2) {
        ex_flag = 0;
      }

      // �G���f�B���O����
      // �T�̓N���A
      if (stage_flag == 6) {
        _Ending(app_env);
      }
      // �S�̓N���A���_�ŁA�s���񐔂� 100��𒴂��Ă���Ƃ�
      else if (stage_flag == 5 && ex_flag > 100) {
        _Ending(app_env);
      }

      // �N���A�{�[�i�X�̔��f
      if (stage_flag == 2 && P_fund[0] == 0) {
        P_fund[0] = 1;
        P.fund += fundinfo[0];
      }
      P.money += (P.fund / 10 + P.territory * 100 + (P.fame - P.infamy) * 10);
      P.fame += 5;
      P.infamy -= 5;
    }

  } // game Loop end
}

////////////////////////////////////////////////////////////////////////////////////////////////////

