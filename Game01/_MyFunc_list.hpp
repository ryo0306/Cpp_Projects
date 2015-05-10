//
// Revival of Kingdom �̊O�������֐�
//

#pragma once
#include "appEnv.hpp"
#include "_MyVar_list.hpp"    // �ϐ����X�g

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �ڎ�
  �E�G�t�F�N�g����
  �E���ʏ����֌W
  �E�퓬�����֌W
  �E�V�~�����[�V�����p�[�g�֌W
  �E�C�x���g�����i�e�L�X�g�\���ƃA�j���[�V�����n�j
 */


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
�G�t�F�N�g����
void _BG_slide(AppEnv&, Texture&, Texture&);
void _BG_fade(AppEnv&, Texture&, float, float, float);
void _titleCall(AppEnv&, short);
*/

// �w�i�摜�̐؂�ւ�
void _BG_slide(AppEnv& app_env, Texture& tex_before, Texture& tex_after, Color& before, Color& after) {
  float x = Window::WIDTH;
  while (x > 0) {
    if (!app_env.isOpen()) return;

    // �N���b�N������A�j���I��
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      x = 0;
    }

    app_env.setupDraw();

    // �؂�ւ��O
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      tex_before, before);

    // �؂�ւ���̔w�i
    drawTextureBox(x - (Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH - x, Window::HEIGHT,
      x, 0, Window::WIDTH - x, Window::HEIGHT,
      tex_after, after);

    // �؂�ւ��X�s�[�h
    x -= 4;

    app_env.update();
  }
}

// �w�i�摜�̃t�F�[�h�A�E�g�A�t�F�[�h�C��
// angle �� 0 �����̐��l�ɂ���ƃt�F�[�h�C��
void _BG_fade(AppEnv& app_env, Texture& tex, float base, float limit, float angle) {
  float fade = base;

  while (1) {
    if (!app_env.isOpen()) return;

    // ���[�v�I������
    if (angle > 0) {
      if (fade <= limit) {
        return;
      }
    }
    else if (angle < 0) {
      if (fade >= limit) {
        return;
      }
    }
    else {
      return;
    }

    // �N���b�N������A�j���I��
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      fade = limit;
    }

    app_env.setupDraw();

    // �w�i
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      tex, Color(fade, fade, fade));

    fade -= angle;

    app_env.update();
  }
}

// �͊J�n���A�N���A���̃^�C�g���\��
void _titleCall(AppEnv& app_env) {
  // �摜�̓ǂݍ���
  Texture titleEP("res/png/Sp6_BG_Title.png");
  Texture telopEP("res/png/Sp7_BG_Telop.png");

  // �N���A���̌��ʉ�
  Media clearSE("res/wav/se_clear.wav");
  clearSE.gain(0.2);

  // �^�C�g���p�e�L�X�g�摜�̃T�C�Y
  float title_w = 352, title_h = 28;

  // �A�j���[�V�����p
  short time = 0;
  float y = 0;
  float y0 = title_h * (stage_flag * 2);

  // �X�e�[�W�N���A���Ɍ��ʉ���炷
  if (stage_clear) {
    clearSE.play();
  }

  while (1) {
    if (!app_env.isOpen()) return;

    // �N���b�N������A�j���I��
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      break;
    }

    // �A�j���[�V��������
    if (time < 1000) {
      y += 0.5;
      time += 1;
    }

    app_env.setupDraw();

    // �w�i
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      titleEP, Color(1, 1, 1));

    // �͂̔ԍ�
    if (y < title_h) {
      drawTextureBox((-title_w / 2), (Window::HEIGHT / 4) - (title_h - y), title_w, y,
        0, y0, title_w, y,
        telopEP, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-title_w / 2), (Window::HEIGHT / 4), title_w, title_h,
        0, y0, title_w, title_h,
        telopEP, Color(1, 1, 1));
    }

    // �͂̃^�C�g��
    if (y > title_h + 2 && y < (title_h * 2) + 2) {
      drawTextureBox((-title_w / 2), (Window::HEIGHT / 8) - ((title_h * 2) + 2 - y), title_w, y - (title_h + 2),
        0, y0 + title_h, title_w, y - (title_h + 2),
        telopEP, Color(1, 1, 1));
    }
    else if (y >= (title_h * 2) + 2) {
      drawTextureBox((-title_w / 2), (Window::HEIGHT / 8), title_w, title_h,
        0, y0 + title_h, title_w, title_h,
        telopEP, Color(1, 1, 1));
    }

    // �X�e�[�W�N���A
    if (stage_clear) {
      if (y > (title_h * 2) + 4 && y < (title_h * 3) + 4) {
        drawTextureBox((-title_w / 2), (-Window::HEIGHT / 4) - ((title_h * 3) + 4 - y), title_w, y - ((title_h * 2) + 4),
          0, title_h * 13, title_w, y - ((title_h * 2) + 4),
          telopEP, Color(1, 1, 0));
      }
      else if (y >= (title_h * 3) + 4) {
        drawTextureBox((-title_w / 2), (-Window::HEIGHT / 4), title_w, title_h,
          0, title_h * 13, title_w, title_h,
          telopEP, Color(1, 1, 0));
      }
    }

    app_env.update();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
���ʏ����֌W
void numberGet()
void enemy_reaction()
*/

// �p�����[�^�̌������擾
void numberGet(int val0) {
  short i;

  // ������������
  for (i = 0; i < 6; i += 1) {
    val[i] = 0;
  }
  skip = 0;

  // ���ꂼ��̌��̐��l���擾
  i = 5;
  if (val0 != 0) {
    while (val0 != 0) {
      val[i] = val0 % 10;
      val0 = val0 / 10;
      i -= 1;
    }
  }
  else {
    i = 4;
  }

  // ��ʂ̌����O�݂̂Ȃ�X�L�b�v
  i += 1;
  skip = i;
}

// �v���C���[�̍s���ɑ΂��鑊��̔���
void enemy_reaction() {
  short sabo_flag = 1;
  short val;

  // �W�Q�H�삪���s���Ȃ�S�Ă̌��ʂ𔼌�
  if (sabo > 0) {
    sabo_flag = 2;
    sabo -= 1;
  }

  // �����𓾂�
  E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;

  // ���͂𑝋�����
  switch (stage_flag) {
    case 0:
      break;

      // �P��
      // ���m�̑����̂�
    case 1:
      // ���m
      if (E.soldier < 200) {
        val = (200 - E.soldier);

        if (val > 50) {
          val = 50;
        }

        val = val / sabo_flag;

        if (E.money < val * 10) {
          val = E.money / 10;
        }

        E.money -= val * 10;
        E.soldier += val;
      }

      break;

      // �Q��
      // ���m�Ƌ`�E���̑���
    case 2:
      // ���m
      if (E.soldier < 1500) {
        val = 150 / sabo_flag;

        if (E.money < val * 10) {
          val = E.money / 10;
        }

        E.money -= val * 10;
        E.soldier += val;
      }

      // �`�E��
      if (E.money > 2500) {
        if (E.mercenary < 400) {
          val = 100 / sabo_flag;
          E.money -= val * 5;
          E.mercenary += val;
        }
      }

      break;

      // �R��
      // ���m�Ǝ����̑���
    case 3:
      // ����
      if (E.money < 100000) {
        E.money += (E.fund / 10 + E.territory * 10) / sabo_flag;
      }

      // ���m
      if (E.soldier < 10000) {
        val = 5000 / sabo_flag;

        if (E.money < val * 10) {
          val = E.money / 10;
        }

        E.money -= val * 10;
        E.soldier += val;
      }
      else {
        val = (20000 - E.soldier) * 2;

        if (val > 1000) {
          val = 1000;
        }

        val = val / sabo_flag;

        if (E.money < val * 10) {
          val = E.money / 10;
        }

        E.money -= val * 10;
        E.soldier += val;
      }

      break;

      // �S��
      // ���m�A�`�E���A�����̑���
    case 4:
      // ���m
      if (E.soldier < 10000) {
        val = 5000 / sabo_flag;

        if (E.money < val * 10) {
          val = E.money / 10;
        }

        E.money -= val * 10;
        E.soldier += val;
      }
      else {
        val = (30000 - E.soldier) * 2;

        if (val > 2000) {
          val = 2000;
        }

        val = val / sabo_flag;

        if (E.money < val * 10) {
          val = E.money / 10;
        }

        E.money -= val * 10;
        E.soldier += val;
      }

      // �`�E��
      if (E.money > 50000) {
        if (E.mercenary < 5000) {
          val = 5000 / sabo_flag;

          if (val > 1000) {
            val = 1000;
          }

          E.money -= val * 5;
          E.mercenary += val;
        }
      }

      // ����
      if (E.money < 75000) {
        E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;
      }

      break;

      // �T�́A�f�o�b�O
      // ���m�Ǝ����̑���
    default:
      // ����
      E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;

      // ���m
      if (E.money > 50000) {
        if (E.soldier < 10000) {
          val = 10000 / sabo_flag;

          if (E.money < val * 10) {
            val = E.money / 10;
          }

          E.money -= val * 10;
          E.soldier += val;
        }
        else {
          val = (65535 - E.soldier) * 2;

          if (val > 5000) {
            val = 5000;
          }

          val = val / sabo_flag;

          if (E.money < val * 10) {
            val = E.money / 10;
          }

          E.money -= val * 10;
          E.soldier += val;
        }
      }

      // ����
      if (E.money < 50000) {
        E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;
      }

      break;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �퓬�����֌W
  void duel_standby();
  void duel_turnStart();
  void duel_fight(short, short);
  void duel_turnEnd();
  int duel_enemyCommand();
  void duel_anime(AppEnv&, short, short);

  void setPowGauge();
  Color setGaugeColor(float);

  Media setBGM_a(short);
  Media setBGM_b(short);
 */

// �퓬�J�n�O�F�U���͊�l�i�l�]�l�j�̌v�Z�ƕ��͍ő�l�̕ۑ�
void duel_standby() {
  // �l�]�l�̌v�Z
  P_pop = P.fame - P.infamy;
  if (P_pop < -20) P_pop = -20;
  E_pop = E.fame - E.infamy;
  if (E_pop < -20) E_pop = -20;


  // ���͂̌v�Z�Ə����l�i�ő�l�j�̕ۑ�
  P_pow = P.soldier * 10 + P.mercenary * 5;
  P_pow0 = P_pow;
  E_pow = E.soldier * 10 + E.mercenary * 5;
  E_pow0 = E_pow;

  // �`�E���l���̕ۑ�
  P_M0 = P.mercenary;
  E_M0 = E.mercenary;
}

// �퓬�^�[���J�n�F���̎��_�ł̍U���͂̌v�Z�ƕ��͕ۑ�
void duel_turnStart() {
  // �v���C���[�̍U���͌v�Z�Ə������
  P_attack = P.soldier * (2 + (P_pop / 20)) + P.mercenary * 4;
  if (P_attack > 10000) P_attack = 10000;

  // ����̍U���͌v�Z�Ə������
  E_attack = E.soldier * (2 + (E_pop / 20)) + E.mercenary * 4;
  if (E_attack > 10000) E_attack = 10000;

  // ���͕ۑ�
  P_start = P_pow;
  E_start = E_pow;
}

// �퓬�^�[�������F�I�������s���̑�������ƃ_���[�W����
void duel_fight(short player, short enemy) {

  // ��������
  {
    // �ˌ��@�Ɓ@�ˌ�
    if (player == 0 && enemy == 0) {
      P_damage = P_attack * 1;
      E_damage = E_attack * 1;
    }

    // �ˌ��@�Ɓ@���
    else if (player == 0 && enemy == 1) {
      P_damage = 0;
      E_damage = E_attack * 1;
    }

    // �ˌ��@�Ɓ@���
    else if (player == 0 && enemy == 2) {
      P_damage = P_attack * 1.5;
      E_damage = E_attack * 0.5;
    }

    // ���@�Ɓ@�ˌ�
    else if (player == 1 && enemy == 0) {
      P_damage = P_attack * 1;
      E_damage = 0;
    }

    // ���@�Ɓ@���
    else if (player == 1 && enemy == 1) {
      P_damage = 0;
      E_damage = 0;
    }

    // ���@�Ɓ@���
    else if (player == 1 && enemy == 2) {
      P_damage = 0;
      E_damage = E_attack * 1;
    }

    // ��݁@�Ɓ@�ˌ�
    else if (player == 2 && enemy == 0) {
      P_damage = P_attack * 0.5;
      E_damage = E_attack * 1.5;
    }

    // ��݁@�Ɓ@���
    else if (player == 2 && enemy == 1) {
      P_damage = P_attack * 1;
      E_damage = 0;
    }

    // ��݁@�Ɓ@���
    else if (player == 2 && enemy == 2) {
      P_damage = P_attack * 0.5;
      E_damage = E_attack * 0.5;
    }
  }

  // �ǌ��픻��
  if (add_attack) {
    P_damage = P_damage * 3;
    E_damage = E_damage * 2;
  }

  // �_���[�W����
  P_pow -= E_damage;
  E_pow -= P_damage;

  // ���s��̏���
  P_lastcmd = player;
}

// �퓬�^�[���I���F�_���[�W������̕��͌v�Z�ƒ���
void duel_turnEnd() {
  int damage0;

  // �v���C���[�̏���
  {
    // �^�[���J�n���ƏI�����̕��͂̍��𔻒�
    P_end = P_pow;
    if (P_start != P_end) {
      damage0 = E_damage - P.mercenary * 5;

      if (damage0 >= 0) {
        P.mercenary = 0;
        damage0 = damage0 - P.soldier * 10;

        if (damage0 >= 0) {
          P.soldier = 0;
        }
        else if (damage0 < 0) {
          damage0 *= -1;
          P.soldier = damage0 / 10;
        }
      }
      else if (damage0 < 0) {
        damage0 *= -1;
        P.mercenary = damage0 / 5;
      }
    }
  }

  // ����̏���
  {
    // �^�[���J�n���ƏI�����̕��͂̍��𔻒�
    E_end = E_pow;
    if (E_start != E_end) {
      damage0 = P_damage - E.mercenary * 5;

      if (damage0 >= 0) {
        E.mercenary = 0;
        damage0 = damage0 - E.soldier * 10;

        if (damage0 >= 0) {
          E.soldier = 0;
        }
        else if (damage0 < 0) {
          damage0 *= -1;
          E.soldier = damage0 / 10;
        }
      }
      else if (damage0 < 0) {
        damage0 *= -1;
        E.mercenary = damage0 / 5;
      }
    }
  }
}

// ����̍s���I���`�h
int duel_enemyCommand() {
  switch (stage_flag) {
    case 0:
      return 0;

    case 1:
      if (P_pow > E_pow)
        return 1;
      else
        return 0;

    case 2:
      if (P_pow < E_attack)
        return 0;
      else if (P_pow > E_pow)
        return 1;
      else
        return 2;

    case 3:
      if (P_lastcmd == 0)
        return 1;
      else if (P_lastcmd == 2)
        return 0;
      else
        return 2;

    case 4:
      if (P_lastcmd == 0) {
        if (E_powGauge > 100)
          return 0;
        return 1;
      }
      else if (P_lastcmd == 1) {
        if (E_powGauge > 100)
          return 1;
        return 2;
      }
      else if (P_lastcmd == 2) {
        if (E_powGauge > 100)
          return 2;
        return 0;
      }
      else
        return 0;

    default:
      srand((unsigned)time(NULL));
      int r = rand() % 3;

      return r;
  }
}

// �퓬���̃��b�Z�[�W�ƃA�j���[�V�����\��
void duel_anime(AppEnv& app_env, short player, short enemy) {
  // �w�i�A�e�L�X�g�A�L�����N�^�[�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture battle("res/png/Me5_Battle.png");
  Texture chara6("res/png/06_Rival_C.png");

  // �A�j���[�V�����J�E���^
  short animetime = 0;

  // �e�L�X�g�A�L�����N�^�[�\���p
  short i, j;
  float x, y, x0, y0, x1, y1 = 0;

  // �퓬�A�j���[�V�����̏���
  while (1) {
    if (!app_env.isOpen()) return;
    
    // �A�j���[�V�����J�n
    if (animetime < 300) animetime += 1;

    // �A�j���I����A�N���b�N����ƃ��[�v�I��
    if (animetime > 110) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        break;
      }
    }
    else {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        animetime = 110;
      }
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

    // �s���̕\��
    {
      // �v���C���[�̍s��
      {
        drawTextureBox(-telop_W / 2, (-Window::HEIGHT / 2) + (telop_H * 5), telop_W, telop_H,
        0, telop_H * (13 + player), telop_W, telop_H,
        battle, Color(0, 0, 0));

        // �_���[�W�̕\��
        drawTextureBox((-telop_W / 2) + number_W * 6, (-Window::HEIGHT / 2) + (telop_H * 4), telop_W, telop_H,
          0, telop_H * 20, telop_W, telop_H,
          battle, Color(0, 0, 0));

        numberGet(P_damage);
        for (i = 0; i < 6; i += 1) {
          if (i < skip) {
            drawTextureBox((-telop_W / 2) + number_W * (i + 1), (-Window::HEIGHT / 2) + telop_H * 4, number_W, telop_H,
              number_W * val[i], telop_H * 19, number_W, telop_H,
              battle, Color(0, 0, 0));
          }
          else {
            drawTextureBox((-telop_W / 2) + number_W * (i + 1), (-Window::HEIGHT / 2) + telop_H * 4, number_W, telop_H,
              number_W * val[i], telop_H * 22, number_W, telop_H,
              battle, Color(0, 0, 0));
          }
        }
      }

      // ����̍s��
      {
        drawTextureBox(-telop_W / 2, (-Window::HEIGHT / 2) + (telop_H * 3), telop_W, telop_H,
          0, telop_H * (16 + enemy), telop_W, telop_H,
          battle, Color(0, 0, 0));

        // �_���[�W�̕\��
        drawTextureBox((-telop_W / 2) + number_W * 6, (-Window::HEIGHT / 2) + (telop_H * 2), telop_W, telop_H,
          0, telop_H * 21, telop_W, telop_H,
          battle, Color(0, 0, 0));

        numberGet(E_damage);
        for (i = 0; i < 6; i += 1) {
          if (i < skip) {
            drawTextureBox((-telop_W / 2) + number_W * (i + 1), (-Window::HEIGHT / 2) + telop_H * 2, number_W, telop_H,
              number_W * val[i], telop_H * 19, number_W, telop_H,
              battle, Color(0, 0, 0));
          }
          else {
            drawTextureBox((-telop_W / 2) + number_W * (i + 1), (-Window::HEIGHT / 2) + telop_H * 2, number_W, telop_H,
              number_W * val[i], telop_H * 22, number_W, telop_H,
              battle, Color(0, 0, 0));
          }
        }
      }
    }

    // �L�����N�^�[�̃A�j���[�V����
    {
      // �v���C���[��
      for (i = 0; i < 4; i += 1) {
        for (j = 0; j < 4; j += 1) {
          switch (player) {

              // �ˌ�
            case 0:
              if (animetime > 60 && animetime <= 75) {
                x = (-chara_W * 4) - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j) + (animetime * 8) - 480;
                y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
                x0 = x + 8 + (16 * (1 + i) * 0.25);
                y0 = y + 4 + (16 * (1 + i) * 0.1);
              }
              else if (animetime > 75 && animetime <= 105) {
                x = (-chara_W * 4) - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j) - (animetime * 4) + 420;
                y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
                x0 = x + 8 + (16 * (1 + i) * 0.25);
                y0 = y + 4 + (16 * (1 + i) * 0.1);
              }
              else {
                x = (-chara_W * 4) - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j);
                y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
                x0 = x + 8 + (16 * (1 + i) * 0.25);
                y0 = y + 4 + (16 * (1 + i) * 0.1);
              }
              break;

              // ���
            case 1:
              x = (-chara_W * 4) - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j);
              y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
              x0 = x + 6 + (16 * (1 + i) * 0.1);
              y0 = y + (16 * (1 + i) * 0.1);
              break;

              // ���
            case 2:
              x = (-chara_W * 4) - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j);
              y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
              if (animetime > 60 && animetime <= 85) {
                x0 = x + (10 * (1 + i) * 0.125);
                y0 = y + 8 + (16 * (1 + i) * 0.2);
                x1 = x0 + (animetime * 6) - 360;
              }
              else if (animetime > 85) {
                x0 = x + (10 * (1 + i) * 0.125);
                y0 = y + 8 + (16 * (1 + i) * 0.2);
                x1 = x0;
                y1 = 16;
              }
              else {
                x0 = x + (10 * (1 + i) * 0.125);
                y0 = y + 8 + (16 * (1 + i) * 0.2);
                x1 = x0;
              }
              break;
          }

          // �L�����N�^�[�\���i�v���C���[���j
          drawTextureBox(x, y, chara_W, chara_H,
            0, 0, chara_W, chara_H,
            chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          switch (player) {

            // ���̕\��
            case 0:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 0), 0, 16, 16,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // ���̕\��
            case 1:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 1), 0, 16, 16,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // �|��̕\��
            case 2:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 3), 0, 16, 16,
                chara6, Color(0.5, 1, 1), -(M_PI / 4), Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              drawTextureBox(x1, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 2), y1, 16, 16,
                chara6, Color(0.5, 1, 1), -(M_PI / 4), Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;
          }
        }
      }

      // ���葤�̃A�j���[�V����
      for (i = 0; i < 4; i += 1) {
        for (j = 0; j < 4; j += 1) {
          switch (enemy) {

              // �ˌ�
            case 0:
              if (animetime > 60 && animetime <= 75) {
                x = (chara_W * 3) + ((chara_W * (1 + i * 0.25)) * j) - ((animetime * 8) - 480);
                y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
                x0 = x + 4 + (16 * (1 + i) * 0.125);
                y0 = y + 4 + (16 * (1 + i) * 0.125);
              }
              else if (animetime > 75 && animetime <= 105) {
                x = (chara_W * 3) + ((chara_W * (1 + i * 0.25)) * j) + ((animetime * 4) - 420);
                y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
                x0 = x + 4 + (16 * (1 + i) * 0.125);
                y0 = y + 4 + (16 * (1 + i) * 0.125);
              }
              else {
                x = (chara_W * 3) + ((chara_W * (1 + i * 0.25)) * j);
                y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
                x0 = x + 4 + (16 * (1 + i) * 0.125);
                y0 = y + 4 + (16 * (1 + i) * 0.125);
              }
              break;

              // ���
            case 1:
              x = (chara_W * 3) + ((chara_W * (1 + i * 0.25)) * j);
              y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
              x0 = x - (10 * (1 + i) * 0.125);
              y0 = y + (16 * (1 + i) * 0.1);
              break;

              // ���
            case 2:
              x = (chara_W * 3) + ((chara_W * (1 + i * 0.25)) * j);
              y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
              if (animetime > 60 && animetime <= 85) {
                x0 = x + 20 + (16 * (1 + i) * 0.25);
                y0 = y + 8 + (16 * (1 + i) * 0.2);
                x1 = x0 - ((animetime * 6) - 360);
              }
              else if (animetime > 85) {
                x0 = x + 20 + (16 * (1 + i) * 0.25);
                y0 = y + 8 + (16 * (1 + i) * 0.2);
                x1 = x0;
                y1 = 16;
              }
              else {
                x0 = x + 20 + (16 * (1 + i) * 0.25);
                y0 = y + 8 + (16 * (1 + i) * 0.2);
                x1 = x0;
              }
              break;
          }

          // �L�����N�^�[�\���i���葤�j
          drawTextureBox(x, y, chara_W, chara_H,
            0, 0, chara_W, chara_H,
            chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          switch (enemy) {

              // ���̕\��
            case 0:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 0), 0, 16, 16,
                chara6, Color(1, 0.5, 0.5), M_PI / 2, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // ���̕\��
            case 1:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 1), 0, 16, 16,
                chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // �|��̕\��
            case 2:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 3), 0, 16, 16,
                chara6, Color(1, 0.5, 0.5), (M_PI / 4) * 3, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              drawTextureBox(x1, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 2), y1, 16, 16,
                chara6, Color(1, 0.5, 0.5), (M_PI / 4) * 3, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;
          }
        }
      }
    }

    app_env.update();
  }
}

// ���̓Q�[�W�Ƌ`�E���Q�[�W�̌v�Z
void setPowGauge() {
  // ���̓Q�[�W
  if (P_pow != 0) {
    P_powGauge = (P_pow * 100) / P_pow0;
  }
  else {
    P_powGauge = 0;
  }
  if (E_pow != 0) {
    E_powGauge = (E_pow * 100) / E_pow0;
  }
  else {
    E_powGauge = 0;
  }

  // �`�E���Q�[�W
  if (P.mercenary != 0) {
    P_Mgauge = ((P.mercenary * 100) / P_M0) / 2;
  }
  else {
    P_Mgauge = 0;
  }
  if (E.mercenary != 0) {
    E_Mgauge = ((E.mercenary * 100) / E_M0) / 2;
  }
  else {
    E_Mgauge = 0;
  }
}

// ���̓Q�[�W�̃J���[�̕ύX
Color setGaugeColor(float gauge) {
  if (gauge == 100) {
    return Color(0.5, 0.5, 1);
  }
  else if (gauge < 100 && gauge >= 50) {
    return Color(0.5, 1, 0.5);
  }
  else if (gauge < 50 && gauge >= 25) {
    return Color(1, 1, 0.5);
  }
  else {
    return Color(1, 0.5, 0.5);
  }
}

// �ʏ�퓬�a�f�l�̐ݒ�
Media setBGM_a(short flag) {
  switch (flag) {
    case 0:
      return Media("res/wav/Battle_Ep1_A.wav");

    case 1:
      return Media("res/wav/Battle_Ep1_A.wav");

    case 2:
      return Media("res/wav/Battle_Ep2_A.wav");

    case 3:
      return Media("res/wav/Battle_Ep3_A.wav");

    case 4:
      return Media("res/wav/Battle_Ep4_A.wav");

    default:
      return Media("res/wav/Battle_Ep5_A.wav");
  }
}

// �ǌ���a�f�l�̐ݒ�
Media setBGM_b(short flag) {
  switch (flag) {
    case 0:
      return Media("res/wav/Battle_Ep2_B.wav");

    case 1:
      return Media("res/wav/Battle_Ep2_B.wav");

    case 2:
      return Media("res/wav/Battle_Ep2_B.wav");

    case 3:
      return Media("res/wav/Battle_Ep3_B.wav");

    case 4:
      return Media("res/wav/Battle_Ep4_B.wav");

    default:
      return Media("res/wav/Battle_Ep4_B.wav");
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  �V�~�����[�V�����p�[�g�֌W
  bool on_button(Vec2f, float, float, short, short);

  Color setButtonColor(bool);
  Color setTimeColor(float);

  Media setBGM_Main(short);
 */

// �}�E�X�J�[�\���̍��W����
bool on_button(Vec2f pos, float x, float y, short tex_width, short tex_height) {
  if (x < pos.x() && pos.x() < x + tex_width &&
      y < pos.y() && pos.y() < y + tex_height) {
    return true;
  }
  return false;
}

// �{�^���J���[�̕ύX
Color setButtonColor(bool on_mouse) {
  if (on_mouse) {
    return Color(1, 0.8, 0.4);
  }
  return Color(1, 1, 1);
}

// �^�C���o�[�̃J���[�ύX
Color setTimeColor(float gauge) {
  if (gauge >= 80) {
    return Color(0.5, 1, 0.5);
  }
  else if (gauge < 80 && gauge >= 40) {
    return Color(1, 0.8, 0.4);
  }
  else {
    return Color(1, 0.5, 0.5);
  }
}

// �������[�h�a�f�l�̐ݒ�
Media setBGM_Main(short flag) {
  switch (flag) {
    case 0:
      return Media("res/wav/Event_tutorial.wav");

    case 1:
      return Media("res/wav/Stage_1.wav");

    case 2:
      return Media("res/wav/Stage_2.wav");

    case 3:
      return Media("res/wav/Stage_3.wav");

    case 4:
      return Media("res/wav/Stage_4.wav");

    default:
      return Media("res/wav/Stage_4.wav");
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
�C�x���g�����i�e�L�X�g�\���ƃA�j���[�V�����n�j
void _Opening(AppEnv&)
void _prologue_event(AppEnv&);
void _tutorial(AppEnv&);
void _1ststage_event(AppEnv&);
void _2ndstage_event(AppEnv&);
void _3rdstage_event(AppEnv&);
void _********_spy_act(AppEnv&);
void _********_clear(AppEnv&);
void _4thstage_event(AppEnv&);
void _********_clear(AppEnv&);
void _5thstage_event(AppEnv&);
void _********_clear(AppEnv&);
void _Ending(AppEnv&)
*/

// �I�[�v�j���O
void _Opening(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture title_OP("res/png/Sp4_Opening.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T01_Opening.png");

  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^

  while (animetime < (Window::HEIGHT) * 2) {
    if (!app_env.isOpen()) return;

    // �e���b�v�̃X�N���[��
    animetime += 1;

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      animetime = (Window::HEIGHT) * 2;
    }

    // �A�j���̉���
    if (app_env.isPressKey('S')) {
      animetime += 2;
    }

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      title_OP, Color(0.5, 0.5, 0.5));

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), animetime - (Window::HEIGHT * 1.5) - telop_H, telop_W, Window::HEIGHT,
      0, 0, telop_W, Window::HEIGHT,
      telop, Color(1, 1, 1));

    app_env.update();
  }
}

// �v�����[�O�E�C�x���g
void _Prologue_event(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");
  Texture event2("res/png/BG3_Castle_out.png");
  Texture blackBG("res/png/BG5_Black.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara5("res/png/05_Rival_B.png");
  Texture chara6("res/png/06_Rival_C.png");

  // �e���b�v�̓ǂݍ���
  Texture telopA("res/png/T02_Prologue_a.png");
  Texture telopB("res/png/T02_Prologue_b.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  // �r�d�̓ǂݍ��݂Ɛݒ�
  Media rush[] = {
    Media("res/wav/EventSE_rush1.wav"),
    Media("res/wav/EventSE_rush2.wav"),
    Media("res/wav/EventSE_rush3.wav"),
    Media("res/wav/EventSE_rush4.wav")
  };

  rush[0].gain(0.2);
  rush[1].gain(0.2);
  rush[2].gain(0.2);
  rush[3].gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^
  short r = 0;             // �r�d�Đ��p����
  short r0, r1, r2;        // �L�����N�^�[�\���p

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    2, 2, 2, 3, 2, 4, 2, 2, 3, 2,
    2, 3, 1, 2, 2, 2, 2, 1, 3, 3,
    3, 4, 1, 3, 3, 3, 4, 2, 3, 2,
    3, 2, 4, 4, 3, 1,
    2, 3, 3, 3, 1, 4, 3, 4, 3, 3
  };

  // �L�����N�^�[�\���ʒu�𗐐��Ō���
  srand((unsigned)time(NULL));
  r0 = 0 - rand() % chara_H;
  r1 = r0 - rand() % chara_H;
  r2 = r1 - rand() % chara_H;

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;

      // �e�L�X�g�摜 A ���I��
      if (i == 36) {
        telop_pos = 0;
      }

      // ��ʐ؂�ւ�
      if (i == 6) {
        _BG_fade(app_env, event1, 1.0, 0, 0.02);
      }
      else if (i == 11) {
        _BG_fade(app_env, event1, 0, 1.0, -0.02);
      }
      else if (i == 17) {
        _BG_slide(app_env, event1, blackBG, Color(1, 1, 1), Color(1, 1, 1));
      }
      else if (i == 18) {
        _BG_slide(app_env, blackBG, event2, Color(1, 1, 1), Color(1, 1, 1));
      }
      else if (i == 36) {
        _BG_fade(app_env, event2, 1.0, 0, 0.02);
        _BG_slide(app_env, event2, mainBG, Color(0, 0, 0), Color(1, 1, 1));
      }
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    {
      // �Z�s�A�F��ڎw�������ǃ_�����������ۂ��w�i
      if (i == 3) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 0.6, 0.2));
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
          0, 365, Window::WIDTH, Window::HEIGHT - 365,
          event1, Color(1, 1, 1));
      }

      // ��F����
      else if (i < 6) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 1, 1));
      }

      // ��
      else if (i < 11 || i == 17) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          blackBG, Color(1, 1, 1));
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
          0, 365, Window::WIDTH, Window::HEIGHT - 365,
          event1, Color(1, 1, 1));
      }

      // ��F����
      else if (i < 17) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 1, 1));
      }

      // ��F���O
      else if (i < 36) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event2, Color(1, 1, 1));
      }

      // ����
      else if (i < 46) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));
      }
    }

    // �L�����N�^�[�̃A�j��
    {
      // ���m��������
      if (i == 3) {
        // �A�j���[�V�����̊Ǘ�
        if (animetime < Window::WIDTH) {
          animetime += 2;
        }
        else {
          animetime = 0;
          r0 = 0 - rand() % (chara_H * 2);
          r1 = r0 - rand() % (chara_H * 2);
          r2 = r1 - rand() % (chara_H * 2);
        }

        // ���m�P
        drawTextureBox((Window::WIDTH / 2) + (-r0 * 3) - animetime * 1.25, r0, (chara_W * 3), (chara_H * 3),
          0, 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));

        // ���m�Q
        drawTextureBox((Window::WIDTH / 2) + (-r1 * 3) - animetime * 1.5, r1, (chara_W * 3), (chara_H * 3),
          0, 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));

        // ���m�R
        drawTextureBox((Window::WIDTH / 2) + (-r2 * 3) - animetime * 1.75, r2, (chara_W * 3), (chara_H * 3),
          0, 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
      }

      // �����o��
      else if (i > 6 && i < 11) {
        // �o��V�[��
        if (i == 7) {
          if (animetime < chara_W * 3) {
            animetime += 1;
          }
          drawTextureBox((Window::WIDTH / 2) - (animetime * 6), (-Window::HEIGHT / 2), (chara_W * 10), (chara_H * 10),
            0, 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
            0, 365, Window::WIDTH, Window::HEIGHT - 365,
            event1, Color(1, 1, 1));
        }
        // �o���
        else {
          drawTextureBox((Window::WIDTH / 2) - (chara_W * 18), (-Window::HEIGHT / 2), (chara_W * 10), (chara_H * 10),
            0, 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
            0, 365, Window::WIDTH, Window::HEIGHT - 365,
            event1, Color(1, 1, 1));
        }
      }

      // ���q��яo��
      else if (i == 11 || i == 12) {
        // ���q
        {
          if (i == 12) {
            if (animetime < 60) {
              animetime += 1;
            }

            // ���q�W�����v
            if (animetime < 10) {
              drawTextureBox((-chara_W * 6), (-chara_H) + (animetime * 2), (chara_W * 3), (chara_H * 3),
                0, 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            else if (animetime < 20) {
              drawTextureBox((-chara_W * 6), (-chara_H) - (animetime * 2) + 40, (chara_W * 3), (chara_H * 3),
                0, 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // �҃_�b�V��
            else {
              drawTextureBox((-chara_W * 6) - (animetime * 8) + 160, (-chara_H), (chara_W * 3), (chara_H * 3),
                0, 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          else {
            drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
        // ����
        {
          drawTextureBox((-chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
        }
      }

      // �����Ƃ茾
      else if (i > 11 && i < 17) {
        // �P�l
        if (i < 15) {
          drawTextureBox((-chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
        }
        // ���m�o��
        else {
          // �͂܂��
          if (i == 15) {
            if (animetime < chara_W * 1) {
              animetime += 1;
            }
            // ���m
            drawTextureBox((-chara_W * 3) + (animetime * 10) - 200, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            drawTextureBox((chara_W * 1) - (animetime * 10) + 200, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            // ����
            drawTextureBox((-chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // ��ʊO�Ɍ������Ȃ���A�t���{�b�R
          else {
            if (animetime < chara_W * 5) {
              animetime += 1;
            }
            // ���m
            drawTextureBox((-chara_W * 3) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            drawTextureBox((chara_W * 1) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            // ����
            drawTextureBox((-chara_W * 1) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));

            // ���ʉ��Đ�
            {
              srand((unsigned)time(NULL));
              if (!rush[r].isPlaying()) {
                r = rand() % 4;
                rush[r].play();
              }
            }
          }
        }
      }

      // ���q�t�����
      else if (i > 17 && i < 22) {
        // �t����@�Ȃ�
        if (i < 21) {
          // ���q�j���P��
          if (i < 19) {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 3, 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // ���q�V���b�N
          else {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              0, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // �Ђ�
          drawTextureBox((chara_W * 7), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara3, Color(1, 1, 1));
        }
        // �Ђ߈ړ�
        else {
          if (animetime < chara_W * 3) {
            animetime += 1;
          }
          // ���q
          drawTextureBox((chara_W * 2) + (animetime), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, chara_H * 1, chara_W, chara_H,
            chara1, Color(1, 1, 1));
          // �Ђ�
          drawTextureBox((chara_W * 7) + (animetime * 3), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara3, Color(1, 1, 1));
        }
      }

      // ���q�V���b�N
      else if (i >= 22 && i < 32) {
        // ��������@�Ȃ�
        if (i == 22) {
          drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, chara_H * 1, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        // �����A���m�o��
        else {
          // �ړ�
          if (i == 23) {
            if (animetime < chara_W * 2) {
              animetime += 1;
            }
            // ���q
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
            // ����
            drawTextureBox((chara_W * 2) + (animetime * 10) - 400, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));
            // ���m
            drawTextureBox((-chara_W * 2) + (animetime * 8) - 320, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, chara_H * 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
          }
          // �ړ���
          else {
            // ���q
            {
              // ���q�@�V���b�N
              if (i < 29) {
                drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 0, chara_H * 1, chara_W, chara_H,
                  chara1, Color(1, 1, 1));
              }
              // ���q�@�����
              else {
                drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 1, chara_H * 1, chara_W, chara_H,
                  chara1, Color(1, 1, 1));
              }
            }
            // ����
            {
              // �����@�����
              if (i < 27) {
                drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 1, chara_H * 1, chara_W, chara_H,
                  chara2, Color(1, 1, 1));
              }
              // �����@�V���b�N
              else if (i < 29) {
                drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 0, chara_H * 1, chara_W, chara_H,
                  chara2, Color(1, 1, 1));
              }
              // �����@�{��
              else {
                drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 2, chara_H * 0, chara_W, chara_H,
                  chara2, Color(1, 1, 1));
              }
            }
            // ���m
            {
              if (i < 31) {
                drawTextureBox((-chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  0, chara_H * 0, chara_W, chara_H,
                  chara6, Color(1, 1, 1));
              }
              // �ސE
              else {
                if (animetime < chara_W * 2) {
                  animetime += 1;
                }
                drawTextureBox((-chara_W * 2) + animetime * 8, (-chara_H), (chara_W * 3), (chara_H * 3),
                  0, chara_H * 0, chara_W, chara_H,
                  chara6, Color(1, 1, 1));
              }
            }
          }
        }
      }

      // �����ʂƃf�u
      else if (i >= 32 && i < 36) {

        // �f�u��������
        {
          // �o��
          if (i == 32) {
            if (animetime < chara_W * 2) {
              animetime += 1;
            }
            // �f�u
            drawTextureBox((-chara_W * 2) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara5, Color(1, 1, 1));
            // ������
            drawTextureBox((-chara_W * 6) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // �ޏ�
          else if (i == 35) {
            if (animetime < chara_W * 3) {
              animetime += 1;
            }
            // �f�u
            drawTextureBox((-chara_W * 2) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara5, Color(1, 1, 1));
            // ������
            drawTextureBox((-chara_W * 6) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          else {
            // �f�u
            drawTextureBox((-chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara5, Color(1, 1, 1));
            // ������
            drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
        }
        // ���q
        {
          // �����
          if (i < 32) {
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // �����
          else {
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
        // ����
        {
          // �{��
          if (i < 32) {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 2, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // �ʏ�
          else {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
        }
      }

      // �ƂȂ��q
      else if (i >= 36) {
        // ���q
        {
          // �V���b�N
          if (i < 38) {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // �����
          else {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
        // ����
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
    }

    // �e���b�v�\��
    if (i < 36) {
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
        0, telop_pos, telop_W, telop_H * telop_line[i],
        telopA, Color(0, 0, 0));
    }
    else {
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
        0, telop_pos, telop_W, telop_H * telop_line[i],
        telopB, Color(0, 0, 0));
    }

    app_env.update();
  }
  thema.stop();
}

// �`���[�g���A��
void _tutorial(AppEnv& app_env) {
  // �w�i�ƃ{�^���̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara6("res/png/06_Rival_C.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T02_Prologue_b.png");
  Texture system("res/png/Me4_System.png");
  Texture battle("res/png/Me5_Battle.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_tutorial.wav");
  thema.looping(true);
  thema.gain(0.2);

  Media bgm_a = setBGM_a(stage_flag);
  bgm_a.looping(true);
  bgm_a.gain(0.2);

  // �r�d�̓ǂݍ��݂Ɛݒ�
  Media se_start("res/wav/se_battle.wav");
  se_start.gain(0.2);

  // �{�^���̔���A�F
  bool on_mouse;
  Color angle;

  // �{�^���A�L�����N�^�[�̕\���ʒu
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);

  short i = 0, i0 = 0;     // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^
  short n, j;              // �{�^���\���p
  float x0, y0;            // �C�x���g�����p

  short telop_pos = telop_H * 29;  // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {           // �e�L�X�g�摜�؂�o���s��
    4, 2, 1, 1, 4, 3, 4, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �}�E�X���W�̎擾
    mouse_pos = app_env.mousePosition();

    // �a�f�l�Đ�
    if (i < 5 && !thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (i > 4) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        animetime = 0;
        i += 1;

        telop_pos += telop_H * telop_line[i0];
        i0 += 1;
      }
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      thema.stop();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) {
      bgm_a.stop();
      _BG_fade(app_env, mainBG, 1.0, 0, 0.02);
      break;
    }

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �N���b�N�񐔂ɑΉ������C�x���g�̕\��
    switch (i) {

      // ���͑�����I�����Ă��炤
      case 0:
        // �e���b�v�\��
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - 1), telop_W, telop_H * 1,
          0, telop_pos, telop_W, telop_H * 1,
          telop, Color(0, 0, 0));

        // �{�^���\��
        for (n = 0; n < 6; n += 1) {
          if (n != 1) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * (n + 1), menu_W, menu_H);
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x, y - (menu_H + 2) * (n + 1), menu_W, menu_H,
            0, menu_H * n, menu_W, menu_H,
            button, angle);

          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              i += 1;
            }
          }
        }

        break;

        // ���m��W��I�����Ă��炤
      case 1:
        // ���͑���
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 1, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));

        // �e���b�v�\��
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        // �{�^���\��
        for (n = 0; n < 3; n += 1) {
          if (n != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            on_mouse = on_button(mouse_pos, x + (menu_W + 2), y - (menu_H + 2) * (n + 1), menu_W, menu_H);
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x + (menu_W + 2), y - (menu_H + 2) * (n + 1), menu_W, menu_H,
            0, menu_H * (9 + n), menu_W, menu_H,
            button, angle);

          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              i += 1;

              telop_pos += telop_H * telop_line[i0];
              i0 += 1;
            }
          }
        }

        break;

        // �l����I�����Ă��炤
      case 2:
        // ���͑���
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 1, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));

        // ���m��W
        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 9, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));

        // �e���b�v�\��
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        // �{�^���\��
        for (n = 0; n < 4; n += 1) {
          if (n != 0) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            on_mouse = on_button(mouse_pos, x + (menu_W + 2), y - (menu_H + 2) * (n + 1), menu_W, menu_H);
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x + (menu_W + 2), y - (menu_H + 2) * (n + 1), menu_W, menu_H,
            0, menu_H * (27 + n), menu_W, menu_H,
            button, angle);

          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              P.soldier += 10;
              i += 1;

              telop_pos += telop_H * telop_line[i0];
              i0 += 1;
            }
          }
        }

        break;

        // �N�U��I�����Ă��炤
      case 3:
        // �e���b�v�\��
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        // �{�^���\��
        for (n = 0; n < 6; n += 1) {
          if (n != 4) {
            angle = Color(0.5, 0.5, 0.5);
          }
          else {
            on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * (n + 1), menu_W, menu_H);
            angle = setButtonColor(on_mouse);
          }
          drawTextureBox(x, y - (menu_H + 2) * (n + 1), menu_W, menu_H,
            0, menu_H * n, menu_W, menu_H,
            button, angle);

          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              i += 1;

              telop_pos += telop_H * telop_line[i0];
              i0 += 1;
            }
          }
        }

        break;

        // ����I�����Ă��炤
      case 4:
        thema.stop();

        // �퓬�O�̏���
        while (animetime < 90) {
          if (!app_env.isOpen()) return;

          // �퓬�J�n�r�d�Đ�
          if (animetime == 0) se_start.play();

          app_env.setupDraw();

          // �w�i�\��
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            mainBG, Color(1, 1, 1));
          // �퓬�J�n
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
            0, telop_pos, telop_W, telop_H * telop_line[i0],
            telop, Color(0, 0, 0));

          app_env.update();

          animetime += 1;
        }

        // �{�^���̕\���ʒu�w��
        {
          x = (-Window::WIDTH / 2) + 2;
          y = (Window::HEIGHT / 2) - 358;
        }

        // �A�j���[�V�����J�E���^�̃��Z�b�g
        animetime = 0;

        // �e�L�X�g�؂�o���ʒu�̍X�V
        telop_pos += telop_H * telop_line[i0];
        i0 += 1;

        // �U���͊�l�ƕ��͏����l���擾
        duel_standby();

        bgm_a.play();

        // �퓬��ʂ̕\��
        while (1) {
          if (!app_env.isOpen()) return;
          app_env.setupDraw();

          // �}�E�X���W�̎擾
          mouse_pos = app_env.mousePosition();

          // �w�i�\��
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            mainBG, Color(1, 1, 1));

          // �e���b�v�\��
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
            0, telop_pos, telop_W, telop_H * telop_line[i0],
            telop, Color(0, 0, 0));

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

          // �{�^���̕\��
          for (n = 0; n < 3; n += 1) {
            if (n != 1) {
              angle = Color(0.5, 0.5, 0.5);
            }
            else {
              on_mouse = on_button(mouse_pos, x, y + (menu_H + 2) * (2 - n), menu_W, menu_H);
              angle = setButtonColor(on_mouse);
            }
            drawTextureBox(x, y + (menu_H + 2) * (2 - n), menu_W, menu_H,
              0, menu_H * (22 + n), menu_W, menu_H,
              button, angle);

            if (on_mouse) {
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();

                // �퓬�����̕\��
                {
                  duel_turnStart();
                  duel_fight(1, 0);
                  duel_anime(app_env, 1, 0);
                }

                i += 1;

                telop_pos += telop_H * telop_line[i0];
                i0 += 1;
                break;
              }
            }
          }

          // �N���b�N�����烋�[�v�I��
          if (i > 4) {
            break;
          }

          app_env.update();
        }

        break;

        // �C�x���g�i�s
      default:
        // �e���b�v�\��
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        break;
    }

    app_env.update();
  }
  thema.stop();
}

// �P�́E�C�x���g
void _1ststage_event(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T03_1stStage.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    3, 3, 5, 2, 5, 5, 3, 4, 4, 5,
    2, 5
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      if (i == 4) {
        _BG_slide(app_env, event1, mainBG, Color(1, 1, 1), Color(1, 1, 1));
      }
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    if (i < 5) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        event1, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
    }

    // �L�����N�^�[�̃A�j��
    if (i < 5) {
      // ���q
      drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
        chara_W * 0, chara_H * 0, chara_W, chara_H,
        chara1, Color(1, 1, 1));
      // ����
      drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
        chara_W * 0, chara_H * 0, chara_W, chara_H,
        chara2, Color(1, 1, 1));
    }

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// �Q�́E�C�x���g
void _2ndstage_event(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T04_2ndStage.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^
  bool stop = false;       // �A�j���[�V�����I���܂ŃC�x���g�X�g�b�v

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    4, 3, 1, 4, 2, 4, 3, 5, 3, 3,
    3, 4, 3, 5, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (!stop) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        telop_pos += telop_H * telop_line[i];
        i += 1;
        animetime = 0;

        if (i == 12) {
          _BG_slide(app_env, event1, mainBG, Color(1, 1, 1), Color(1, 1, 1));
        }
      }
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    if (i < 12) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        event1, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
    }

    // �L�����N�^�[�̃A�j��
    if (i < 12) {

      // �]�T�̍��΂�
      if (i == 2) {
        // ���q
        drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 3, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // ����
        drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }

      // ���q�@�{��̈ꌂ
      else if (i == 4) {
        // �A�j���[�V�����J�E���^����
        if (animetime < (chara_W * 2) + 60) {
          stop = true;
          animetime += 1;
        }
        else {
          stop = false;
        }

        // ����
        {
          // ������O
          if (animetime < (chara_W * 2) + 60) {
            drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // ����ꂽ
          else {
            drawTextureBox((chara_W * 0), (-chara_H * 1), chara_W * 3, chara_H * 3,
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1), M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
        }

        // ���q
        {
          // �A�j���[�V�����J�n
          if (animetime > 60) {
            // �U�肩�Ԃ�
            if (animetime <= (chara_W * 1) + 60) {
              drawTextureBox((chara_W * 2) + (animetime - 60) * 3, (-chara_H * 1), chara_W * 3, chara_H * 3,
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // �ːi
            else if (animetime < (chara_W * 2) + 60) {
              drawTextureBox((-chara_W * 2) - (animetime - ((chara_W * 1) + 60)) * 6 + (chara_W * 6), (-chara_H * 1), chara_W * 3, chara_H * 3,
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // ��������
            else {
              drawTextureBox((-chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // �Î~���
          else {
            drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
              chara_W * 3, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
      }

      // ���q�}�E���g
      else if (i > 4) {
        // ����
        drawTextureBox((chara_W * 0), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 1, chara_W, chara_H,
          chara2, Color(1, 1, 1), M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
        // ���q
        drawTextureBox((-chara_W * 3), (-chara_H / 2), chara_W * 3, chara_H * 3,
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
      }

      // ��b
      else if (i < 4) {
        // ���q
        drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // ����
        drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
    }

    // �e���b�v�\��
    if (i != 4) {
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
        0, telop_pos, telop_W, telop_H * telop_line[i],
        telop, Color(0, 0, 0));
    }
    else {
      if (animetime < (chara_W * 2) + 60) {
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), 144, telop_H * telop_line[i],
          0, telop_pos, 144, telop_H * telop_line[i],
          telop, Color(0, 0, 0));
      }
      else {
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
          0, telop_pos, telop_W, telop_H * telop_line[i],
          telop, Color(0, 0, 0));
      }
    }

    app_env.update();
  }
  thema.stop();
}

// �R�́E�C�x���g
void _3rdstage_event(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara5("res/png/05_Rival_B.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T05_3rdStage.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    4, 2, 3, 4, 3, 3, 4, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;

      if (i == 7) {
        _BG_slide(app_env, event1, mainBG, Color(1, 1, 1), Color(1, 1, 1));
      }
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    if (i < 7) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        event1, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
    }

    // �L�����N�^�[�̃A�j��
    if (i < 7) {
      // ���q
      {
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
      }
      // ����
      {
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
      // �f�u
      {
        if (animetime < chara_W * 2) {
          animetime += 1;
        }

        // �o��
        if (i == 1) {
          drawTextureBox((-chara_W * 5) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 2, chara_H * 0, chara_W, chara_H,
            chara5, Color(1, 1, 1));
        }
        // �ޏ�
        else if (i == 4) {
          drawTextureBox((-chara_W * 5) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara5, Color(1, 1, 1));
        }
        // �Î~���
        else if (i > 0 && i < 4) {
          drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara5, Color(1, 1, 1));
        }
      }
    }

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// �R�́E�X�p�C���s
void _3rdstage_spy_act(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture mainBG("res/png/BG1_GrassField.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T05_3rdStage.png");

  short i = 0;             // �N���b�N�񐔂̃J�E���^

  short telop_pos = telop_H * 29;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    3, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
}

// �R�́E��������N���A
void _3rdstage_clear(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture event1("res/png/BG2_Castle_in.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara5("res/png/05_Rival_B.png");
  Texture chara6("res/png/06_Rival_C.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T05_3rdStage.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  // �r�d�̓ǂݍ��݂Ɛݒ�
  Media rush[] = {
    Media("res/wav/EventSE_rush1.wav"),
    Media("res/wav/EventSE_rush2.wav"),
    Media("res/wav/EventSE_rush3.wav"),
    Media("res/wav/EventSE_rush4.wav")
  };

  rush[0].gain(0.2);
  rush[1].gain(0.2);
  rush[2].gain(0.2);
  rush[3].gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^
  short r = 0;             // �r�d�Đ��p����

  short telop_pos = telop_H * 42;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    2, 5, 2
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      event1, Color(1, 1, 1));

    // �L�����N�^�[�̃A�j��
    {
      // �����`
      if (i == 2) {
        if (animetime < chara_W * 4) {
          animetime += 1;
        }

        // ���m�P
        drawTextureBox((-chara_W * 5) + animetime * 8, (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // ���m�Q
        drawTextureBox((-chara_W * 2) + animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // �f�u
        drawTextureBox((chara_W * 5) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 1, chara_H * 1, chara_W, chara_H,
          chara5, Color(1, 1, 1));

        // ���ʉ��Đ�
        {
          srand((unsigned)time(NULL));
          if (!rush[r].isPlaying()) {
            r = rand() % 4;
            rush[r].play();
          }
        }
      }

      // �f�u�ǂ��l�߂���
      else {
        // ���m�P
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // ���m�Q
        drawTextureBox((-chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // �f�u
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 1, chara_H * 1, chara_W, chara_H,
          chara5, Color(1, 1, 1));
      }
    }

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// �S�́E�C�x���g
void _4thstage_event(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture event1("res/png/BG2_Castle_in.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");
  Texture chara4("res/png/04_Rival_A.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T06_4thStage_a.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    3, 5, 3, 4, 4, 4, 2, 2, 2, 3,
    2, 3, 2, 2, 2, 2, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      event1, Color(1, 1, 1));

    // �L�����N�^�[�̃A�j��
    {
      // ���q�Ƃ���
      {
        if (i == 7 || i == 8) {
          if (animetime < chara_W * 1) {
            animetime += 1;
          }
        }

        // ���q
        {
          // �����
          if (i > 0 && i < 9) {
            // �ړ��O
            if (i < 8) {
              drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // �莆�ɋC�Â��Ĉړ�
            else {
              drawTextureBox((chara_W * 5) - animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // �ʏ�
          else if (i < 16) {
            // �ړ��O
            if (i < 8) {
              drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // �ړ���
            else {
              drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // �{��
          else {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 2, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }

        // ����
        {
          // �����
          if (i < 2) {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // �{��
          else if (i == 17) {
            drawTextureBox((-chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 2, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // �ʏ�
          else {
            // �ړ��O
            if (i < 7) {
              drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
            // �莆�ɋC�Â��Ĉړ�
            else if (i == 7) {
              drawTextureBox((chara_W * 1) - animetime * 5, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
            // �ړ���
            else {
              drawTextureBox((-chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
          }
        }
      }

      // �Ђ߂Ɛ�����
      {
        if (i == 1 || i == 6) {
          if (animetime < chara_W * 2) {
            animetime += 1;
          }
        }

        // ������
        if (i < 7) {
          // �o��
          if (i == 1) {
            drawTextureBox((-chara_W * 5) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // �ޏ�
          else if (i == 6) {
            drawTextureBox((-chara_W * 5) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // �Î~���
          else if (i > 0) {
            drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
        }
        // �Ђ�
        if (i < 7) {
          // �o��
          if (i == 1) {
            drawTextureBox((-chara_W * 7) + animetime * 4 - 160, (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // �ޏ�
          else if (i == 6) {
            drawTextureBox((-chara_W * 7) - animetime * 5, (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // �Î~���
          else if (i > 0) {
            drawTextureBox((-chara_W * 7), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
        }
      }
    }

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// �S�́E��������N���A
void _4thstage_clear(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture event1("res/png/BG2_Castle_in.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara6("res/png/06_Rival_C.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T06_4thStage_b.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    2, 4, 4, 4, 4, 5, 2
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      event1, Color(1, 1, 1));

    // �L�����N�^�[�̃A�j��
    {
      // ������
      {
        // �ʏ�
        if (i < 3) {
          drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
        // �ł�
        else if (i < 5) {
          drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 1, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
        // �V���b�N
        else {
          drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
      }
      // ���m
      {
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 1, chara_H * 1, chara_W, chara_H,
          chara6, Color(1, 1, 1));
      }
    }

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// �T�́E�C�x���g
void _5thstage_event(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture event1("res/png/BG2_Castle_in.png");
  Texture event2("res/png/BG3_Castle_out.png");
  Texture event3("res/png/BG4_Shrine.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara5("res/png/05_Rival_B.png");
  Texture chara7("res/png/07_Unknown.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T07_5thStage_a.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    5, 4, 4, 2, 3, 4, 4, 4, 4, 3,
    4, 4, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;

      // ��ʐ؂�ւ�
      if (i == 2) {
        _BG_slide(app_env, event1, event2, Color(1, 1, 1), Color(1, 1, 1));
      }
      else if (i == 8) {
        _BG_slide(app_env, event2, event3, Color(1, 1, 1), Color(1, 1, 1));
      }
      else if (i == 11) {
        _BG_slide(app_env, event3, event2, Color(1, 1, 1), Color(1, 1, 1));
      }
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    // �w�i�\��
    {
      // ��F����
      if (i < 2) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 1, 1));
      }
      // ��F���O
      else if (i < 8 || i >= 11) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event2, Color(1, 1, 1));
      }
      // ���
      else if (i < 11) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event3, Color(1, 1, 1));
      }
    }

    // �L�����N�^�[�̃A�j��
    {
      // ��F����
      if (i < 2) {
        // ���q
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // ����
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }

      // �ЂߗU��
      else if (i < 8) {
        // �A�j���[�V�����Ǘ�
        if (i == 5) {
          if (animetime < chara_W * 2) {
            animetime += 1;
          }
        }

        // ���q
        {
          // �Ђߔ�ԑO
          if (i < 6) {
            if (i < 5) {
              drawTextureBox((chara_W * 7), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // �Ђߔ��
            else {
              drawTextureBox((chara_W * 7) - animetime, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // �Ђߔ��ł�������
          else {
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
        // ����
        {
          // �Ђߔ�ԑO
          if (i < 6) {
            if (i < 5) {
              drawTextureBox((chara_W * 3), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
            // �Ђߔ��
            else {
              drawTextureBox((chara_W * 3) - animetime, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
          }
          // �Ђߔ��ł�������
          else {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
        }
        // ������
        {
          // �o��
          if (i == 6) {
            if (animetime < chara_W) {
              animetime += 1;
            }
            drawTextureBox((-chara_W * 6) + animetime * 8 - 160, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // �Î~���
          else if (i > 5) {
            drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
        }
        // �{�X
        {
          if (i < 5) {
            drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara7, Color(1, 1, 1));
          }
          // �ЂߗU��
          else if (i == 5) {
            drawTextureBox((-chara_W * 5), (-chara_H) + animetime * 8, (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara7, Color(1, 1, 1));
          }
        }
        // �Ђ�
        {
          if (i < 5) {
            drawTextureBox((-chara_W * 6), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 4, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // �U�������
          else if (i == 5) {
            drawTextureBox((-chara_W * 6), (-chara_H * 2) + animetime * 8, (chara_W * 3), (chara_H * 3),
              chara_W * 4, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
        }
      }

      // �ڂ����f�u
      else if (i < 11) {
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara5, Color(1, 1, 1));
      }

      // ���J�n
      else {
        // ���q
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // ����
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
        // ������
        drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara4, Color(1, 1, 1));
      }
    }

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// �T�́E�N���A
void _5thstage_clear(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture event3("res/png/BG4_Shrine.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara7("res/png/07_Unknown.png");

  // �e���b�v�̓ǂݍ���
  Texture telop("res/png/T07_5thStage_b.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media bgm("res/wav/Battle_EP5_A.wav");
  bgm.gain(0.2);

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  short animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    4, 4, 3, 1, 2, 4, 2, 2, 1, 2
  };

  // �a�f�l�Đ�
  bgm.play();

  while (1) {
    if (!app_env.isOpen()) return;

    // ���N���b�N�ŃC�x���g�i�s
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) {
      bgm.stop();
      break;
    }

    app_env.setupDraw();

    // �w�i�\��
    if (i < 8) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        event3, Color(0.5, 0.5, 0.5));
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
        0, 365, Window::WIDTH, Window::HEIGHT - 365,
        event3, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        event3, Color(1, 1, 1));
    }

    // �L�����N�^�[�̃A�j��
    {
      // ��
      if (i > 1 && i < 8) {
        drawFillBox(-10, 0, 20, (Window::HEIGHT / 2), Color(1, 0.8, 0.4, 0.5));
      }

      if (i > 4 && i < 8) {
        drawFillBox(0, 0, 120, 20, Color(1, 0.8, 0.4, 0.5), M_PI / 18, Vec2f(1, 1), Vec2f(0, 0));
      }

      // ���q
      {
        // ���q�@�M���O
        if (i < 4) {
          drawTextureBox((-chara_W * 7), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        // ���q�@�M����
        else if (i < 8) {
          drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        // ���q�@�h����
        else {
          drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 3, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
      }

      // ����
      {
        // ���q�@�M���O
        if (i < 4) {
          drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
        }
        // ���q�ɂԂ���΂����
        else {
          // �|�꒼��
          if (i == 4) {
            drawTextureBox((-chara_W * 3.5), (chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1), -M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
          // �_�E��
          else {
            drawTextureBox((-chara_W * 3.5), (chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1), -M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
        }
      }

      // ������
      {
        // �����
        if (i < 5) {
          drawTextureBox((-chara_W * 9), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
            chara_W * 1, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
        // �V���b�N
        else {
          drawTextureBox((-chara_W * 9), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
      }

      // �{�X
      {
        // �����
        if (i < 5) {
          drawTextureBox((chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 1, chara_H * 1, chara_W, chara_H,
            chara7, Color(1, 1, 1));
        }
        // ���ɂ����
        else {
          // ����
          if (i < 8) {
            drawTextureBox((chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara7, Color(1, 1, 1));
          }
          // �_�E��
          else {
            drawTextureBox((chara_W * 4), (chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara7, Color(1, 1, 1), -M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
        }
      }
    }

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
}

// �G���f�B���O
void _Ending(AppEnv& app_env) {
  // �w�i�̓ǂݍ���
  Texture event2("res/png/BG3_Castle_out.png");
  Texture blackBG("res/png/BG5_Black.png");
  Texture Ending("res/png/Sp5_Ending.png");

  // �L�����N�^�[�̓ǂݍ���
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");

  // �a�f�l�̓ǂݍ��݂Ɛݒ�
  Media thema("res/wav/Event_Ending.wav");
  thema.looping(true);
  thema.gain(0.2);

  // �e���b�v�̓ǂݍ���
  Texture telopA("res/png/T08_Ending_a.png");
  Texture telopB("res/png/T08_Ending_b.png");

  short i = 0;             // �N���b�N�񐔂̃J�E���^
  float animetime = 0;     // �A�j���[�V�����̃^�C���J�E���^
  bool stop = false;       // �v���C���[�̑���֎~�t���O

  short telop_pos = 0;         // �e�L�X�g�摜�؂�o���J�n�ʒu
  short telop_line[] = {       // �e�L�X�g�摜�؂�o���s��
    4, 3, 3, 3, 2, 1, 3, 4, 2
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // �a�f�l�Đ�
    if (!thema.isPlaying()) {
      thema.play();
    }

    // ���N���b�N�ŃC�x���g�i�s
    if (!stop) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        telop_pos += telop_H * telop_line[i];
        i += 1;
        animetime = 0;

        if (i == 6) {
          stop = true;
        }
      }
    }

    // �X�L�b�v
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // �e�L�X�g���Ō�܂ŕ\�������烋�[�v�I��
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // �w�i�\��
    if (i < 7) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        event2, Color(1, 1, 1));
    }
    else if (i == 7) {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        blackBG, Color(1, 1, 1));
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
        0, 365, Window::WIDTH, Window::HEIGHT - 365,
        event2, Color(1, 1, 1));
    }
    else {
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        Ending, Color(1, 1, 1));
      drawTextureBox((-Window::WIDTH / 2), (Window::HEIGHT / 2) - (Window::HEIGHT - 365), Window::WIDTH, Window::HEIGHT - 365,
        0, 365, Window::WIDTH, Window::HEIGHT - 365,
        event2, Color(1, 1, 1));
    }

    // �L�����N�^�[�̃A�j��
    if (i < 7) {
      if (i == 5) {
        if (animetime < 360) {
          animetime += 0.05;
        }
        else {
          animetime = 0;
        }
      }
      else if (i == 6) {
        if (animetime < chara_W * 2) {
          animetime += 1;
        }
        else {
          stop = false;
          telop_pos += telop_H * telop_line[i];
          i += 1;
          animetime = 0;
        }
      }

      // ���q
      {
        if (i < 5) {
          drawTextureBox((chara_W * 0), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 3, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        else if (i < 7) {
          // �x�肾��
          if (i == 5) {
            drawTextureBox((chara_W * 0) + std::cos(animetime) * 20, (-chara_H) + std::sin(animetime) * 10, (chara_W * 3), (chara_H * 3),
              chara_W * 3, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // �����Ɍ���
          else {
            if (animetime < chara_W) {
              drawTextureBox((chara_W * 0) + animetime * 1, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            else {
              drawTextureBox((chara_W * 0) - animetime * 4 + 80, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
        }
      }
      // ����
      {
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
      // �Ђ�
      {
        if (i < 5) {
          drawTextureBox((chara_W * 3), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 3, chara_H * 0, chara_W, chara_H,
            chara3, Color(1, 1, 1));
        }
        else if (i < 7) {
          // �x�肾��
          if (i == 5) {
            drawTextureBox((chara_W * 3) + std::cos(animetime) * 20, (-chara_H) + std::sin(animetime) * 10, (chara_W * 3), (chara_H * 3),
              chara_W * 3, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // �����Ɍ���
          else {
            if (animetime < chara_W) {
              drawTextureBox((chara_W * 3) + animetime * 1, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara3, Color(1, 1, 1));
            }
            else {
              drawTextureBox((chara_W * 3) - animetime * 4 + 80, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara3, Color(1, 1, 1));
            }
          }
        }
      }
    }

    // �e���b�v�\��
    if (i < 8) {
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
        0, telop_pos, telop_W, telop_H * telop_line[i],
        telopA, Color(0, 0, 0));
    }
    else {
      drawTextureBox((-telop_W / 2), (Window::HEIGHT / 2) - telop_H * (1 + telop_line[i]), telop_W, telop_H * telop_line[i],
        0, telop_pos, telop_W, telop_H * telop_line[i],
        telopA, Color(0, 0, 0));
    }

    app_env.update();
  }

  _BG_fade(app_env, Ending, 1.0, 0.5, 0.02);

  // �X�N���[��
  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // �e���b�v�̃X�N���[��
    if (animetime < (Window::HEIGHT * 1) + telop_H) {
      animetime += 0.5;
    }

    // �w�i�\��
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      Ending, Color(0.5, 0.5, 0.5));

    // �e���b�v�\��
    drawTextureBox((-telop_W / 2), animetime - ((Window::HEIGHT * 1.5) + telop_H), telop_W, Window::HEIGHT,
      0, 0, telop_W, Window::HEIGHT,
      telopB, Color(1, 1, 1));

    app_env.update();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////

