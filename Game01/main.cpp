//
// Revival of Kingdom
//

#include "appEnv.hpp"
#include "_MyVar_list.hpp"    // 変数リスト
#include "_MyFunc_list.hpp"   // 外部処理関数
#include <time.h>


// 関数（ゲーム本編用）
////////////////////////////////////////////////////////////////////////////////////////////////////

// はい・いいえ
//  0：タイトル：コンティニュー
//  1：セーブ
//  2：侵攻
//  3：売却
//  4：スパイ
//  5：妨害工作
//  6：風説の流布
//  7：毒を盛る
//  8：演説
//  9：ゴシップ
// 10：反乱
// 11：買収
int yes_or_no(AppEnv& app_env, short flag) {
  // 背景、ボタン、テキスト画像の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture blackBG("res/png/BG5_Black.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");

  // ボタン画像表示位置の情報
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float y0 = (Window::HEIGHT / 2) - 360;

  // ボタンのカーソル判定、色
  bool on_mouse;
  Color angle;

  while (1) {
    if (!app_env.isOpen()) return 0;
    app_env.setupDraw();

    // 相手の行動開始までのカウント
    if (E_time < 7200) {
      E_time += 1;
    }

    // カーソル座標の取得
    mouse_pos = app_env.mousePosition();

    // 背景表示
    if (flag == 0) {
      // タイトル：コンティニュー選択
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

    // テロップ表示
    switch (flag) {

      // タイトル：コンティニュー選択
      case 0:
        y = y0 + (menu_H + 2) * 3;
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 0, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「セーブ」選択
      case 1:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 5, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 5, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「侵攻」選択
      case 2:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 4, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 85, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「売却」選択
      case 3:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 8, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 20, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「スパイ」選択
      case 4:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 12, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 57, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「妨害工作」選択
      case 5:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 13, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 59, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「風説の流布」選択
      case 6:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 14, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 61, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「毒を盛る」選択
      case 7:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 15, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 63, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「演説」選択
      case 8:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 16, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 66, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「ゴシップ」選択
      case 9:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 17, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 67, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「反乱」選択
      case 10:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 18, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 68, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;

        // 「買収」選択
      case 11:
        drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
          0, menu_H * 3, menu_W, menu_H,
          button, Color(1, 0.8, 0.4));
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 81, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        break;
    }

    // ボタンの表示
    {
      // はい
      {
        // マウス判定による画像カラーの設定
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);

        drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 19, menu_W, menu_H,
          button, angle);

        // マウスクリックの可否とクリックしたときの処理
        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            return 1;
          }
        }
      }

      // いいえ
      {
        // マウス判定による画像カラーの設定
        on_mouse = on_button(mouse_pos, x, y - (menu_H + 2) * 3, menu_W, menu_H);
        angle = setButtonColor(on_mouse);

        drawTextureBox(x, y - (menu_H + 2) * 3, menu_W, menu_H,
          0, menu_H * 20, menu_W, menu_H,
          button, angle);

        // マウスクリックの可否とクリックしたときの処理
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

// 売却
void item_sale(AppEnv& app_env) {
  // 背景、テキスト画像の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture f_list("res/png/Me3_FundList.png");
  Texture system("res/png/Me4_System.png");
  Texture buytxt("res/png/Me6_BuyTxt.png");

  // ボタン画像表示位置の情報
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // ボタンのカーソル判定、色
  bool on_mouse;
  Color angle;

  // フラグリセット
  button_flag = -1;

  // ボタン表示位置指定
  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // 相手の行動開始までのカウント
    if (E_time < 7200) {
      E_time += 1;
    }

    // カーソル座標の取得
    mouse_pos = app_env.mousePosition();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // ボタン表示
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 8, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // 売却リストの表示
    if (button_flag == -1) {
      // どれを売却しますか？
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
        0, telop_H * 18, telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // やめる
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

      // ボタンの表示
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
              // 売却済み
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 19, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // アイテムの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * (27 + i * 2), telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();

                // はいを選ぶとループ終了
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

    // アイテムを選択したとき
    else {
      // 売却しました
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
        0, telop_H * 21, telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // マウスクリックでループ終了
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

// 資金繰り
void add_money(AppEnv& app_env) {
  // 背景、テキスト画像の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");

  // ボタン画像表示位置の情報
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // ボタンのカーソル判定、色
  bool on_mouse;
  Color angle;

  // 収入
  int income;

  // フラグリセット
  button_flag = -1;

  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // 相手の行動開始までのカウント
    if (E_time < 7200) {
      E_time += 1;
    }

    // カーソル座標の取得
    mouse_pos = app_env.mousePosition();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // ボタン表示
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 0, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // コマンドリストの表示
    if (button_flag == -1) {

      // やめる
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

      // アルバイト
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
            // 兵士がいないので実行できません
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W / 2, telop_H * 1,
              status_W * 2, telop_H * 86, telop_W / 2, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else {
            // コマンドの解説
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
              0, telop_H * 10, telop_W, telop_H * 2,
              system, Color(0, 0, 0));

            // クリックで確認画面表示
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();

              // 収入額の計算
              income = P.soldier * 2;

              // 収入を得ると、資金の上限を超える場合
              if (P.money + income > 999999) {
                income = 999999 - P.money;
              }

              // 桁情報を取得
              numberGet(income);

              P.money += income;
              P.fame -= 1;
              button_flag = 0;
            }
          }
        }
      } // command 1 end

      // 執政
      {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 2, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x0, y - (menu_H + 2) * 2, menu_W, menu_H,
          0, menu_H * 7, menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // コマンドの解説
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
            0, telop_H * 13, telop_W, telop_H * 2,
            system, Color(0, 0, 0));

          // クリックで確認画面表示
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();

            // 収入額の計算
            income = P.fund / 10 + P.territory * 100 + (P.fame - P.infamy) * 10;

            // 収入を得ると、資金の上限を超える場合
            if (P.money + income > 999999) {
              income = 999999 - P.money;
            }

            // 桁情報を取得
            numberGet(income);

            P.money += income;
            button_flag = 1;
          }
        }
      } // command 2 end

      // 売却
      {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 3, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x0, y - (menu_H + 2) * 3, menu_W, menu_H,
          0, menu_H * 8, menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // コマンドの解説
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
            0, telop_H * 16, telop_W, telop_H * 2,
            system, Color(0, 0, 0));

          // クリックで確認画面表示
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            item_sale(app_env);
            ex_flag += 1;
            break;
          }
        }
      } // command 3 end
    }

    // コマンド選択したときの処理
    else {
      // 実行内容を表示
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
        0, telop_H * (12 + button_flag * 3), telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // 収入
      drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
        0, telop_H * 9, telop_W, telop_H * 1,
        system, Color(0, 0, 0));

      // 収入額の表示
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

      // マウスクリックでループ終了
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

// 兵力増強
void add_soldier(AppEnv& app_env) {
  // 背景、テキスト画像の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");

  // ボタン画像表示位置の情報
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // ボタンのカーソル判定、色
  bool on_mouse;
  Color angle;

  // フラグ
  short button_flag0 = -1;
  short guara;

  // フラグリセット
  button_flag = -1;

  // ボタンの表示位置指定
  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // 相手の行動開始までのカウント
    if (E_time < 7200) {
      E_time += 1;
    }

    // カーソル座標の取得
    mouse_pos = app_env.mousePosition();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // ボタン表示
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 1, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // コマンドリストの表示
    if (button_flag == -1) {

      // やめる
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

      // ボタンの表示
      for (i = 0; i < 3; i += 1) {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H,
          0, menu_H * (9 + i), menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // コマンドの解説
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

          // クリックで確認画面表示
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            button_flag = i;
          }
        }
      }
    }

    // ボタンが選択されたとき
    else {

      // 人数選択ボタンの表示
      if (button_flag0 == -1) {

        // コマンドの解説
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 29, telop_W, telop_H * 1,
          system, Color(0, 0, 0));

        // 選択されたボタンと解説の表示
        switch (button_flag) {
          case 0:
            // 兵士
            drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
              0, menu_H * 9, menu_W, menu_H,
              button, Color(1, 0.8, 0.4));
            // コマンドの解説
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
              0, telop_H * 30, telop_W, telop_H * 1,
              system, Color(0, 0, 0));

            // 賃金の設定
            guara = 10;
            break;

          case 1:
            // 義勇兵
            drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
              0, menu_H * 10, menu_W, menu_H,
              button, Color(1, 0.8, 0.4));
            // コマンドの解説
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
              0, telop_H * 31, telop_W, telop_H * 1,
              system, Color(0, 0, 0));

            // 賃金の設定
            guara = 5;
            break;

          case 2:
            // 強制兵役
            drawTextureBox(x, y - (menu_H + 2) * 2, menu_W, menu_H,
              0, menu_H * 11, menu_W, menu_H,
              button, Color(1, 0.8, 0.4));

            // 賃金の設定
            guara = 0;
            break;
        }

        // やめる
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

        // 人数ボタンの表示
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
              // これ以上募集できません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                0, telop_H * 33, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (button_flag == 1 && P.mercenary + std::pow(10, (i + 1)) > 65535) {
              // これ以上募集できません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                0, telop_H * 33, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < guara * std::pow(10, (i + 1))) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag0 = i;
              }
            }
          }
        }
      }

      // 人数が確定したとき
      else {

        // 募集しました。
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 32, telop_W, telop_H * 1,
          system, Color(0, 0, 0));

        // マウスクリックでループ終了
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();

          // それぞれの選択内容をパラメータに反映
          switch (button_flag) {

              // 兵士
            case 0:
              P.money -= guara * std::pow(10, (button_flag0 + 1));
              P.soldier += std::pow(10, (button_flag0 + 1));
              break;

              // 義勇兵
            case 1:
              P.money -= guara * std::pow(10, (button_flag0 + 1));
              P.mercenary += std::pow(10, (button_flag0 + 1));
              break;

              // 強制兵役
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

// 計略
void trick(AppEnv& app_env) {
  // 背景、テキスト画像の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");
  Texture telop("res/png/T05_3rdStage.png");

  // ボタン画像表示位置の情報
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // ボタンのカーソル判定、色
  bool on_mouse;
  Color angle;

  // 実行可否のフラグ
  bool cmd = true;

  // 成否判定のフラグ
  bool cmd_ok = false;

  // 乱数
  int r;

  // フラグリセット
  button_flag = -1;

  // ボタンの表示位置指定
  short i;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // 相手の行動開始までのカウント
    if (E_time < 7200) {
      E_time += 1;
    }

    // 乱数生成
    srand((unsigned)time(NULL));

    // カーソル座標の取得
    mouse_pos = app_env.mousePosition();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // ボタン表示
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 2, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // コマンドリストの表示
    if (button_flag == -1) {

      // やめる
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

      // ボタンの表示
      for (i = 0; i < 7; i += 1) {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H);
        cmd = true;

        // ボタンの色変更
        angle = setButtonColor(on_mouse);
        switch (i) {

            // スパイ
          case 0:
            if (spy > 0 || P.money < 100) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // 妨害工作
          case 1:
            if (P.money < 150) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // 風説の流布
          case 2:
            if (rumor || P.money < 100) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // 毒を盛る
          case 3:
            if (P.money < 200) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // 演説
          case 4:
            if (P.money < 50) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // ゴシップ
          case 5:
            if (P.money < 150) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;

            // 反乱
          case 6:
            if (P.money < 200) {
              angle = Color(0.5, 0.5, 0.5);
              cmd = false;
            }
            break;
        }

        // ボタンの表示
        drawTextureBox(x0, y - (menu_H + 2) * (i + 1), menu_W, menu_H,
          0, menu_H * (12 + i), menu_W, menu_H,
          button, angle);

        if (on_mouse) {

          // 実行不可だったとき
          if (!cmd) {

            // スパイ：実行済みか
            if (i == 0 && spy > 0) {
              // すでに実行しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 56, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }

            // 風説の流布：実行済みか
            else if (i == 2 && rumor) {
              // すでに実行しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 56, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }

            // それ以外
            else {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
          }

          // 実行可能だったとき
          else {
            // コマンドの解説
            switch (i) {

              // スパイ
              case 0:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 40, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // 活動資金：100 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 37, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // 妨害工作
              case 1:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 42, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // 活動資金：150 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 38, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // 風説の流布
              case 2:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                  0, telop_H * 44, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));

                // 活動資金：100 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 37, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // 毒を盛る
              case 3:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 45, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // 活動資金：200 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 39, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // 演説
              case 4:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 47, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // 活動資金：50 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 36, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // ゴシップ
              case 5:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 49, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // 活動資金：150 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 38, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;

                // 反乱
              case 6:
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 51, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));

                // 活動資金：200 G
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
                  0, telop_H * 39, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
                break;
            }

            // クリックで確認画面表示
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              button_flag = i;
            }
          }
        } // on_mouse end
      }
    }

    // コマンド選択したときの処理
    else {

      // はい以外を選択でコマンド終了
      if (yes_or_no(app_env, (4 + button_flag)) != 1) {
        return;
      }

      // コマンド実行
      switch (button_flag) {

          // スパイ
        case 0:
          P.money -= 100;
          spy = 10;

          // ３章だったとき
          if (stage_flag == 3)
            _3rdstage_spy_act(app_env);

          break;

          // 妨害工作
        case 1:
          P.money -= 150;
          sabo += 3;

          // スパイ実行中なら効果アップ
          if (spy > 0) {
            sabo += 2;
            spy -= 1;
          }
          break;

          // 風説の流布
        case 2:
          P.money -= 100;

          // 乱数を発生
          r = rand() % 100;

          // スパイ実行中なら成功率アップ（+20%）
          if (spy > 0) {
            r -= 20;
            spy -= 1;
          }

          // 成否判定（75% の確率で成功）
          if (r < 75) {
            cmd_ok = true;
            rumor = true;
            E.fund -= E.fund / 10;
          }
          else {
            cmd_ok = false;
          }
          break;

          // 毒を盛る
        case 3:
          P.money -= 200;

          // 乱数を発生
          r = rand() % 100;

          // スパイ実行中なら成功率アップ（+20%）
          if (spy > 0) {
            r -= 20;
            spy -= 1;
          }

          // 成否判定（75% の確率で成功）
          if (r < 75) {
            cmd_ok = true;
            E.mercenary += (E.soldier / 10) / 10;
            E.soldier -= E.soldier / 10;
          }
          else {
            cmd_ok = false;
          }

          // スパイ実行中ではないとき
          if (spy <= 0) {
            // 乱数を発生
            r = rand() % 100;

            // 成否判定（40% の確率で悪事が発覚）
            if (r < 40) {
              disclos = true;
              P.infamy += 10;
            }
          }
          break;

          // 演説
        case 4:
          P.money -= 50;

          // 乱数を発生
          r = rand() % 100;

          // 成否判定（85% の確率で成功）
          if (r < 85) {
            cmd_ok = true;

            // 悪名が０
            if (P.infamy == 0) {
              P.fame += 5;
            }

            // ０ではないとき
            else {
              P.fame += 2;
              P.infamy -= 2;
            }
          }
          else {
            cmd_ok = false;
          }
          break;

          // ゴシップ
        case 5:
          P.money -= 150;

          // 乱数を発生
          r = rand() % 100;

          // スパイ実行中なら成功率アップ（+20%）
          if (spy > 0) {
            r -= 20;
            spy -= 1;
          }

          // 成否判定（75% の確率で成功）
          if (r < 75) {
            cmd_ok = true;
            E.infamy += 10;
          }
          else {
            cmd_ok = false;
            P.infamy += 5;
          }
          break;

          // 反乱
        case 6:
          P.money -= 200;

          // ３章だったとき
          if (stage_flag == 3) {

            // 「立派なかんむり」を所持しているか
            if (P_item[3] != 0) {
              cmd_ok = true;
              rebel = true;
            }
            else {
              cmd_ok = false;
            }
          }

          // ３章以外
          else {

            // 乱数を発生
            r = rand() % 100;

            // スパイ実行中なら成功率アップ（+20%）
            if (spy > 0) {
              r -= 20;
              spy -= 1;
            }

            // 成否判定（相手の人望が低いほど成功率アップ）
            if (r < (E.fame - E.infamy)) {
              cmd_ok = true;
            }
            else {
              cmd_ok = false;
            }

            // 反乱発動が確定したとき
            if (cmd_ok) {
              // 乱数を発生
              r = rand() % 100;

              // 成否判定（相手の人望が低く、プレイヤーの人望が高いほど成功率アップ）
              if (r < (E.fame - E.infamy) + (P.fame - P.infamy) / 2) {
                rebel = true;
              }

              // 反乱が成功しなかったとき
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

        // マウスクリックでループ終了
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          disclos = false;
          ex_flag += 1;
          return;
        }

        app_env.setupDraw();

        // 背景表示
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));

        // 実行結果の表示
        switch (button_flag) {

            // スパイ
          case 0:
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
              0, telop_H * 58, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
            break;

            // 妨害工作
          case 1:
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
              0, telop_H * 60, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
            break;

            // 風説の流布
          case 2:
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
              0, telop_H * 62, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
            break;

            // 毒を盛る
          case 3:
            // 成功
            if (cmd_ok) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 64, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            // 失敗
            else {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 55, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }

            // 悪事が発覚
            if (disclos) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
                0, telop_H * 65, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            break;

            // 演説
          case 4:
            // 成功
            if (cmd_ok) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 54, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            // 失敗
            else {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 55, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            break;

            // ゴシップ
          case 5:
            // 成功
            if (cmd_ok) {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 54, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            // 失敗
            else {
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                0, telop_H * 55, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            break;

            // 反乱
          case 6:
            // 成功
            if (cmd_ok) {
              // 反乱成立
              if (rebel) {
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                  0, telop_H * 69, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              // 反乱不成立
              else {
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                  0, telop_H * 70, telop_W, telop_H * 2,
                  system, Color(0, 0, 0));
              }
            }
            // 失敗
            else {
              // ３章
              if (stage_flag == 3) {
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 4,
                  0, telop_H * 38, telop_W, telop_H * 4,
                  telop, Color(0, 0, 0));
              }
              // ３章以外
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

// 買収
void buy_fund(AppEnv& app_env) {
  // 背景、テキスト画像の読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture b_list("res/png/Me2_BuyList.png");
  Texture system("res/png/Me4_System.png");
  Texture buytxt("res/png/Me6_BuyTxt.png");

  // ボタン画像表示位置の情報
  float x = (-Window::WIDTH / 2) + 2;
  float y = (Window::HEIGHT / 2);
  float x0 = x + (menu_W + 2);

  // ボタンのカーソル判定、色
  bool on_mouse;
  Color angle;

  // 買取の成否判定
  bool buy_flag = true;

  // フラグリセット
  button_flag = -1;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // 相手の行動開始までのカウント
    if (E_time < 7200) {
      E_time += 1;
    }

    // カーソル座標の取得
    mouse_pos = app_env.mousePosition();

    // 背景表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // ボタン表示
    drawTextureBox(x, y - (menu_H + 2) * 1, menu_W, menu_H,
      0, menu_H * 3, menu_W, menu_H,
      button, Color(1, 0.8, 0.4));

    // どれを買取りますか？
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
      0, telop_H * 75, telop_W, telop_H * 1,
      system, Color(0, 0, 0));

    // コマンドリストの表示
    if (button_flag == -1) {

      // やめる
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

      // 領地
      if (stage_flag < 5) {
        on_mouse = on_button(mouse_pos, x0, y - (menu_H + 2) * 1, fund_W, menu_H);
        angle = setButtonColor(on_mouse);
        buy_flag = true;

        // ステージごとの買取価格
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
            // 買い取れません
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 80, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else if (!buy_flag) {
            // 資金が足りません
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 77, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else {
            // コマンドの解説
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 79, telop_W, telop_H * 1,
              system, Color(0, 0, 0));

            // クリックで確認画面表示
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              button_flag = 14;
            }
          }
        }
      } // button 14 end

      // １章だけ
      if (stage_flag == 1) {
        // 王子の城
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
            // すでに所有しています
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 76, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else if (P.money < fundinfo[0]) {
            // 資金が足りません
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
              0, telop_H * 77, telop_W, telop_H * 1,
              system, Color(0, 0, 0));
          }
          else {
            // コマンドの解説
            drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
              0, telop_H * 0, telop_W, telop_H * 2,
              buytxt, Color(0, 0, 0));

            // クリックで確認画面表示
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              button_flag = 0;
            }
          }
        }
      } // button 1 end

      // １章から３章の間
      if (stage_flag > 0 && stage_flag < 4) {
        // 農地
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[1]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 2, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 1;
              }
            }
          }
        } // button 1 end

        // 牧場
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[2]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 4, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 2;
              }
            }
          }
        } // button 2 end
      }

      // ２章から
      if (stage_flag > 1) {
        // ３章まで
        if (stage_flag < 4) {
          // 農場＋１
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
                // すでに所有しています
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 76, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else if (P.money < fundinfo[3]) {
                // 資金が足りません
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 77, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else {
                // コマンドの解説
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                  0, telop_H * 6, telop_W, telop_H * 2,
                  buytxt, Color(0, 0, 0));

                // クリックで確認画面表示
                if (app_env.isPushButton(Mouse::LEFT)) {
                  app_env.flushInput();
                  button_flag = 3;
                }
              }
            }
          } // button 3 end

          // 牧場＋１
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
                // すでに所有しています
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 76, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else if (P.money < fundinfo[4]) {
                // 資金が足りません
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                  0, telop_H * 77, telop_W, telop_H * 1,
                  system, Color(0, 0, 0));
              }
              else {
                // コマンドの解説
                drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                  0, telop_H * 8, telop_W, telop_H * 2,
                  buytxt, Color(0, 0, 0));

                // クリックで確認画面表示
                if (app_env.isPushButton(Mouse::LEFT)) {
                  app_env.flushInput();
                  button_flag = 4;
                }
              }
            }
          } // button 4 end
        }

        // 工場
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[5]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 10, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 5;
              }
            }
          }
        } // button 5 end
      }

      // ３章から
      if (stage_flag > 2) {
        // 鉄の鉱山
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[6]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 12, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 6;
              }
            }
          }
        } // button 6 end
        
        // 銀の鉱山
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[7]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 14, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 7;
              }
            }
          }
        } // button 7 end

        // 金の鉱山
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[8]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 16, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 8;
              }
            }
          }
        } // button 8 end

        // 美術館
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[9]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 18, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 9;
              }
            }
          }
        } // button 9 end
      }

      // ４章から
      if (stage_flag > 3) {

        // 良質な鉄鉱山
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[10]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 20, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 10;
              }
            }
          }
        } // button 10 end

        // 武器の鍛冶屋
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[11]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 22, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 11;
              }
            }
          }
        } // button 11 end

        // 防具の鍛冶屋
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[12]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 22, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 12;
              }
            }
          }
        } // button 12 end

        // 店
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
              // すでに所有しています
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 76, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else if (P.money < fundinfo[13]) {
              // 資金が足りません
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 3, telop_W, telop_H * 1,
                0, telop_H * 77, telop_W, telop_H * 1,
                system, Color(0, 0, 0));
            }
            else {
              // コマンドの解説
              drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 2,
                0, telop_H * 24, telop_W, telop_H * 2,
                buytxt, Color(0, 0, 0));

              // クリックで確認画面表示
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                button_flag = 13;
              }
            }
          }
        } // button 13 end
      }
    }

    // コマンド選択したときの処理
    else {
      // 確認画面を表示
      if (yes_or_no(app_env, 11) != 1) {
        if (!app_env.isOpen()) return;
        return;
      }

      // 買収成立の処理
      if (button_flag != 14) {
        P_fund[button_flag] = 1;
        P.money -= fundinfo[button_flag];
        E.money += fundinfo[button_flag];
        P.fund += fundinfo[button_flag];
        E.fund -= fundinfo[button_flag];
      }

      // 領地の買収処理
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

        // マウスクリックでループ終了
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          ex_flag += 1;
          return;
        }

        app_env.setupDraw();

        // 背景表示
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));

        // 買い取りました
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 82, telop_W, telop_H * 1,
          system, Color(0, 0, 0));

        app_env.update();
      }
    }

    app_env.update();
  }
}

// 侵攻
void duel(AppEnv& app_env) {
  // 背景、テキスト、キャラクターの読み込み
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture button("res/png/Me1_MenuList.png");
  Texture system("res/png/Me4_System.png");
  Texture battle("res/png/Me5_Battle.png");
  Texture PowerG("res/png/Me9_Gauge.png");
  Texture chara6("res/png/06_Rival_C.png");

  // ＢＧＭ、ＳＥの読み込み
  // 通常戦
  Media bgm_a = setBGM_a(stage_flag);
  bgm_a.looping(true);
  bgm_a.gain(0.2);

  // 追撃戦
  Media bgm_b = setBGM_b(stage_flag);
  bgm_b.looping(true);
  bgm_b.gain(0.2);

  // 戦闘開始
  Media se_start("res/wav/se_battle.wav");
  se_start.gain(0.2);

  // 勝利：アイテム獲得
  Media se_get("res/wav/se_itemget.wav");
  se_get.gain(0.2);

  // 勝利
  Media se_win("res/wav/se_win.wav");
  se_win.gain(0.2);

  // 敗北
  Media se_lose("res/wav/se_lose.wav");
  se_lose.gain(0.2);

  // 撤退
  Media se_away("res/wav/se_away.wav");
  se_away.gain(0.2);

  // ボタン判定、色
  bool on_mouse;
  Color angle;

  // 画像表示用
  short i = 0, j;         // キャラクターカウンタ
  float x, y;             // ボタン座標用
  float x0, y0;   // キャラクター表示用

  // 行動回数設定
  srand((unsigned)time(NULL));
  short count = (rand() % 3) + 3;

  // アイテム取得フラグ
  bool item_get = false;

  // 戦闘前の処理
  {
    while (i < 90) {
      if (!app_env.isOpen()) return;

      // 戦闘開始ＳＥ再生
      if (i == 0) se_start.play();

      app_env.setupDraw();
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
      // 戦闘開始
      if (!E_action) {
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 0, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));
      }
      // 相手に攻め込まれた
      else {
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 100, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
      }
      app_env.update();

      i += 1;
    }

    // ボタンの表示位置指定
    {
      x = (-Window::WIDTH / 2) + 2;
      y = (Window::HEIGHT / 2) - 358;
    }

    // 攻撃力基準値と兵力初期値を取得
    duel_standby();
  }

  // フラグリセット
  button_flag = -1;

  // 戦闘処理
  while (1) {
    if (!app_env.isOpen()) return;

    // 戦闘ＢＧＭ再生
    if (!add_attack) {
      if (!bgm_a.isPlaying()) bgm_a.play();
    }
    else {
      if (!bgm_b.isPlaying()) bgm_b.play();
    }

    // 戦闘終了条件確認
    {
      // 兵力が０
      if (P_pow <= 0 || E_pow <= 0) {
        bgm_a.stop();
        bgm_b.stop();
        break;
      }

      // 行動回数が０
      if (count == 0) {
        // 追撃戦で行動したとき
        if (add_attack) {
          button_flag = 0;
          bgm_b.stop();
          break;
        }

        button_flag = -1;
        // 追撃戦突入確認
        while (1) {
          if (!app_env.isOpen()) return;

          app_env.setupDraw();

          // マウス座標を取得
          mouse_pos = app_env.mousePosition();

          // 背景表示
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            mainBG, Color(1, 1, 1));

          // 相手は撤退するようだ
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
            0, telop_H * 1, telop_W, telop_H * 1,
            battle, Color(0, 0, 0));

          // ボタン表示
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

                  // 追撃戦突入
                  if (i == 0) {
                    add_attack = true;
                    bgm_a.stop();
                    button_flag = 0;
                    break;
                  }
                  // 追撃しない
                  else {
                    button_flag = 5;
                    break;
                  }
                }
              }
            }
          }

          // ボタンが選択されたとき
          if (button_flag != -1) {
            break;
          }

          app_env.update();
        }

        // 追撃しないとき
        if (!add_attack) {
          bgm_a.stop();
          break;
        }
        // 追撃戦突入
        else {
          button_flag = -1;
          count = 1;
        }
      } // count 0 end
    }

    app_env.setupDraw();

    // マウス座標を取得
    mouse_pos = app_env.mousePosition();

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

    // 兵力ゲージの計算と表示
    {
      // どちらのゲージか表示
      // プレイヤー側
      drawTextureBox((-Window::WIDTH / 2) + 20, (Window::HEIGHT / 2) - (34 + telop_H), status_W * 2, telop_H,
        0, telop_H * 23, status_W * 2, telop_H,
        battle, Color(0, 0, 0));
      // 相手側
      drawTextureBox((Window::WIDTH / 2) - status_W * 2, (Window::HEIGHT / 2) - (34 + telop_H), status_W * 2, telop_H,
        0, telop_H * 24, status_W * 2, telop_H,
        battle, Color(0, 0, 0));

      // ゲージの計算とカラー設定
      setPowGauge();
      P_GaugeColor = setGaugeColor(P_powGauge);
      E_GaugeColor = setGaugeColor(E_powGauge);

      // プレイヤー側ゲージの表示
      {
        // 兵力
        // ゲージ裏側の表示（黒）
        drawTextureBox((-Window::WIDTH / 2) + 20, (Window::HEIGHT / 2) - 32, 200, 32,
          0, 32, 200, 32,
          PowerG, Color(0, 0, 0));
        // 現在の兵力ゲージ
        drawTextureBox((-Window::WIDTH / 2) + 20 + (200 - P_powGauge * 2), (Window::HEIGHT / 2) - 32, P_powGauge * 2, 32,
          0, 32, P_powGauge * 2, 32,
          PowerG, P_GaugeColor);

        // 義勇兵
        // ゲージ裏側の表示（黒）
        drawTextureBox((-Window::WIDTH / 2) + 180, (Window::HEIGHT / 2) - 32, 50, 32,
          400, 32, 50, 32,
          PowerG, Color(0, 0, 0));
        // 現在の義勇兵ゲージ
        drawTextureBox((-Window::WIDTH / 2) + 180 + (50 - P_Mgauge), (Window::HEIGHT / 2) - 32, P_Mgauge, 32,
          400, 32, P_Mgauge, 32,
          PowerG, Color(1, 0.5, 1));
      }

      // 相手側ゲージの表示
      {
        // 兵力
        // ゲージ裏側の表示（黒）
        drawTextureBox((Window::WIDTH / 2) - 220, (Window::HEIGHT / 2) - 32, 200, 32,
          200, 32, 200, 32,
          PowerG, Color(0, 0, 0));
        // 現在の兵力ゲージ
        drawTextureBox((Window::WIDTH / 2) - 220, (Window::HEIGHT / 2) - 32, E_powGauge * 2, 32,
          200 + (100 - E_powGauge) * 2, 32, E_powGauge * 2, 32,
          PowerG, E_GaugeColor);

        // 義勇兵
        // ゲージ裏側の表示（黒）
        drawTextureBox((Window::WIDTH / 2) - 230, (Window::HEIGHT / 2) - 32, 50, 32,
          450, 32, 50, 32,
          PowerG, Color(0, 0, 0));
        // 現在の義勇兵ゲージ
        drawTextureBox((Window::WIDTH / 2) - 230, (Window::HEIGHT / 2) - 32, E_Mgauge, 32,
          450 + (50 - E_Mgauge), 32, E_Mgauge, 32,
          PowerG, Color(1, 0.5, 1));
      }

      // フレームの表示
      drawTextureBox((-Window::WIDTH / 2), (Window::HEIGHT / 2) - 32, Window::WIDTH, 32,
        0, 0, Window::WIDTH, 32,
        PowerG, Color(1, 1, 1));
    }

    // 残り行動回数の表示
    if (!add_attack) {
      drawTextureBox((-number_W / 2), (Window::HEIGHT / 2) - 28, number_W, telop_H,
        number_W * count, telop_H * 22, number_W, telop_H,
        battle, Color(1, 1, 1));
    }

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

    // ボタン表示
    {
      // 撤退
      if (!add_attack) {
        on_mouse = on_button(mouse_pos, x + (menu_W + 2) * 1, y, menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x + (menu_W + 2) * 1, y, menu_W, menu_H,
          0, menu_H * 26, menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // コマンド解説
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

      // 突撃、守り、ゆみ
      for (i = 0; i < 3; i += 1) {
        on_mouse = on_button(mouse_pos, x, y + (menu_H + 2) * (2 - i), menu_W, menu_H);
        angle = setButtonColor(on_mouse);
        drawTextureBox(x, y + (menu_H + 2) * (2 - i), menu_W, menu_H,
          0, menu_H * (22 + i), menu_W, menu_H,
          button, angle);

        if (on_mouse) {
          // コマンドの解説
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
            0, telop_H * (3 + i), telop_W, telop_H * 1,
            battle, Color(0, 0, 0));

          // クリックで確認画面表示
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            button_flag = i;
          }
        }
      }
    }

    // 戦闘実行
    if (button_flag != -1) {
      duel_turnStart();
      E_command = duel_enemyCommand();
      duel_fight(button_flag, E_command);
      duel_anime(app_env, button_flag, E_command);
      duel_turnEnd();

      // カウンタ、フラグのリセット
      button_flag = -1;
      count -= 1;
    }

    app_env.update();
  }

  // 戦闘終了後の処理
  {
    // 撤退以外
    if (button_flag != 4) {
      // プレイヤーと相手の残り兵力差を確認
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

        // 相手の兵力が０
        if ((E.soldier + E.mercenary) == 0) {
          P.territory += 1;
          E.territory -= 1;
          P.fame += 2;
          enemy_reaction();
        }

        // アイテム獲得判定
        switch (stage_flag) {

            // １章
          case 1:
            // 相手兵力の 25% 以上のダメージを与えた
            if ((E_pow * 100) / E_pow0 <= 75) {
              item_get = true;
              P_item[1] = 1;
            }
            break;

            // ２章
          case 2:
            // 相手兵力の 50% 以上のダメージを与えた
            if ((E_pow * 100) / E_pow0 <= 50) {
              item_get = true;
              P_item[2] = 1;
            }
            break;

            // ３章
          case 3:
            // 相手に 10000 以上のダメージを与えた
            if (P_item[3] != 1 && (E_pow0 - E_pow) >= 10000) {
              item_get = true;
              P_item[3] = 1;
            }
            // 相手兵力の 25% 以上のダメージを与えた
            else if ((E_pow * 100) / E_pow0 <= 75) {
              item_get = true;
              P_item[4] = 1;
            }
            break;

            // ４章
          case 4:
            // 相手兵力の 50% 以上のダメージを与えた
            if ((E_pow * 100) / E_pow0 <= 50) {
              item_get = true;
              P_item[6] = 1;
            }
            // 相手兵力の 25% 以上のダメージを与えた
            else if ((E_pow * 100) / E_pow0 <= 75) {
              item_get = true;
              P_item[5] = 1;
            }
            break;

            // ５章
          case 5:
            // 相手兵力の 50% 以上のダメージを与えた
            if ((E_pow * 100) / E_pow0 <= 50) {
              item_get = true;
              P_item[5] = 1;
            }
            // 相手兵力の 40% 以上のダメージを与えた
            else if ((E_pow * 100) / E_pow0 <= 60) {
              item_get = true;
              P_item[4] = 1;
            }
            // 相手兵力の 30% 以上のダメージを与えた
            else if ((E_pow * 100) / E_pow0 <= 70) {
              item_get = true;
              P_item[2] = 1;
            }
            // 相手兵力の 20% 以上のダメージを与えた
            else if ((E_pow * 100) / E_pow0 <= 80) {
              item_get = true;
              P_item[1] = 1;
            }
            // 相手兵力の 10% 以上のダメージを与えた
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

        // プレイヤーの領地が０ではない
        if (P.territory != 0) {
          P.territory -= 1;
          E.territory += 1;
          P.fame -= 1;
        }
      }

      // 勝利時のＳＥ再生
      if (button_flag == 0) {
        if (item_get)
          se_get.play();
        else
          se_win.play();
      }
    }

    // 撤退
    else {
      se_away.play();
    }
  }

  // 戦闘終了画面の表示
  while (1) {
    if (!app_env.isOpen()) return;

    // クリックでループ終了
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      P.mercenary = 0;
      E.mercenary = 0;
      add_attack = false;
      ex_flag += 1;
      break;
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

    // 戦闘結果テキスト
    switch (button_flag) {

        // 勝利
      case 0:
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 9, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));

        // アイテム取得
        if (item_get) {
          drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
            0, telop_H * 10, telop_W, telop_H * 1,
            battle, Color(0, 0, 0));
        }
        break;

        // 敗北
      case 1:
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 11, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));
        break;

        // 撤退
      case 4:
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
          0, telop_H * 8, telop_W, telop_H * 1,
          battle, Color(0, 0, 0));
        break;
    }

    app_env.update();
  }
}

// セーブ
void save(AppEnv& app_env) {
  // 背景、テキスト画像の読み込み
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

    // 左クリックでループ終了
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      break;
    }

    app_env.setupDraw();

    // 背景の表示
    drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    // テキスト表示
    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
      0, telop_H * 6, telop_W, telop_H * 1,
      system, Color(0, 0, 0));

    app_env.update();
  }
}


// ゲーム本体
////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  AppEnv app_env(Window::WIDTH, Window::HEIGHT);

  // 画像の読み込み
  Texture blackBG("res/png/BG5_Black.png");
  Texture mainBG("res/png/BG1_GrassField.png");
  Texture event1("res/png/BG2_Castle_in.png");
  Texture event2("res/png/BG3_Castle_out.png");
  Texture system("res/png/Me4_System.png");
  Texture button("res/png/Me1_MenuList.png");

  // ボタンの表示位置、カーソル判定、色
  float x, y;
  bool on_mouse;
  Color angle;

  // タイトル //--------------------------------------------------------------------------------
  {
    // 開始画面の画像読み込み
    Texture title_bg("res/png/Sp3_TitleBack.png");
    Texture title_OP("res/png/Sp4_Opening.png");
    Texture title_LG("res/png/Sp1_TitleLogo_b.png");
    Texture select("res/png/Sp2_Select.png");

    // ＢＧＭの設定
    Media BGM("res/wav/Event_Opening.wav");
    BGM.looping(true);
    BGM.gain(0.2);
    BGM.play();

    // ボタンのサイズ
    short select_w = 208, select_h = 56;

    // タイトルのサイズ
    short title_h = 64;

    while (1) {
      if (!app_env.isOpen()) return 0;
      app_env.setupDraw();

      // カーソル座標の取得
      mouse_pos = app_env.mousePosition();

      // 背景表示
      drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        title_bg, Color(1, 1, 1));

      // タイトル表示
      drawTextureBox((-Window::WIDTH / 2), (Window::HEIGHT / 4), Window::WIDTH, title_h,
        0, 0, Window::WIDTH, title_h,
        title_LG, Color(1, 1, 0.5));

      // ボタン表示
      {
        // Continue
        {
          // ボタン画像の表示座標設定とマウス判定による画像カラーの設定
          x = (-select_w / 2);
          y = (-Window::HEIGHT / 6);
          on_mouse = on_button(mouse_pos, x, y, select_w, select_h);
          angle = setButtonColor(on_mouse);

          drawTextureBox(x, y, select_w, select_h,
            0, select_h * 1, select_w, select_h,
            select, angle);

          // マウスクリックの可否とクリックしたときの処理
          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();

              // セーブデータの確認
              std::ifstream save("res/Savedata.txt");

              // セーブデータがある場合
              if (save) {
                // セーブデータの読み込み確認
                button_flag = yes_or_no(app_env, 0);

                // 処理が異常終了したとき
                if (button_flag == 0) return 0;

                // セーブデータを正しく読み込めたとき
                if (button_flag == 1) stage_flag = 6;
              }

              // セーブデータがなかった場合
              else {
                while (1) {
                  if (!app_env.isOpen()) return 0;

                  // マウスクリックでタイトルに戻る
                  if (app_env.isPushButton(Mouse::LEFT)) {
                    app_env.flushInput();
                    break;
                  }

                  app_env.setupDraw();

                  // 背景の表示
                  drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
                    0, 0, Window::WIDTH, Window::HEIGHT,
                    title_bg, Color(1, 1, 1));
                  drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
                    0, 365, Window::WIDTH, Window::HEIGHT - 365,
                    event1, Color(1, 1, 1));

                  // エラーメッセージの表示
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
          // ボタン画像の表示座標設定とマウス判定による画像カラーの設定
          x = (-select_w / 2);
          y = (-Window::HEIGHT / 3);
          on_mouse = on_button(mouse_pos, x, y, select_w, select_h);
          angle = setButtonColor(on_mouse);

          drawTextureBox(x, y, select_w, select_h,
            0, select_h * 0, select_w, select_h,
            select, angle);

          // マウスクリックの可否とクリックしたときの処理
          if (on_mouse) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();

              // オープニング処理
              _BG_slide(app_env, title_bg, title_OP, Color(1, 1, 1), Color(1, 1, 1));
              _BG_fade(app_env, title_OP, 1.0, 0.5, 0.02);
              _Opening(app_env);
              _BG_fade(app_env, title_OP, 0.5, 0, 0.02);

              button_flag = 1;
            }
          }
        }
      } // button end

      // どちらかが正常に処理されたら本編開始
      if (button_flag == 1) {
        BGM.stop();
        break;
      }

      app_env.update();
    }
  } // title Loop end

  // 画像の読み込み
  Texture titleEP("res/png/Sp6_BG_Title.png");
  Texture s_window("res/png/Me7_StatusWindow.png");
  Texture target("res/png/Me8_target.png");

  // ゲーム本編 //--------------------------------------------------------------------------------
  while (1) {
    if (!app_env.isOpen()) return 0;

    // ストーリー //--------------------------------------------------------------------------------
    {
      // テロップ表示の設定
      switch (stage_flag) {

          // プロローグ
        case 0:
          // パラメータの初期化
          P = {500, 10, 0, 10, 0, 0, 0};
          E = {0, 0, 0, 10, 0, 0, 0};

          // イベント開始
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _Prologue_event(app_env);
          break;

          // １章
        case 1:
          // パラメータの初期化
          E = {2000, 5000, 5, 100, 0, 0, 0};

          // イベント開始
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _1ststage_event(app_env);
          break;

          // ２章
        case 2:
          // パラメータの初期化
          E = {5000, 15000, 5, 200, 100, 0, 0};

          // イベント開始
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _2ndstage_event(app_env);
          break;

          // ３章
        case 3:
          // パラメータの初期化
          E = {300000, 200000, 10, 20000, 0, 0, 80};

          // イベント開始
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _3rdstage_event(app_env);
          break;

          // ４章
        case 4:
          // パラメータの初期化
          E = {150000, 300000, 10, 15000, 1000, 80, 0};

          // イベント開始
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _4thstage_event(app_env);
          _BG_slide(app_env, event1, mainBG, Color(1, 1, 1), Color(1, 1, 1));
          break;

          // ５章
        case 5:
          // パラメータの初期化
          E = {300000, 300000, 10, 20000, 0, 80, 0};

          // イベント開始
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, event1, Color(1, 1, 1), Color(1, 1, 1));
          _5thstage_event(app_env);
          _BG_slide(app_env, event2, mainBG, Color(1, 1, 1), Color(1, 1, 1));
          break;

          // データ読み込み時の処理
        default:
          // セーブデータの内容をゲームに反映
          std::ifstream save("res/Savedata.txt");
          save >> stage_flag
            >> ex_flag

            // 相手の情報を取得
            >> E.money
            >> E.fund
            >> E.territory
            >> E.soldier
            >> E.mercenary
            >> E.fame
            >> E.infamy

            // プレイヤーの情報を取得
            >> P.money
            >> P.fund
            >> P.territory
            >> P.soldier
            >> P.mercenary
            >> P.fame
            >> P.infamy

            // 所有アイテム情報を取得
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

            // 売却可能なアイテム情報を取得
            >> P_item[0]
            >> P_item[1]
            >> P_item[2]
            >> P_item[3]
            >> P_item[4]
            >> P_item[5]
            >> P_item[6];

          // 読み込み後のステージタイトルを表示
          _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
          _titleCall(app_env);
          _BG_slide(app_env, titleEP, mainBG, Color(1, 1, 1), Color(1, 1, 1));
          break;
      }
    } // story event end

    // メインループ //--------------------------------------------------------------------------------
    {
      // ＢＧＭの設定
      Media bgm_main = setBGM_Main(stage_flag);
      bgm_main.looping(true);
      bgm_main.gain(0.2);

      // 画像表示用カウンタ
      short i = 0;

      // 各種フラグのリセット
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

        // ＢＧＭ再生
        if (!bgm_main.isPlaying()) bgm_main.play();

        // 勝利条件を満たしているか確認
        {
          // プロローグ
          if (stage_flag == 0) {
            bgm_main.stop();
            _tutorial(app_env);
            stage_clear = true;
          }

          // 相手の領地をすべて占領した
          if (E.territory <= 0) {
            stage_clear = true;
          }

          // 反乱成功
          if (rebel) {
            // ３章のとき
            if (stage_flag == 3) {
              bgm_main.stop();
              _BG_slide(app_env, mainBG, event1, Color(1, 1, 1), Color(1, 1, 1));
              _3rdstage_clear(app_env);
              _BG_fade(app_env, event1, 1.0, 0, 0.02);
            }
            stage_clear = true;
          }

          // 目的の物件を買収した
          if (stage_flag == 1 && P_fund[0] == 1) {
            stage_clear = true;
          }
          else if (stage_flag == 2) {
            if (P_fund[3] == 1 && P_fund[4] == 1 && P_fund[5] == 1) {
              stage_clear = true;
            }
          }

          // ４章：アイテムを売却した
          if (stage_flag == 4 && P_item[6] == 2) {
            bgm_main.stop();
            _BG_slide(app_env, mainBG, event1, Color(1, 1, 1), Color(1, 1, 1));
            _4thstage_clear(app_env);
            _BG_fade(app_env, event1, 1.0, 0, 0.02);
            stage_clear = true;
          }
        }

        // 勝利条件達成ならループ終了
        if (stage_clear) {
          bgm_main.stop();
          break;
        }

        // パラメータが限界値を超えてないか確認
        {
          // プレイヤー
          {
            // 資金
            if (P.money > 999999) P.money = 999999;
            else if (P.money < 0) P.money = 0;

            // 総資産額
            if (P.fund > 999999) P.fund = 999999;
            else if (P.fund < 0) P.fund = 0;

            // 領地
            if (P.territory > 100) P.territory = 100;
            else if (P.territory < 0) P.territory = 0;

            // 兵士
            if (P.soldier > 65535) P.soldier = 65535;
            else if (P.soldier < 0) P.soldier = 0;

            // 義勇兵
            if (P.mercenary > 65535) P.mercenary = 65535;
            else if (P.mercenary < 0) P.mercenary = 0;

            // 名声
            if (P.fame > 100) P.fame = 100;
            else if (P.fame < 0) P.fame = 0;

            // 悪名
            if (P.infamy > 100) P.infamy = 100;
            else if (P.infamy < 0) P.infamy = 0;
          }

          // 相手
          {
            // 資金
            if (E.money > 999999) E.money = 999999;
            else if (E.money < 0) E.money = 0;

            // 総資産額
            if (E.fund > 999999) E.fund = 999999;
            else if (E.fund < 0) E.fund = 0;

            // 領地
            if (E.territory > 100) E.territory = 100;
            else if (E.territory < 0) E.territory = 0;

            // 兵士
            if (E.soldier > 65535) E.soldier = 65535;
            else if (E.soldier < 0) E.soldier = 0;

            // 義勇兵
            if (E.mercenary > 65535) E.mercenary = 65535;
            else if (E.mercenary < 0) E.mercenary = 0;

            // 名声
            if (E.fame > 100) E.fame = 100;
            else if (E.fame < 0) E.fame = 0;

            // 悪名
            if (E.infamy > 100) E.infamy = 100;
            else if (E.infamy < 0) E.infamy = 0;
          }
        } // status limit check end

        // 相手の行動開始までのカウント
        if (E_time < 7200) {
          E_time += 1;
        }
        // 相手の行動
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

              // 兵力差で負けているとき
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

          // タイムバーのリセット
          E_time = 0;
        }

        // タイムバーの長さと色設定
        if (E_time < 7200) {
          E_timeGauge = (7200 - E_time) / 60;
          timeColor = setTimeColor(E_timeGauge);
        }

        app_env.setupDraw();

        // カーソル座標の取得
        mouse_pos = app_env.mousePosition();

        // 背景の表示
        drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          mainBG, Color(1, 1, 1));

        // タイムバーの表示
        drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 2, telop_W, telop_H * 1,
          0, telop_H * 99, telop_W, telop_H * 1,
          system, Color(0, 0, 0));
        drawFillBox(-30, (-Window::HEIGHT / 2) + telop_H * 2, E_timeGauge * 2, 20, timeColor);

        // パラメータウィンドウの表示
        drawTextureBox(0, (-Window::HEIGHT / 2), (Window::WIDTH / 2), Window::HEIGHT,
          0, 0, (Window::WIDTH / 2), Window::HEIGHT,
          s_window, Color(1, 1, 1));

        // 勝利条件の表示
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

        // パラメータの表示
        {
          // プレイヤー
          {
            // 情報
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 2, status_W * 2, telop_H,
              0, telop_H * 88, status_W * 2, telop_H,
              system, Color(0, 0.4, 0.8));

            // 資金
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

            // 兵士
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

            // 義兵（義勇兵）
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

            // 名声
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

            // 悪名
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

          // 相手
          if (spy > 0) {
            // 情報
            drawTextureBox(number_W, (Window::HEIGHT / 2) - telop_H * 9, status_W * 2, telop_H,
              0, telop_H * 89, status_W * 2, telop_H,
              system, Color(0.8, 0.2, 0.2));

            // 資金
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

            // 兵士
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

            // 領地
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

        // ボタンの表示
        {
          // ボタンの表示位置設定
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
              // コマンド解説
              switch (i) {

                  // 資金繰り
                case 0:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 8, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // 兵力増強
                case 1:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 23, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // 計略
                case 2:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 35, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // 買収
                case 3:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 74, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));
                  break;

                  // 侵攻
                case 4:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 5, telop_W, telop_H * 1,
                    0, telop_H * 84, telop_W, telop_H * 1,
                    system, Color(0, 0, 0));

                  // 兵力０なら実行不可
                  if (P.soldier + P.mercenary == 0) {
                    drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 1,
                      0, telop_H * 86, telop_W, telop_H * 1,
                      system, Color(0, 0, 0));
                  }
                  break;

                  // セーブ
                case 5:
                  drawTextureBox((-telop_W / 2), (-Window::HEIGHT / 2) + telop_H * 4, telop_W, telop_H * 2,
                    0, telop_H * 3, telop_W, telop_H * 2,
                    system, Color(0, 0, 0));
                  break;
              }
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();

                // コマンド実行開始
                switch (i) {

                    // 資金繰り
                  case 0:
                    add_money(app_env);
                    break;

                    // 兵力増強
                  case 1:
                    add_soldier(app_env);
                    break;

                    // 計略
                  case 2:
                    trick(app_env);
                    break;

                    // 買収
                  case 3:
                    buy_fund(app_env);
                    break;

                    // 侵攻
                  case 4:
                    // 兵力０なら実行不可
                    if ((P.soldier + P.mercenary) != 0) {
                      // はいを選択すると戦闘開始
                      if (yes_or_no(app_env, 2) == 1) {
                        bgm_main.stop();
                        duel(app_env);
                      }
                    }
                    break;

                    // セーブ
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

    // クリア画面の表示 //--------------------------------------------------------------------------------
    {
      // ５章クリア
      if (stage_flag == 5) {
        _5thstage_clear(app_env);
      }

      _BG_slide(app_env, blackBG, titleEP, Color(1, 1, 1), Color(1, 1, 1));
      _titleCall(app_env);
      _BG_fade(app_env, titleEP, 1.0, 0, 0.02);
      stage_clear = false;
      stage_flag += 1;

      // １章クリア時のみ、５章突入フラグを一度リセット
      if (stage_flag == 2) {
        ex_flag = 0;
      }

      // エンディング判定
      // ５章クリア
      if (stage_flag == 6) {
        _Ending(app_env);
      }
      // ４章クリア時点で、行動回数が 100回を超えているとき
      else if (stage_flag == 5 && ex_flag > 100) {
        _Ending(app_env);
      }

      // クリアボーナスの反映
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

