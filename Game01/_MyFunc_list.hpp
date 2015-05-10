//
// Revival of Kingdom の外部処理関数
//

#pragma once
#include "appEnv.hpp"
#include "_MyVar_list.hpp"    // 変数リスト

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  目次
  ・エフェクト処理
  ・共通処理関係
  ・戦闘処理関係
  ・シミュレーションパート関係
  ・イベント処理（テキスト表示とアニメーション系）
 */


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
エフェクト処理
void _BG_slide(AppEnv&, Texture&, Texture&);
void _BG_fade(AppEnv&, Texture&, float, float, float);
void _titleCall(AppEnv&, short);
*/

// 背景画像の切り替え
void _BG_slide(AppEnv& app_env, Texture& tex_before, Texture& tex_after, Color& before, Color& after) {
  float x = Window::WIDTH;
  while (x > 0) {
    if (!app_env.isOpen()) return;

    // クリックしたらアニメ終了
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      x = 0;
    }

    app_env.setupDraw();

    // 切り替え前
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      tex_before, before);

    // 切り替え後の背景
    drawTextureBox(x - (Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH - x, Window::HEIGHT,
      x, 0, Window::WIDTH - x, Window::HEIGHT,
      tex_after, after);

    // 切り替えスピード
    x -= 4;

    app_env.update();
  }
}

// 背景画像のフェードアウト、フェードイン
// angle を 0 未満の数値にするとフェードイン
void _BG_fade(AppEnv& app_env, Texture& tex, float base, float limit, float angle) {
  float fade = base;

  while (1) {
    if (!app_env.isOpen()) return;

    // ループ終了判定
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

    // クリックしたらアニメ終了
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      fade = limit;
    }

    app_env.setupDraw();

    // 背景
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      tex, Color(fade, fade, fade));

    fade -= angle;

    app_env.update();
  }
}

// 章開始時、クリア時のタイトル表示
void _titleCall(AppEnv& app_env) {
  // 画像の読み込み
  Texture titleEP("res/png/Sp6_BG_Title.png");
  Texture telopEP("res/png/Sp7_BG_Telop.png");

  // クリア時の効果音
  Media clearSE("res/wav/se_clear.wav");
  clearSE.gain(0.2);

  // タイトル用テキスト画像のサイズ
  float title_w = 352, title_h = 28;

  // アニメーション用
  short time = 0;
  float y = 0;
  float y0 = title_h * (stage_flag * 2);

  // ステージクリア時に効果音を鳴らす
  if (stage_clear) {
    clearSE.play();
  }

  while (1) {
    if (!app_env.isOpen()) return;

    // クリックしたらアニメ終了
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      break;
    }

    // アニメーション処理
    if (time < 1000) {
      y += 0.5;
      time += 1;
    }

    app_env.setupDraw();

    // 背景
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      titleEP, Color(1, 1, 1));

    // 章の番号
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

    // 章のタイトル
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

    // ステージクリア
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
共通処理関係
void numberGet()
void enemy_reaction()
*/

// パラメータの桁情報を取得
void numberGet(int val0) {
  short i;

  // 桁情報を初期化
  for (i = 0; i < 6; i += 1) {
    val[i] = 0;
  }
  skip = 0;

  // それぞれの桁の数値を取得
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

  // 上位の桁が０のみならスキップ
  i += 1;
  skip = i;
}

// プレイヤーの行動に対する相手の反応
void enemy_reaction() {
  short sabo_flag = 1;
  short val;

  // 妨害工作が実行中なら全ての効果を半減
  if (sabo > 0) {
    sabo_flag = 2;
    sabo -= 1;
  }

  // 収入を得る
  E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;

  // 兵力を増強する
  switch (stage_flag) {
    case 0:
      break;

      // １章
      // 兵士の増強のみ
    case 1:
      // 兵士
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

      // ２章
      // 兵士と義勇兵の増強
    case 2:
      // 兵士
      if (E.soldier < 1500) {
        val = 150 / sabo_flag;

        if (E.money < val * 10) {
          val = E.money / 10;
        }

        E.money -= val * 10;
        E.soldier += val;
      }

      // 義勇兵
      if (E.money > 2500) {
        if (E.mercenary < 400) {
          val = 100 / sabo_flag;
          E.money -= val * 5;
          E.mercenary += val;
        }
      }

      break;

      // ３章
      // 兵士と資金の増強
    case 3:
      // 資金
      if (E.money < 100000) {
        E.money += (E.fund / 10 + E.territory * 10) / sabo_flag;
      }

      // 兵士
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

      // ４章
      // 兵士、義勇兵、資金の増強
    case 4:
      // 兵士
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

      // 義勇兵
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

      // 資金
      if (E.money < 75000) {
        E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;
      }

      break;

      // ５章、デバッグ
      // 兵士と資金の増強
    default:
      // 資金
      E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;

      // 兵士
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

      // 資金
      if (E.money < 50000) {
        E.money += (E.fund / 10 + E.territory * 50) / sabo_flag;
      }

      break;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  戦闘処理関係
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

// 戦闘開始前：攻撃力基準値（人望値）の計算と兵力最大値の保存
void duel_standby() {
  // 人望値の計算
  P_pop = P.fame - P.infamy;
  if (P_pop < -20) P_pop = -20;
  E_pop = E.fame - E.infamy;
  if (E_pop < -20) E_pop = -20;


  // 兵力の計算と初期値（最大値）の保存
  P_pow = P.soldier * 10 + P.mercenary * 5;
  P_pow0 = P_pow;
  E_pow = E.soldier * 10 + E.mercenary * 5;
  E_pow0 = E_pow;

  // 義勇兵人数の保存
  P_M0 = P.mercenary;
  E_M0 = E.mercenary;
}

// 戦闘ターン開始：その時点での攻撃力の計算と兵力保存
void duel_turnStart() {
  // プレイヤーの攻撃力計算と上限判定
  P_attack = P.soldier * (2 + (P_pop / 20)) + P.mercenary * 4;
  if (P_attack > 10000) P_attack = 10000;

  // 相手の攻撃力計算と上限判定
  E_attack = E.soldier * (2 + (E_pop / 20)) + E.mercenary * 4;
  if (E_attack > 10000) E_attack = 10000;

  // 兵力保存
  P_start = P_pow;
  E_start = E_pow;
}

// 戦闘ターン処理：選択した行動の相性判定とダメージ処理
void duel_fight(short player, short enemy) {

  // 相性判定
  {
    // 突撃　と　突撃
    if (player == 0 && enemy == 0) {
      P_damage = P_attack * 1;
      E_damage = E_attack * 1;
    }

    // 突撃　と　守り
    else if (player == 0 && enemy == 1) {
      P_damage = 0;
      E_damage = E_attack * 1;
    }

    // 突撃　と　ゆみ
    else if (player == 0 && enemy == 2) {
      P_damage = P_attack * 1.5;
      E_damage = E_attack * 0.5;
    }

    // 守り　と　突撃
    else if (player == 1 && enemy == 0) {
      P_damage = P_attack * 1;
      E_damage = 0;
    }

    // 守り　と　守り
    else if (player == 1 && enemy == 1) {
      P_damage = 0;
      E_damage = 0;
    }

    // 守り　と　ゆみ
    else if (player == 1 && enemy == 2) {
      P_damage = 0;
      E_damage = E_attack * 1;
    }

    // ゆみ　と　突撃
    else if (player == 2 && enemy == 0) {
      P_damage = P_attack * 0.5;
      E_damage = E_attack * 1.5;
    }

    // ゆみ　と　守り
    else if (player == 2 && enemy == 1) {
      P_damage = P_attack * 1;
      E_damage = 0;
    }

    // ゆみ　と　ゆみ
    else if (player == 2 && enemy == 2) {
      P_damage = P_attack * 0.5;
      E_damage = E_attack * 0.5;
    }
  }

  // 追撃戦判定
  if (add_attack) {
    P_damage = P_damage * 3;
    E_damage = E_damage * 2;
  }

  // ダメージ処理
  P_pow -= E_damage;
  E_pow -= P_damage;

  // 実行後の処理
  P_lastcmd = player;
}

// 戦闘ターン終了：ダメージ処理後の兵力計算と調整
void duel_turnEnd() {
  int damage0;

  // プレイヤーの処理
  {
    // ターン開始時と終了時の兵力の差を判定
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

  // 相手の処理
  {
    // ターン開始時と終了時の兵力の差を判定
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

// 相手の行動選択ＡＩ
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

// 戦闘中のメッセージとアニメーション表示
void duel_anime(AppEnv& app_env, short player, short enemy) {
  // 背景、テキスト、キャラクターの読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture battle("res/png/Me5_Battle.png");
  Texture chara6("res/png/06_Rival_C.png");

  // アニメーションカウンタ
  short animetime = 0;

  // テキスト、キャラクター表示用
  short i, j;
  float x, y, x0, y0, x1, y1 = 0;

  // 戦闘アニメーションの処理
  while (1) {
    if (!app_env.isOpen()) return;
    
    // アニメーション開始
    if (animetime < 300) animetime += 1;

    // アニメ終了後、クリックするとループ終了
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

    // 背景表示
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

    // 行動の表示
    {
      // プレイヤーの行動
      {
        drawTextureBox(-telop_W / 2, (-Window::HEIGHT / 2) + (telop_H * 5), telop_W, telop_H,
        0, telop_H * (13 + player), telop_W, telop_H,
        battle, Color(0, 0, 0));

        // ダメージの表示
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

      // 相手の行動
      {
        drawTextureBox(-telop_W / 2, (-Window::HEIGHT / 2) + (telop_H * 3), telop_W, telop_H,
          0, telop_H * (16 + enemy), telop_W, telop_H,
          battle, Color(0, 0, 0));

        // ダメージの表示
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

    // キャラクターのアニメーション
    {
      // プレイヤー側
      for (i = 0; i < 4; i += 1) {
        for (j = 0; j < 4; j += 1) {
          switch (player) {

              // 突撃
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

              // 守り
            case 1:
              x = (-chara_W * 4) - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j);
              y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
              x0 = x + 6 + (16 * (1 + i) * 0.1);
              y0 = y + (16 * (1 + i) * 0.1);
              break;

              // ゆみ
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

          // キャラクター表示（プレイヤー側）
          drawTextureBox(x, y, chara_W, chara_H,
            0, 0, chara_W, chara_H,
            chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          switch (player) {

            // 剣の表示
            case 0:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 0), 0, 16, 16,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // 盾の表示
            case 1:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 1), 0, 16, 16,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // 弓矢の表示
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

      // 相手側のアニメーション
      for (i = 0; i < 4; i += 1) {
        for (j = 0; j < 4; j += 1) {
          switch (enemy) {

              // 突撃
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

              // 守り
            case 1:
              x = (chara_W * 3) + ((chara_W * (1 + i * 0.25)) * j);
              y = (-chara_H * 2) + chara_H * (2 - i * 0.5);
              x0 = x - (10 * (1 + i) * 0.125);
              y0 = y + (16 * (1 + i) * 0.1);
              break;

              // ゆみ
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

          // キャラクター表示（相手側）
          drawTextureBox(x, y, chara_W, chara_H,
            0, 0, chara_W, chara_H,
            chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          switch (enemy) {

              // 剣の表示
            case 0:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 0), 0, 16, 16,
                chara6, Color(1, 0.5, 0.5), M_PI / 2, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // 盾の表示
            case 1:
              drawTextureBox(x0, y0, 16, 16,
                4 + (chara_W * 3) + (16 * 1), 0, 16, 16,
                chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              break;

              // 弓矢の表示
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

// 兵力ゲージと義勇兵ゲージの計算
void setPowGauge() {
  // 兵力ゲージ
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

  // 義勇兵ゲージ
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

// 兵力ゲージのカラーの変更
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

// 通常戦闘ＢＧＭの設定
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

// 追撃戦ＢＧＭの設定
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
  シミュレーションパート関係
  bool on_button(Vec2f, float, float, short, short);

  Color setButtonColor(bool);
  Color setTimeColor(float);

  Media setBGM_Main(short);
 */

// マウスカーソルの座標判定
bool on_button(Vec2f pos, float x, float y, short tex_width, short tex_height) {
  if (x < pos.x() && pos.x() < x + tex_width &&
      y < pos.y() && pos.y() < y + tex_height) {
    return true;
  }
  return false;
}

// ボタンカラーの変更
Color setButtonColor(bool on_mouse) {
  if (on_mouse) {
    return Color(1, 0.8, 0.4);
  }
  return Color(1, 1, 1);
}

// タイムバーのカラー変更
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

// 政略モードＢＧＭの設定
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
イベント処理（テキスト表示とアニメーション系）
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

// オープニング
void _Opening(AppEnv& app_env) {
  // 背景の読み込み
  Texture title_OP("res/png/Sp4_Opening.png");

  // テロップの読み込み
  Texture telop("res/png/T01_Opening.png");

  short animetime = 0;     // アニメーションのタイムカウンタ

  while (animetime < (Window::HEIGHT) * 2) {
    if (!app_env.isOpen()) return;

    // テロップのスクロール
    animetime += 1;

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      animetime = (Window::HEIGHT) * 2;
    }

    // アニメの加速
    if (app_env.isPressKey('S')) {
      animetime += 2;
    }

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      title_OP, Color(0.5, 0.5, 0.5));

    // テロップ表示
    drawTextureBox((-telop_W / 2), animetime - (Window::HEIGHT * 1.5) - telop_H, telop_W, Window::HEIGHT,
      0, 0, telop_W, Window::HEIGHT,
      telop, Color(1, 1, 1));

    app_env.update();
  }
}

// プロローグ・イベント
void _Prologue_event(AppEnv& app_env) {
  // 背景の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");
  Texture event2("res/png/BG3_Castle_out.png");
  Texture blackBG("res/png/BG5_Black.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara5("res/png/05_Rival_B.png");
  Texture chara6("res/png/06_Rival_C.png");

  // テロップの読み込み
  Texture telopA("res/png/T02_Prologue_a.png");
  Texture telopB("res/png/T02_Prologue_b.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  // ＳＥの読み込みと設定
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

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ
  short r = 0;             // ＳＥ再生用乱数
  short r0, r1, r2;        // キャラクター表示用

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    2, 2, 2, 3, 2, 4, 2, 2, 3, 2,
    2, 3, 1, 2, 2, 2, 2, 1, 3, 3,
    3, 4, 1, 3, 3, 3, 4, 2, 3, 2,
    3, 2, 4, 4, 3, 1,
    2, 3, 3, 3, 1, 4, 3, 4, 3, 3
  };

  // キャラクター表示位置を乱数で決定
  srand((unsigned)time(NULL));
  r0 = 0 - rand() % chara_H;
  r1 = r0 - rand() % chara_H;
  r2 = r1 - rand() % chara_H;

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;

      // テキスト画像 A が終了
      if (i == 36) {
        telop_pos = 0;
      }

      // 画面切り替え
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

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
    {
      // セピア色を目指したけどダメだったっぽい背景
      if (i == 3) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 0.6, 0.2));
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
          0, 365, Window::WIDTH, Window::HEIGHT - 365,
          event1, Color(1, 1, 1));
      }

      // 城：屋内
      else if (i < 6) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 1, 1));
      }

      // 黒
      else if (i < 11 || i == 17) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          blackBG, Color(1, 1, 1));
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
          0, 365, Window::WIDTH, Window::HEIGHT - 365,
          event1, Color(1, 1, 1));
      }

      // 城：屋内
      else if (i < 17) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 1, 1));
      }

      // 城：屋外
      else if (i < 36) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event2, Color(1, 1, 1));
      }

      // 草原
      else if (i < 46) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));
      }
    }

    // キャラクターのアニメ
    {
      // 兵士が逃げた
      if (i == 3) {
        // アニメーションの管理
        if (animetime < Window::WIDTH) {
          animetime += 2;
        }
        else {
          animetime = 0;
          r0 = 0 - rand() % (chara_H * 2);
          r1 = r0 - rand() % (chara_H * 2);
          r2 = r1 - rand() % (chara_H * 2);
        }

        // 兵士１
        drawTextureBox((Window::WIDTH / 2) + (-r0 * 3) - animetime * 1.25, r0, (chara_W * 3), (chara_H * 3),
          0, 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));

        // 兵士２
        drawTextureBox((Window::WIDTH / 2) + (-r1 * 3) - animetime * 1.5, r1, (chara_W * 3), (chara_H * 3),
          0, 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));

        // 兵士３
        drawTextureBox((Window::WIDTH / 2) + (-r2 * 3) - animetime * 1.75, r2, (chara_W * 3), (chara_H * 3),
          0, 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
      }

      // じい登場
      else if (i > 6 && i < 11) {
        // 登場シーン
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
        // 登場後
        else {
          drawTextureBox((Window::WIDTH / 2) - (chara_W * 18), (-Window::HEIGHT / 2), (chara_W * 10), (chara_H * 10),
            0, 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
            0, 365, Window::WIDTH, Window::HEIGHT - 365,
            event1, Color(1, 1, 1));
        }
      }

      // 王子飛び出す
      else if (i == 11 || i == 12) {
        // 王子
        {
          if (i == 12) {
            if (animetime < 60) {
              animetime += 1;
            }

            // 王子ジャンプ
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
            // 猛ダッシュ
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
        // じい
        {
          drawTextureBox((-chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
        }
      }

      // じい独り言
      else if (i > 11 && i < 17) {
        // １人
        if (i < 15) {
          drawTextureBox((-chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
        }
        // 兵士登場
        else {
          // 囲まれる
          if (i == 15) {
            if (animetime < chara_W * 1) {
              animetime += 1;
            }
            // 兵士
            drawTextureBox((-chara_W * 3) + (animetime * 10) - 200, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            drawTextureBox((chara_W * 1) - (animetime * 10) + 200, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            // じい
            drawTextureBox((-chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // 画面外に向かいながら、フルボッコ
          else {
            if (animetime < chara_W * 5) {
              animetime += 1;
            }
            // 兵士
            drawTextureBox((-chara_W * 3) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            drawTextureBox((chara_W * 1) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
            // じい
            drawTextureBox((-chara_W * 1) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));

            // 効果音再生
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

      // 王子フラれる
      else if (i > 17 && i < 22) {
        // フラれ　なう
        if (i < 21) {
          // 王子ニヤケ顔
          if (i < 19) {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 3, 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // 王子ショック
          else {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              0, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // ひめ
          drawTextureBox((chara_W * 7), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara3, Color(1, 1, 1));
        }
        // ひめ移動
        else {
          if (animetime < chara_W * 3) {
            animetime += 1;
          }
          // 王子
          drawTextureBox((chara_W * 2) + (animetime), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, chara_H * 1, chara_W, chara_H,
            chara1, Color(1, 1, 1));
          // ひめ
          drawTextureBox((chara_W * 7) + (animetime * 3), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, 0, chara_W, chara_H,
            chara3, Color(1, 1, 1));
        }
      }

      // 王子ショック
      else if (i >= 22 && i < 32) {
        // たそがれ　なう
        if (i == 22) {
          drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
            0, chara_H * 1, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        // じい、兵士登場
        else {
          // 移動
          if (i == 23) {
            if (animetime < chara_W * 2) {
              animetime += 1;
            }
            // 王子
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
            // じい
            drawTextureBox((chara_W * 2) + (animetime * 10) - 400, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));
            // 兵士
            drawTextureBox((-chara_W * 2) + (animetime * 8) - 320, (-chara_H), (chara_W * 3), (chara_H * 3),
              0, chara_H * 0, chara_W, chara_H,
              chara6, Color(1, 1, 1));
          }
          // 移動後
          else {
            // 王子
            {
              // 王子　ショック
              if (i < 29) {
                drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 0, chara_H * 1, chara_W, chara_H,
                  chara1, Color(1, 1, 1));
              }
              // 王子　あわて
              else {
                drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 1, chara_H * 1, chara_W, chara_H,
                  chara1, Color(1, 1, 1));
              }
            }
            // じい
            {
              // じい　あわて
              if (i < 27) {
                drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 1, chara_H * 1, chara_W, chara_H,
                  chara2, Color(1, 1, 1));
              }
              // じい　ショック
              else if (i < 29) {
                drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 0, chara_H * 1, chara_W, chara_H,
                  chara2, Color(1, 1, 1));
              }
              // じい　怒り
              else {
                drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  chara_W * 2, chara_H * 0, chara_W, chara_H,
                  chara2, Color(1, 1, 1));
              }
            }
            // 兵士
            {
              if (i < 31) {
                drawTextureBox((-chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
                  0, chara_H * 0, chara_W, chara_H,
                  chara6, Color(1, 1, 1));
              }
              // 退職
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

      // 逝き面とデブ
      else if (i >= 32 && i < 36) {

        // デブ＆逝き面
        {
          // 登場
          if (i == 32) {
            if (animetime < chara_W * 2) {
              animetime += 1;
            }
            // デブ
            drawTextureBox((-chara_W * 2) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara5, Color(1, 1, 1));
            // 逝き面
            drawTextureBox((-chara_W * 6) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // 退場
          else if (i == 35) {
            if (animetime < chara_W * 3) {
              animetime += 1;
            }
            // デブ
            drawTextureBox((-chara_W * 2) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara5, Color(1, 1, 1));
            // 逝き面
            drawTextureBox((-chara_W * 6) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          else {
            // デブ
            drawTextureBox((-chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara5, Color(1, 1, 1));
            // 逝き面
            drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
        }
        // 王子
        {
          // あわて
          if (i < 32) {
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // 困り顔
          else {
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
        // じい
        {
          // 怒り
          if (i < 32) {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 2, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // 通常
          else {
            drawTextureBox((chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
        }
      }

      // 家なき子
      else if (i >= 36) {
        // 王子
        {
          // ショック
          if (i < 38) {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // 困り顔
          else {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
        // じい
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
    }

    // テロップ表示
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

// チュートリアル
void _tutorial(AppEnv& app_env) {
  // 背景とボタンの読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");

  // キャラクターの読み込み
  Texture chara6("res/png/06_Rival_C.png");

  // テロップの読み込み
  Texture telop("res/png/T02_Prologue_b.png");
  Texture system("res/png/Me4_System.png");
  Texture battle("res/png/Me5_Battle.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_tutorial.wav");
  thema.looping(true);
  thema.gain(0.2);

  Media bgm_a = setBGM_a(stage_flag);
  bgm_a.looping(true);
  bgm_a.gain(0.2);

  // ＳＥの読み込みと設定
  Media se_start("res/wav/se_battle.wav");
  se_start.gain(0.2);

  // ボタンの判定、色
  bool on_mouse;
  Color angle;

  // ボタン、キャラクターの表示位置
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);

  short i = 0, i0 = 0;     // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ
  short n, j;              // ボタン表示用
  float x0, y0;            // イベント処理用

  short telop_pos = telop_H * 29;  // テキスト画像切り出し開始位置
  short telop_line[] = {           // テキスト画像切り出し行数
    4, 2, 1, 1, 4, 3, 4, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // マウス座標の取得
    mouse_pos = app_env.mousePosition();

    // ＢＧＭ再生
    if (i < 5 && !thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (i > 4) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        animetime = 0;
        i += 1;

        telop_pos += telop_H * telop_line[i0];
        i0 += 1;
      }
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      thema.stop();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) {
      bgm_a.stop();
      _BG_fade(app_env, mainBG, 1.0, 0, 0.02);
      break;
    }

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // クリック回数に対応したイベントの表示
    switch (i) {

      // 兵力増強を選択してもらう
      case 0:
        // テロップ表示
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - 1), telop_W, telop_H * 1,
          0, telop_pos, telop_W, telop_H * 1,
          telop, Color(0, 0, 0));

        // ボタン表示
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

        // 兵士募集を選択してもらう
      case 1:
        // 兵力増強
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 1, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));

        // テロップ表示
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        // ボタン表示
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

        // 人数を選択してもらう
      case 2:
        // 兵力増強
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 1, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));

        // 兵士募集
        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 9, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));

        // テロップ表示
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        // ボタン表示
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

        // 侵攻を選択してもらう
      case 3:
        // テロップ表示
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        // ボタン表示
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

        // 守りを選択してもらう
      case 4:
        thema.stop();

        // 戦闘前の処理
        while (animetime < 90) {
          if (!app_env.isOpen()) return;

          // 戦闘開始ＳＥ再生
          if (animetime == 0) se_start.play();

          app_env.setupDraw();

          // 背景表示
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            mainBG, Color(1, 1, 1));
          // 戦闘開始
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
            0, telop_pos, telop_W, telop_H * telop_line[i0],
            telop, Color(0, 0, 0));

          app_env.update();

          animetime += 1;
        }

        // ボタンの表示位置指定
        {
          x = (-Window::WIDTH / 2) + 2;
          y = (Window::HEIGHT / 2) - 358;
        }

        // アニメーションカウンタのリセット
        animetime = 0;

        // テキスト切り出し位置の更新
        telop_pos += telop_H * telop_line[i0];
        i0 += 1;

        // 攻撃力基準値と兵力初期値を取得
        duel_standby();

        bgm_a.play();

        // 戦闘画面の表示
        while (1) {
          if (!app_env.isOpen()) return;
          app_env.setupDraw();

          // マウス座標の取得
          mouse_pos = app_env.mousePosition();

          // 背景表示
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            mainBG, Color(1, 1, 1));

          // テロップ表示
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
            0, telop_pos, telop_W, telop_H * telop_line[i0],
            telop, Color(0, 0, 0));

          // キャラクターの表示
          {
            // プレイヤー側
            x0 = -chara_W * 4;
            y0 = -chara_H * 2;

            for (i = 0; i < 4; i += 1) {
              for (j = 0; j < 4; j += 1) {
                drawTextureBox(x0 - (chara_W * (i * 0.25)) - ((chara_W * (1 + i * 0.25)) * j), y0 + chara_H * (2 - i * 0.5), chara_W, chara_H,
                  0, 0, chara_W, chara_H,
                  chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              }
            }

            // 相手側
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

          // ボタンの表示
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

                // 戦闘処理の表示
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

          // クリックしたらループ終了
          if (i > 4) {
            break;
          }

          app_env.update();
        }

        break;

        // イベント進行
      default:
        // テロップ表示
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i0]), telop_W, telop_H * telop_line[i0],
          0, telop_pos, telop_W, telop_H * telop_line[i0],
          telop, Color(0, 0, 0));

        break;
    }

    app_env.update();
  }
  thema.stop();
}

// １章・イベント
void _1ststage_event(AppEnv& app_env) {
  // 背景の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");

  // テロップの読み込み
  Texture telop("res/png/T03_1stStage.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    3, 3, 5, 2, 5, 5, 3, 4, 4, 5,
    2, 5
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      if (i == 4) {
        _BG_slide(app_env, event1, mainBG, Color(1, 1, 1), Color(1, 1, 1));
      }
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
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

    // キャラクターのアニメ
    if (i < 5) {
      // 王子
      drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
        chara_W * 0, chara_H * 0, chara_W, chara_H,
        chara1, Color(1, 1, 1));
      // じい
      drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
        chara_W * 0, chara_H * 0, chara_W, chara_H,
        chara2, Color(1, 1, 1));
    }

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// ２章・イベント
void _2ndstage_event(AppEnv& app_env) {
  // 背景の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");

  // テロップの読み込み
  Texture telop("res/png/T04_2ndStage.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ
  bool stop = false;       // アニメーション終了までイベントストップ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    4, 3, 1, 4, 2, 4, 3, 5, 3, 3,
    3, 4, 3, 5, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
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

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
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

    // キャラクターのアニメ
    if (i < 12) {

      // 余裕の高笑い
      if (i == 2) {
        // 王子
        drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 3, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // じい
        drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }

      // 王子　怒りの一撃
      else if (i == 4) {
        // アニメーションカウンタ処理
        if (animetime < (chara_W * 2) + 60) {
          stop = true;
          animetime += 1;
        }
        else {
          stop = false;
        }

        // じい
        {
          // 殴られる前
          if (animetime < (chara_W * 2) + 60) {
            drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // 殴られた
          else {
            drawTextureBox((chara_W * 0), (-chara_H * 1), chara_W * 3, chara_H * 3,
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1), M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
        }

        // 王子
        {
          // アニメーション開始
          if (animetime > 60) {
            // 振りかぶる
            if (animetime <= (chara_W * 1) + 60) {
              drawTextureBox((chara_W * 2) + (animetime - 60) * 3, (-chara_H * 1), chara_W * 3, chara_H * 3,
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // 突進
            else if (animetime < (chara_W * 2) + 60) {
              drawTextureBox((-chara_W * 2) - (animetime - ((chara_W * 1) + 60)) * 6 + (chara_W * 6), (-chara_H * 1), chara_W * 3, chara_H * 3,
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // 殴った後
            else {
              drawTextureBox((-chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
                chara_W * 3, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // 静止状態
          else {
            drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
              chara_W * 3, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
      }

      // 王子マウント
      else if (i > 4) {
        // じい
        drawTextureBox((chara_W * 0), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 1, chara_W, chara_H,
          chara2, Color(1, 1, 1), M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
        // 王子
        drawTextureBox((-chara_W * 3), (-chara_H / 2), chara_W * 3, chara_H * 3,
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
      }

      // 会話
      else if (i < 4) {
        // 王子
        drawTextureBox((chara_W * 2), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // じい
        drawTextureBox((-chara_W * 4), (-chara_H * 1), chara_W * 3, chara_H * 3,
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
    }

    // テロップ表示
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

// ３章・イベント
void _3rdstage_event(AppEnv& app_env) {
  // 背景の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara5("res/png/05_Rival_B.png");

  // テロップの読み込み
  Texture telop("res/png/T05_3rdStage.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    4, 2, 3, 4, 3, 3, 4, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;

      if (i == 7) {
        _BG_slide(app_env, event1, mainBG, Color(1, 1, 1), Color(1, 1, 1));
      }
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
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

    // キャラクターのアニメ
    if (i < 7) {
      // 王子
      {
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
      }
      // じい
      {
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
      // デブ
      {
        if (animetime < chara_W * 2) {
          animetime += 1;
        }

        // 登場
        if (i == 1) {
          drawTextureBox((-chara_W * 5) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 2, chara_H * 0, chara_W, chara_H,
            chara5, Color(1, 1, 1));
        }
        // 退場
        else if (i == 4) {
          drawTextureBox((-chara_W * 5) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara5, Color(1, 1, 1));
        }
        // 静止状態
        else if (i > 0 && i < 4) {
          drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara5, Color(1, 1, 1));
        }
      }
    }

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// ３章・スパイ実行
void _3rdstage_spy_act(AppEnv& app_env) {
  // 背景の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");

  // テロップの読み込み
  Texture telop("res/png/T05_3rdStage.png");

  short i = 0;             // クリック回数のカウンタ

  short telop_pos = telop_H * 29;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    3, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
}

// ３章・特殊条件クリア
void _3rdstage_clear(AppEnv& app_env) {
  // 背景の読み込み
  Texture event1("res/png/BG2_Castle_in.png");

  // キャラクターの読み込み
  Texture chara5("res/png/05_Rival_B.png");
  Texture chara6("res/png/06_Rival_C.png");

  // テロップの読み込み
  Texture telop("res/png/T05_3rdStage.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  // ＳＥの読み込みと設定
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

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ
  short r = 0;             // ＳＥ再生用乱数

  short telop_pos = telop_H * 42;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    2, 5, 2
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      event1, Color(1, 1, 1));

    // キャラクターのアニメ
    {
      // リンチ
      if (i == 2) {
        if (animetime < chara_W * 4) {
          animetime += 1;
        }

        // 兵士１
        drawTextureBox((-chara_W * 5) + animetime * 8, (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // 兵士２
        drawTextureBox((-chara_W * 2) + animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // デブ
        drawTextureBox((chara_W * 5) + animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 1, chara_H * 1, chara_W, chara_H,
          chara5, Color(1, 1, 1));

        // 効果音再生
        {
          srand((unsigned)time(NULL));
          if (!rush[r].isPlaying()) {
            r = rand() % 4;
            rush[r].play();
          }
        }
      }

      // デブ追い詰められる
      else {
        // 兵士１
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // 兵士２
        drawTextureBox((-chara_W * 2), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 2, chara_H * 0, chara_W, chara_H,
          chara6, Color(1, 1, 1));
        // デブ
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 1, chara_H * 1, chara_W, chara_H,
          chara5, Color(1, 1, 1));
      }
    }

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// ４章・イベント
void _4thstage_event(AppEnv& app_env) {
  // 背景の読み込み
  Texture event1("res/png/BG2_Castle_in.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");
  Texture chara4("res/png/04_Rival_A.png");

  // テロップの読み込み
  Texture telop("res/png/T06_4thStage_a.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    3, 5, 3, 4, 4, 4, 2, 2, 2, 3,
    2, 3, 2, 2, 2, 2, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      event1, Color(1, 1, 1));

    // キャラクターのアニメ
    {
      // 王子とじい
      {
        if (i == 7 || i == 8) {
          if (animetime < chara_W * 1) {
            animetime += 1;
          }
        }

        // 王子
        {
          // 困り顔
          if (i > 0 && i < 9) {
            // 移動前
            if (i < 8) {
              drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // 手紙に気づいて移動
            else {
              drawTextureBox((chara_W * 5) - animetime * 4, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // 通常
          else if (i < 16) {
            // 移動前
            if (i < 8) {
              drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // 移動後
            else {
              drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // 怒り
          else {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 2, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }

        // じい
        {
          // あわて
          if (i < 2) {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // 怒り
          else if (i == 17) {
            drawTextureBox((-chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 2, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
          // 通常
          else {
            // 移動前
            if (i < 7) {
              drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
            // 手紙に気づいて移動
            else if (i == 7) {
              drawTextureBox((chara_W * 1) - animetime * 5, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
            // 移動後
            else {
              drawTextureBox((-chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 0, chara_H * 0, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
          }
        }
      }

      // ひめと逝き面
      {
        if (i == 1 || i == 6) {
          if (animetime < chara_W * 2) {
            animetime += 1;
          }
        }

        // 逝き面
        if (i < 7) {
          // 登場
          if (i == 1) {
            drawTextureBox((-chara_W * 5) + animetime * 6 - 240, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // 退場
          else if (i == 6) {
            drawTextureBox((-chara_W * 5) - animetime * 6, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // 静止状態
          else if (i > 0) {
            drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
        }
        // ひめ
        if (i < 7) {
          // 登場
          if (i == 1) {
            drawTextureBox((-chara_W * 7) + animetime * 4 - 160, (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // 退場
          else if (i == 6) {
            drawTextureBox((-chara_W * 7) - animetime * 5, (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // 静止状態
          else if (i > 0) {
            drawTextureBox((-chara_W * 7), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
        }
      }
    }

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// ４章・特殊条件クリア
void _4thstage_clear(AppEnv& app_env) {
  // 背景の読み込み
  Texture event1("res/png/BG2_Castle_in.png");

  // キャラクターの読み込み
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara6("res/png/06_Rival_C.png");

  // テロップの読み込み
  Texture telop("res/png/T06_4thStage_b.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    2, 4, 4, 4, 4, 5, 2
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      event1, Color(1, 1, 1));

    // キャラクターのアニメ
    {
      // 逝き面
      {
        // 通常
        if (i < 3) {
          drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
        // 焦り
        else if (i < 5) {
          drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 1, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
        // ショック
        else {
          drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
      }
      // 兵士
      {
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 1, chara_H * 1, chara_W, chara_H,
          chara6, Color(1, 1, 1));
      }
    }

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// ５章・イベント
void _5thstage_event(AppEnv& app_env) {
  // 背景の読み込み
  Texture event1("res/png/BG2_Castle_in.png");
  Texture event2("res/png/BG3_Castle_out.png");
  Texture event3("res/png/BG4_Shrine.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara5("res/png/05_Rival_B.png");
  Texture chara7("res/png/07_Unknown.png");

  // テロップの読み込み
  Texture telop("res/png/T07_5thStage_a.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Mainthema.wav");
  thema.looping(true);
  thema.gain(0.2);

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    5, 4, 4, 2, 3, 4, 4, 4, 4, 3,
    4, 4, 3, 3
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;

      // 画面切り替え
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

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    // 背景表示
    {
      // 城：屋内
      if (i < 2) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event1, Color(1, 1, 1));
      }
      // 城：屋外
      else if (i < 8 || i >= 11) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event2, Color(1, 1, 1));
      }
      // 遺跡
      else if (i < 11) {
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          event3, Color(1, 1, 1));
      }
    }

    // キャラクターのアニメ
    {
      // 城：屋内
      if (i < 2) {
        // 王子
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // じい
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }

      // ひめ誘拐
      else if (i < 8) {
        // アニメーション管理
        if (i == 5) {
          if (animetime < chara_W * 2) {
            animetime += 1;
          }
        }

        // 王子
        {
          // ひめ飛ぶ前
          if (i < 6) {
            if (i < 5) {
              drawTextureBox((chara_W * 7), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
            // ひめ飛んだ
            else {
              drawTextureBox((chara_W * 7) - animetime, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara1, Color(1, 1, 1));
            }
          }
          // ひめ飛んでいった後
          else {
            drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
        }
        // じい
        {
          // ひめ飛ぶ前
          if (i < 6) {
            if (i < 5) {
              drawTextureBox((chara_W * 3), (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
            // ひめ飛んだ
            else {
              drawTextureBox((chara_W * 3) - animetime, (-chara_H), (chara_W * 3), (chara_H * 3),
                chara_W * 1, chara_H * 1, chara_W, chara_H,
                chara2, Color(1, 1, 1));
            }
          }
          // ひめ飛んでいった後
          else {
            drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara2, Color(1, 1, 1));
          }
        }
        // 逝き面
        {
          // 登場
          if (i == 6) {
            if (animetime < chara_W) {
              animetime += 1;
            }
            drawTextureBox((-chara_W * 6) + animetime * 8 - 160, (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
          // 静止状態
          else if (i > 5) {
            drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara4, Color(1, 1, 1));
          }
        }
        // ボス
        {
          if (i < 5) {
            drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara7, Color(1, 1, 1));
          }
          // ひめ誘拐
          else if (i == 5) {
            drawTextureBox((-chara_W * 5), (-chara_H) + animetime * 8, (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 0, chara_W, chara_H,
              chara7, Color(1, 1, 1));
          }
        }
        // ひめ
        {
          if (i < 5) {
            drawTextureBox((-chara_W * 6), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
              chara_W * 4, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // 誘拐される
          else if (i == 5) {
            drawTextureBox((-chara_W * 6), (-chara_H * 2) + animetime * 8, (chara_W * 3), (chara_H * 3),
              chara_W * 4, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
        }
      }

      // ぼっちデブ
      else if (i < 11) {
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara5, Color(1, 1, 1));
      }

      // 作戦開始
      else {
        // 王子
        drawTextureBox((chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara1, Color(1, 1, 1));
        // じい
        drawTextureBox((chara_W * 1), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
        // 逝き面
        drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara4, Color(1, 1, 1));
      }
    }

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
  thema.stop();
}

// ５章・クリア
void _5thstage_clear(AppEnv& app_env) {
  // 背景の読み込み
  Texture event3("res/png/BG4_Shrine.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara7("res/png/07_Unknown.png");

  // テロップの読み込み
  Texture telop("res/png/T07_5thStage_b.png");

  // ＢＧＭの読み込みと設定
  Media bgm("res/wav/Battle_EP5_A.wav");
  bgm.gain(0.2);

  short i = 0;             // クリック回数のカウンタ
  short animetime = 0;     // アニメーションのタイムカウンタ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    4, 4, 3, 1, 2, 4, 2, 2, 1, 2
  };

  // ＢＧＭ再生
  bgm.play();

  while (1) {
    if (!app_env.isOpen()) return;

    // 左クリックでイベント進行
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      telop_pos += telop_H * telop_line[i];
      i += 1;
      animetime = 0;
    }

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) {
      bgm.stop();
      break;
    }

    app_env.setupDraw();

    // 背景表示
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

    // キャラクターのアニメ
    {
      // 光
      if (i > 1 && i < 8) {
        drawFillBox(-10, 0, 20, (Window::HEIGHT / 2), Color(1, 0.8, 0.4, 0.5));
      }

      if (i > 4 && i < 8) {
        drawFillBox(0, 0, 120, 20, Color(1, 0.8, 0.4, 0.5), M_PI / 18, Vec2f(1, 1), Vec2f(0, 0));
      }

      // 王子
      {
        // 王子　閃く前
        if (i < 4) {
          drawTextureBox((-chara_W * 7), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        // 王子　閃いた
        else if (i < 8) {
          drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        // 王子　ドヤ顔
        else {
          drawTextureBox((-chara_W * 6), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 3, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
      }

      // じい
      {
        // 王子　閃く前
        if (i < 4) {
          drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 0, chara_W, chara_H,
            chara2, Color(1, 1, 1));
        }
        // 王子にぶっ飛ばされる
        else {
          // 倒れ直後
          if (i == 4) {
            drawTextureBox((-chara_W * 3.5), (chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 1, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1), -M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
          // ダウン
          else {
            drawTextureBox((-chara_W * 3.5), (chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara2, Color(1, 1, 1), -M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
        }
      }

      // 逝き面
      {
        // あわて
        if (i < 5) {
          drawTextureBox((-chara_W * 9), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
            chara_W * 1, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
        // ショック
        else {
          drawTextureBox((-chara_W * 9), (-chara_H * 2), (chara_W * 3), (chara_H * 3),
            chara_W * 0, chara_H * 1, chara_W, chara_H,
            chara4, Color(1, 1, 1));
        }
      }

      // ボス
      {
        // あわて
        if (i < 5) {
          drawTextureBox((chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 1, chara_H * 1, chara_W, chara_H,
            chara7, Color(1, 1, 1));
        }
        // 光にやられる
        else {
          // やられ
          if (i < 8) {
            drawTextureBox((chara_W * 4), (-chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara7, Color(1, 1, 1));
          }
          // ダウン
          else {
            drawTextureBox((chara_W * 4), (chara_H), (chara_W * 3), (chara_H * 3),
              chara_W * 0, chara_H * 1, chara_W, chara_H,
              chara7, Color(1, 1, 1), -M_PI / 2, Vec2f(1, 1), Vec2f(0, 0));
          }
        }
      }
    }

    // テロップ表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * (6 - telop_line[i]), telop_W, telop_H * telop_line[i],
      0, telop_pos, telop_W, telop_H * telop_line[i],
      telop, Color(0, 0, 0));

    app_env.update();
  }
}

// エンディング
void _Ending(AppEnv& app_env) {
  // 背景の読み込み
  Texture event2("res/png/BG3_Castle_out.png");
  Texture blackBG("res/png/BG5_Black.png");
  Texture Ending("res/png/Sp5_Ending.png");

  // キャラクターの読み込み
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara3("res/png/03_Heroine.png");

  // ＢＧＭの読み込みと設定
  Media thema("res/wav/Event_Ending.wav");
  thema.looping(true);
  thema.gain(0.2);

  // テロップの読み込み
  Texture telopA("res/png/T08_Ending_a.png");
  Texture telopB("res/png/T08_Ending_b.png");

  short i = 0;             // クリック回数のカウンタ
  float animetime = 0;     // アニメーションのタイムカウンタ
  bool stop = false;       // プレイヤーの操作禁止フラグ

  short telop_pos = 0;         // テキスト画像切り出し開始位置
  short telop_line[] = {       // テキスト画像切り出し行数
    4, 3, 3, 3, 2, 1, 3, 4, 2
  };

  while (1) {
    if (!app_env.isOpen()) return;

    // ＢＧＭ再生
    if (!thema.isPlaying()) {
      thema.play();
    }

    // 左クリックでイベント進行
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

    // スキップ
    if (app_env.isPushKey('S')) {
      app_env.flushInput();
      i = (sizeof(telop_line) / sizeof(telop_line[0]));
    }

    // テキストを最後まで表示したらループ終了
    if (i == (sizeof(telop_line) / sizeof(telop_line[0]))) break;

    app_env.setupDraw();

    // 背景表示
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

    // キャラクターのアニメ
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

      // 王子
      {
        if (i < 5) {
          drawTextureBox((chara_W * 0), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 3, chara_H * 0, chara_W, chara_H,
            chara1, Color(1, 1, 1));
        }
        else if (i < 7) {
          // 踊りだす
          if (i == 5) {
            drawTextureBox((chara_W * 0) + std::cos(animetime) * 20, (-chara_H) + std::sin(animetime) * 10, (chara_W * 3), (chara_H * 3),
              chara_W * 3, chara_H * 0, chara_W, chara_H,
              chara1, Color(1, 1, 1));
          }
          // じいに激突
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
      // じい
      {
        drawTextureBox((-chara_W * 5), (-chara_H), (chara_W * 3), (chara_H * 3),
          chara_W * 0, chara_H * 0, chara_W, chara_H,
          chara2, Color(1, 1, 1));
      }
      // ひめ
      {
        if (i < 5) {
          drawTextureBox((chara_W * 3), (-chara_H), (chara_W * 3), (chara_H * 3),
            chara_W * 3, chara_H * 0, chara_W, chara_H,
            chara3, Color(1, 1, 1));
        }
        else if (i < 7) {
          // 踊りだす
          if (i == 5) {
            drawTextureBox((chara_W * 3) + std::cos(animetime) * 20, (-chara_H) + std::sin(animetime) * 10, (chara_W * 3), (chara_H * 3),
              chara_W * 3, chara_H * 0, chara_W, chara_H,
              chara3, Color(1, 1, 1));
          }
          // じいに激突
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

    // テロップ表示
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

  // スクロール
  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // テロップのスクロール
    if (animetime < (Window::HEIGHT * 1) + telop_H) {
      animetime += 0.5;
    }

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      Ending, Color(0.5, 0.5, 0.5));

    // テロップ表示
    drawTextureBox((-telop_W / 2), animetime - ((Window::HEIGHT * 1.5) + telop_H), telop_W, Window::HEIGHT,
      0, 0, telop_W, Window::HEIGHT,
      telopB, Color(1, 1, 1));

    app_env.update();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////

