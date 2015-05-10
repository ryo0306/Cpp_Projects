//
// Revival of Kingdom
//

#include "appEnv.hpp"
#include <time.h>

enum Window {
	WIDTH = 512,
	HEIGHT = 512
};


//------------------------------------------------------------------------------------------

// プレイヤーと相手の構成要素
struct status {
	int money;
	int fund;
	short territory;
	int soldier;
	int mercenary;
	short fame;
	short infamy;
};

// プレイヤー情報
// 所持金、所有資産、領地、兵士、義勇兵、名声、悪名
status player = {500, 500, 0, 10, 0, 0, 0};

// 人望 ＝ 名声 ー 悪名
short player_popularity = player.fame - player.infamy;

// 相手情報
// ※各章の開始時にデータの初期化
// 所持金、所有資産、領地、兵士、義勇兵、名声、悪名
status enemy;
short enemy_popularity;

// 資産の価格情報
const unsigned short fundinfo[14] = {
  1500, 500, 1000,
  1000, 1500, 3000,
  30000, 10000, 5000, 50000,
  20000, 10000, 10000, 65535
};

// 売却できる資産の価格情報
const unsigned short iteminfo[7] = {1000, 1500, 3000, 10000, 5000, 10000, 20000};

// アイテムの所有フラグ
short player_fund[14] = {0};
short player_item[7] = {1, 0};

// 戦闘用データの準備
// pow = 兵力,  Attack = 攻撃力
int player_pow, player_Attack, enemy_pow, enemy_Attack;

// 戦闘開始時の兵力データ保存用
int player_pow0, enemy_pow0;

// 戦闘ターン前後の兵力計算用
int player_start, player_end, player_damage;
int enemy_start, enemy_end, enemy_damage;

// 直前のプレイヤーの行動
short player_lastcmd;

// 兵力ゲージ表示用
float player_powGauge, enemy_powGauge;

// 兵力ゲージの色
Color player_gaugecolor, enemy_gaugecolor;

// キャラクター画像の切り出しサイズ
const float chara_width = 20.0;
const float chara_height = 32.0;

// テロップ画像の切り出しサイズ
const float telop_width = 472.0;
const float telop_height = 20.0;
const float number_width = 14.0;
const float status_width = 56.0;

// ボタン画像の切り出しサイズ（ゲーム中）
const float button_width = 112.0;
const float button_height = 32.0;
const float buybutton_width = 240.0;

// 戦闘用フラグ
bool add_attack = false;

// ストーリー用フラグ

float month = 0.0;       // ２章開始からのゲーム内経過時間 : ３６以上でエクストラステージ突入不可能
bool ex_stage = false;    // エクストラステージ突入フラグ
bool spy = false;        // スパイ活動中か
short spy_time = 0;      // スパイ活動開始からのゲーム内行動回数 : １０ターン経過でリセット
bool sabotage = false;   // 妨害工作を使用したか
short sabo_time = 0;     // 妨害工作の残りカウント : ３回妨害で終了（スパイ活動中：６回）
bool rumor = false;      // 風説の流布を使用したか : ステージクリアでフラグリセット
bool disclos = false;    // 悪事が発覚したか
bool rebel = false;      // 反乱の成否判定
bool cmd_flag = false;   // コマンドフラグ


//------------------------------------------------------------------------------------------

void BG_fullslide(AppEnv& app_env, Texture& tex_before, Texture& tex_after);
void BG_slide(AppEnv& app_env, Texture& tex_before, Texture& tex_after);
void BG_fadeout(AppEnv& app_env, Texture& texture, float base, float limit, float angle);
void BG_fadein(AppEnv& app_env, Texture& texture, float base, float limit, float angle);
void titleCall(AppEnv& app_env, short flag);
void clearCall(AppEnv& app_env, short flag);

void duel_standby();
void duel_setpowGauge();
void Attack_limit();
void duel_turnstart();
void duel_fight(short player_cmd, short enemy_cmd);
void duel_turnend();
int duel_enemycommand(short flag);
void battleAnime(AppEnv&, short, short, short);

Color setpowColor(float pow);
Media set_a_bgm(short flag);
Media set_b_bgm(short flag);
Media set_main_bgm(short flag);

void add_soldier(AppEnv& app_env, short genre, short flag);
int buy(AppEnv& app_env);
int sale(AppEnv& app_env);

bool on_button(Vec2f pos, float x, float y, float tex_width, float tex_height);
Color setColor(bool on_mouse);

void tutorial(AppEnv&);
void _3rdstage_win(AppEnv&);
void _4thstage_win(AppEnv&);
void Exstage_win(AppEnv&);


//------------------------------------------------------------------------------------------

int main() {
	AppEnv app_env(Window::WIDTH, Window::HEIGHT);

	short flag = 0;
	bool on_mouse;
	Vec2f mouse_pos;
	Color angle;

	// タイトル
	{
		Texture titleBG("res/png/Sp0_TitleBack.png");
		Texture titleLg("res/png/Sp1_TitleLogo.png");
		Texture select("res/png/Sp2_Select.png");

    Media opening("res/wav/Event_Opening.wav");
    opening.looping(true);
    opening.gain(0.1);
    opening.play();

		// ボタン画像の切り出しサイズ（タイトル）
		float mode_width = 208.0;
		float mode_height = 56.0;

		float x, y;
		float time = -256.0;

		while (1) {
			if (!app_env.isOpen()) return 0;

			app_env.setupDraw();

			mouse_pos = app_env.mousePosition();

			// タイトル画面の表示
			drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
				0, 0, Window::WIDTH, Window::HEIGHT,
				titleBG, Color(1, 1, 1));
			drawTextureBox(-Window::WIDTH / 2, Window::HEIGHT / 3, 512, 64,
				0, 0, 512, 64,
				titleLg, Color(1, 1, 1));

			// Continue
			{
				x = -mode_width / 2;
				y = -Window::HEIGHT / 6;

				on_mouse = on_button(mouse_pos, x, y, mode_width, mode_height);
				angle = setColor(on_mouse);
				drawTextureBox(-mode_width / 2, -Window::HEIGHT / 6, mode_width, mode_height,
					0, mode_height, mode_width, mode_height,
					select, angle);

				if (on_mouse) {
					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();

						Texture button("res/png/Me1_MenuButton.png");
						Texture message("res/png/Me2_System.png");
						Texture display("res/png/BG1_Castle.png");

						while (1) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							mouse_pos = app_env.mousePosition();

							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT - 365,
								0, 365, Window::WIDTH, Window::HEIGHT - 365,
								display, Color(1, 1, 1));
							// データをロードします。よろしいですか？
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
								0, telop_height * 40, telop_width, telop_height,
								message, Color(0, 0, 0));

							// Yes
							{
								x = -Window::WIDTH / 6;
								y = -Window::HEIGHT / 2 + mode_height;

								on_mouse = on_button(mouse_pos, x - button_width, y, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x - button_width, y, button_width, button_height,
									0, button_height * 19, button_width, button_height,
									button, angle);

								if (on_mouse) {
									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();

										std::ifstream save("res/Savedata.txt");
										if (save) {
                      flag = 6;
											break;
										}
										else {
											while (1) {
												if (!app_env.isOpen()) return 0;
												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													break;
												}

												app_env.setupDraw();
												drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT - 365,
													0, 365, Window::WIDTH, Window::HEIGHT - 365,
													display, Color(1, 1, 1));
												// 読み込めるデータがありませんでした。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 41, telop_width, telop_height,
													message, Color(0, 0, 0));
												app_env.update();
											}
											break;
										}
									} // LEFT Push end
								}
							}

							// No
							{
								x = Window::WIDTH / 6;
								y = -Window::HEIGHT / 2 + mode_height;

								on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x, y, button_width, button_height,
									0, button_height * 20, button_width, button_height,
									button, angle);

								if (on_mouse) {
									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										break;
									}
								}
							}

							app_env.update();
						} // while end
						if (flag != 0) break;
					}
				}
			} // Continue end

			// New Game
			{
				x = -mode_width / 2;
				y = -Window::HEIGHT / 3;

				on_mouse = on_button(mouse_pos, x, y, mode_width, mode_height);
				angle = setColor(on_mouse);
				drawTextureBox(-mode_width / 2, -Window::HEIGHT / 3, mode_width, mode_height,
					0, 0, mode_width, mode_height,
					select, angle);

				if (on_mouse) {
					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();

						Texture titleOp("res/png/Sp3_Opening.png");
						Texture telop("res/png/T01_Opening.png");

						BG_fullslide(app_env, titleBG, titleOp);
						BG_fadeout(app_env, titleOp, 1.0, 0.5, 0.01);
						if (!app_env.isOpen()) return 0;

						// テロップ表示
						while (time < 780) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							time += 0.5;
							if (app_env.isPressKey('S')) {
								time += 1.0;
							}

							if (app_env.isPushButton(Mouse::LEFT)) {
								time = 780;
								app_env.flushInput();
							}

							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH, Window::HEIGHT,
								titleOp, Color(0.5, 0.5, 0.5));
							drawTextureBox(-Window::WIDTH / 2 + 20, -Window::HEIGHT + time, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH - 20, Window::HEIGHT,
								telop, Color(1, 1, 1));

							app_env.update();
						}

						BG_fadeout(app_env, titleOp, 0.5, 0.0, 0.01);
						break;
					}
				}

				app_env.update();
			} // New Game end
		} // while end
    opening.stop();
	} // title end

	//------------------------------------------------------------------------------------------

	// メイン背景とメッセージの読み込み
  Texture blackBG("res/png/BG0_Black.png");
	Texture storyBG("res/png/BG1_Castle.png");
  Texture mainBG("res/png/BG2_GrassField.png");
  Texture eventBG("res/png/BG3_Castle2.png");
  Texture extraBG("res/png/BG4_Shrine.png");
	Texture button("res/png/Me1_MenuButton.png");
	Texture system("res/png/Me2_System.png");
	Texture battle("res/png/Me3_Battle.png");
	Texture b_menu("res/png/Me4_BuyMenu.png");
	Texture buytxt("res/png/Me5_BuyTxt.png");
  Texture s_user("res/png/Me7_StatusWindow.png");
  Texture target("res/png/Me8_target.png");

	// キャラクター画像読み込み
	Texture chara1("res/png/01_Hero.png");     // 引きこもり王子
	Texture chara2("res/png/02_Daijin.png");   // じい
	Texture chara3("res/png/03_Heroine.png");  // ひめ
	Texture chara4("res/png/04_Rival_A.png");  // イケメン
	Texture chara5("res/png/05_Rival_B.png");  // デブ
	Texture chara6("res/png/06_Rival_C.png");  // 兵士
  Texture chara7("res/png/07_Unknown.png");  // ボス

  Media castle("res/wav/Event_Mainthema.wav");
  castle.looping(true);
  castle.gain(0.1);
  Media se_win("res/wav/se_win.wav");
  se_win.gain(0.1);
  Media se_lose("res/wav/se_lose.wav");
  se_lose.gain(0.1);

	// 本編
	while (1) {

		// 章イベント
		switch (flag) {

			// プロローグ
			case 0:
			{
				// テロップ画像読み込み
				Texture s_telop("res/png/T02_Prologue_Story.png");
				Texture e_telop("res/png/T02_Prologue_Event.png");

				// 相手情報初期化
				enemy = {0, 0, 0, 10, 0, 0, 0};

				titleCall(app_env, flag);

				int click_checker = 0;
        float animetime = 0;
        float teloptime = 0;
				bool stop = false;
				bool chara = false;
				bool telop = false;
				bool story_end = false;

        castle.play();

				while (1) {
					if (!app_env.isOpen()) return 0;
					if (story_end) break;

					mouse_pos = app_env.mousePosition();
					if (!stop) {
						if (app_env.isPushButton(Mouse::LEFT)) {
							app_env.flushInput();
							click_checker += 1;
							chara = false;
							telop = false;
						}

            if (app_env.isPushKey('S')) {
              app_env.flushInput();
              break;
            }
					}

					app_env.setupDraw();
          if (click_checker > 7 && click_checker < 20) {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              eventBG, Color(1, 1, 1));
          }
          else if (click_checker == 20) {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              blackBG, Color(1, 1, 1));
          }
          else if (click_checker < 46) {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              storyBG, Color(1, 1, 1));
          }
          else {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              mainBG, Color(1, 1, 1));
          }

          switch (click_checker) {
            case 0:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * (click_checker * 2), telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 1:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * (click_checker * 2), telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 2:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * (click_checker * 2), telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 3:
              if (animetime < 0.2) {
                animetime += 0.02;
              }
              drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                0, 0, Window::WIDTH, Window::HEIGHT,
                storyBG, Color(1, 1 - animetime, 1 - (animetime * 3)));
              if (animetime >= 0.2) {
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 6, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                if (teloptime < 100) {
                  teloptime += 2;
                  drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), teloptime, telop_height * 1,
                    0, telop_height * 8, teloptime, telop_height * 1,
                    s_telop, Color(0, 0, 0));
                }
                else {
                  drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 1,
                    0, telop_height * 9, telop_width, telop_height * 1,
                    s_telop, Color(0, 0, 0));
                }
              }
              break;

            case 4:
              click_checker += 1;
              break;

            case 5:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * (click_checker * 2), telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 6:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * (click_checker * 2), telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 7:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * (click_checker * 2), telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 8:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                0, telop_height * 16, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              break;

            case 9:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                0, telop_height * 17, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              break;

            case 10:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 18, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 11:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 20, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 12:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 21, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 13:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 22, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 14:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 23, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 15:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 24, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 16:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 25, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 17:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 26, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 18:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 27, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 19:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 28, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 20:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 30, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 21:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 78, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 31, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 22:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 78, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 33, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 23:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 78, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 34, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 24:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 78, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 36, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 25:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 78, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 38, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 26:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 78, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 40, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 27:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 42, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 28:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 81, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 44, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 29:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 81, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 45, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 30:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 81, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 46, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 31:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 81, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 47, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 32:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 3,
                0, telop_height * 49, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            case 33:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 52, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 34:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 1), telop_width, telop_height * 4,
                0, telop_height * 54, telop_width, telop_height * 4,
                s_telop, Color(0, 0, 0));
              break;

            case 35:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 58, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            case 36:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 62, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 37:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 81, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 63, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 38:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 81, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 65, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 39:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 80, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 67, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 40:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 79, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 68, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 41:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 80, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 69, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 42:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 80, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 71, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 43:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 79, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 73, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 44:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 75, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 45:
              BG_fadeout(app_env, storyBG, 1.0, 0, 0.02);
              BG_fullslide(app_env, blackBG, mainBG);
              click_checker += 1;
              break;

            case 46:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 0, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 47:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 1, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 48:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 2, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 49:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 4, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 50:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 5, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 51:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 6, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 52:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 8, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 53:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 9, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 54:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 10, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 55:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 11, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 56:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 12, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 57:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 15, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 58:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 16, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 59:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 18, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 60:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 20, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            default:
              story_end = true;
              break;
          }

					app_env.update();
        }
        castle.stop();
				break;
			}

			// １章
			case 1:
      {
        Texture s_telop("res/png/T02_Prologue_Story.png");
        Texture e_telop("res/png/T03_1stStage_Event.png");

        // 相手情報初期化
        enemy = {2500, 1000, 5, 100, 0, 0, 0};

        titleCall(app_env, flag);

        int click_checker = 0;
        float animetime = 0;
        float teloptime = 0;
        bool stop = false;
        bool chara = false;
        bool telop = false;
        bool story_end = false;

        castle.play();

        while (1) {
          if (!app_env.isOpen()) return 0;
          if (story_end) break;

          mouse_pos = app_env.mousePosition();
          if (!stop) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click_checker += 1;
              chara = false;
              telop = false;
            }

            if (app_env.isPushKey('S')) {
              app_env.flushInput();
              break;
            }
          }

          app_env.setupDraw();
          if (click_checker < 9) {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              storyBG, Color(1, 1, 1));
          }
          else {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              mainBG, Color(1, 1, 1));
          }

          switch (click_checker) {
            case 0:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 0, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 1:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 1, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 2:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 2, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 3:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 77, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 4, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 4:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 6, telop_width, telop_height * 1,
                e_telop, Color(0, 0, 0));
              break;

            case 5:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 1), telop_width, telop_height * 4,
                0, telop_height * 7, telop_width, telop_height * 4,
                e_telop, Color(0, 0, 0));
              break;

            case 6:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 11, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 7:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 13, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            case 8:
              if (!chara) {
                BG_fullslide(app_env, storyBG, mainBG);
                chara = true;
              }
              else {
                drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                  0, 0, Window::WIDTH, Window::HEIGHT,
                  mainBG, Color(1, 1, 1));
              }

              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 17, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            case 9:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 20, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            case 10:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 24, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            case 11:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 1), telop_width, telop_height * 4,
                0, telop_height * 27, telop_width, telop_height * 4,
                e_telop, Color(0, 0, 0));
              break;

            case 12:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 31, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 13:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 34, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            case 14:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 37, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            case 15:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 41, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            case 16:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 44, telop_width, telop_height * 2,
                e_telop, Color(0, 0, 0));
              break;

            case 17:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 46, telop_width, telop_height * 3,
                e_telop, Color(0, 0, 0));
              break;

            default:
              story_end = true;
              break;
          }

          app_env.update();
        }
        castle.stop();
				break;
			}

			// ２章
			case 2:
      {
        Texture s_telop("res/png/T04_2ndStage_Story.png");

        // 相手情報初期化
        enemy = {10000, 6000, 5, 200, 0, 20, 0};

        titleCall(app_env, flag);

        int click_checker = 0;
        float animetime = 0;
        float teloptime = 0;
        bool stop = false;
        bool chara = false;
        bool telop = false;
        bool story_end = false;

        castle.play();

        while (1) {
          if (!app_env.isOpen()) return 0;
          if (story_end) break;

          mouse_pos = app_env.mousePosition();
          if (!stop) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click_checker += 1;
              chara = false;
              telop = false;
            }

            if (app_env.isPushKey('S')) {
              app_env.flushInput();
              break;
            }
          }

          app_env.setupDraw();
          if (click_checker < 15) {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              storyBG, Color(1, 1, 1));
          }
          else {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              mainBG, Color(1, 1, 1));
          }

          switch (click_checker) {
            case 0:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 0, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 1:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 1, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 2:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 3, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 3:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 5, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 4:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 6, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 5:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 7, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 6:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 9, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 7:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 10, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 8:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 12, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 9:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 14, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 10:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 16, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 11:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 18, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 12:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 20, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 13:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 25, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 22, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 14:
              BG_fullslide(app_env, storyBG, mainBG);
              click_checker += 1;
              break;

            case 15:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 1), telop_width, telop_height * 4,
                0, telop_height * 27, telop_width, telop_height * 4,
                s_telop, Color(0, 0, 0));
              break;

            case 16:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 1), telop_width, telop_height * 4,
                0, telop_height * 31, telop_width, telop_height * 4,
                s_telop, Color(0, 0, 0));
              break;

            case 17:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 35, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            case 18:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 38, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            default:
              story_end = true;
              break;
          }

          app_env.update();
        }
        castle.stop();
				break;
			}

			// ３章
			case 3:
      {
        Texture s_telop("res/png/T05_3rdStage_Story.png");

        // 相手情報初期化
        enemy = {200000, 100000, 10, 20000, 0, 0, 80};

        titleCall(app_env, flag);

        int click_checker = 0;
        float animetime = 0;
        float teloptime = 0;
        bool stop = false;
        bool chara = false;
        bool telop = false;
        bool story_end = false;

        castle.play();

        while (1) {
          if (!app_env.isOpen()) return 0;
          if (story_end) break;

          mouse_pos = app_env.mousePosition();
          if (!stop) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click_checker += 1;
              chara = false;
              telop = false;
            }

            if (app_env.isPushKey('S')) {
              app_env.flushInput();
              break;
            }
          }

          app_env.setupDraw();
          if (click_checker < 9) {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              storyBG, Color(1, 1, 1));
          }
          else {
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              mainBG, Color(1, 1, 1));
          }

          switch (click_checker) {
            case 0:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 17, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 0, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 1:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 17, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 2, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 2:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 18, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 4, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 3:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 18, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 6, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 4:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 18, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 8, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 5:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 18, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 10, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 6:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 17, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 12, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 7:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 17, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 14, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 8:
              BG_fullslide(app_env, storyBG, mainBG);
              click_checker += 1;
              break;

            case 9:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 17, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 20, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 10:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 17, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 22, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            default:
              story_end = true;
              break;
          }

          app_env.update();
        }
        castle.stop();
				break;
			}

			// ４章
			case 4:
      {
        Texture s_telop("res/png/T06_4thStage_Story.png");

        // 相手情報初期化
        enemy = {75000, 110000, 10, 15000, 0, 80, 0};

        titleCall(app_env, flag);

        int click_checker = 0;
        float animetime = 0;
        float teloptime = 0;
        bool stop = false;
        bool chara = false;
        bool telop = false;
        bool story_end = false;

        castle.play();

        while (1) {
          if (!app_env.isOpen()) return 0;
          if (story_end) break;

          mouse_pos = app_env.mousePosition();
          if (!stop) {
            if (app_env.isPushButton(Mouse::LEFT)) {
              app_env.flushInput();
              click_checker += 1;
              chara = false;
              telop = false;
            }

            if (app_env.isPushKey('S')) {
              app_env.flushInput();
              break;
            }
          }

          app_env.setupDraw();
          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            storyBG, Color(1, 1, 1));

          switch (click_checker) {
            case 0:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 0, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 1:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 1, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 2:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 2, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 3:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 4, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 4:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 5, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 5:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 7, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            case 6:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 10, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            case 7:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 13, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 8:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 15, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 9:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 16, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 10:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 17, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 11:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 18, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 12:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 20, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            case 13:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 46, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 22, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            case 14:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 46, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 25, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 15:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 46, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                0, telop_height * 27, telop_width, telop_height * 3,
                s_telop, Color(0, 0, 0));
              break;

            case 16:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 46, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 30, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 17:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 46, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 32, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 18:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 46, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 34, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 19:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 46, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 36, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 20:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 39, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 21:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                0, telop_height * 41, telop_width, telop_height * 2,
                s_telop, Color(0, 0, 0));
              break;

            case 22:
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 45, telop_width, telop_height,
                s_telop, Color(0, 0, 0));
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                0, telop_height * 43, telop_width, telop_height * 1,
                s_telop, Color(0, 0, 0));
              break;

            default:
              story_end = true;
              break;
          }

          app_env.update();
        }
        castle.stop();
				break;
			}

			// エクストラ
			case 5:
      {
        // エクストラステージ突入できるか
        // できなければスキップ

        // 相手情報初期化
        enemy = {300000, 100000, 10, 30000, 0, 80, 0};
        if (ex_stage) {
          Texture s_telop("res/png/T07_ExStage_Story.png");
          Texture n_telop("res/png/T02_Prologue_Story.png");

          titleCall(app_env, flag);

          int click_checker = 0;
          float animetime = 0;
          float teloptime = 0;
          bool stop = false;
          bool chara = false;
          bool telop = false;
          bool story_end = false;

          castle.play();

          while (1) {
            if (!app_env.isOpen()) return 0;
            if (story_end) break;

            mouse_pos = app_env.mousePosition();
            if (!stop) {
              if (app_env.isPushButton(Mouse::LEFT)) {
                app_env.flushInput();
                click_checker += 1;
                chara = false;
                telop = false;
              }

              if (app_env.isPushKey('S')) {
                app_env.flushInput();
                break;
              }
            }

            app_env.setupDraw();
            drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
              0, 0, Window::WIDTH, Window::HEIGHT,
              storyBG, Color(1, 1, 1));

            switch (click_checker) {
              case 0:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 0, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 1:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 1, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 2:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 3, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 3:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 78, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 4, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 4:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 5, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 5:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 50, telop_width, telop_height,
                  s_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 6, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 6:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 7, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 7:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 50, telop_width, telop_height,
                  s_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 8, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 8:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 50, telop_width, telop_height,
                  s_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 10, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 9:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 78, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 12, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 10:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 13, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 11:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 14, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 12:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 15, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 13:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 16, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 14:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 17, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 15:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 3,
                  0, telop_height * 19, telop_width, telop_height * 3,
                  s_telop, Color(0, 0, 0));
                break;

              case 16:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 22, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 17:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 24, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 18:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 25, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 19:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 27, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 20:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
                  0, telop_height * 29, telop_width, telop_height * 1,
                  s_telop, Color(0, 0, 0));
                break;

              case 21:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 79, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 30, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              case 22:
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                  0, telop_height * 77, telop_width, telop_height,
                  n_telop, Color(0, 0, 0));
                drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
                  0, telop_height * 32, telop_width, telop_height * 2,
                  s_telop, Color(0, 0, 0));
                break;

              default:
                story_end = true;
                break;
            }

            app_env.update();
          }
          castle.stop();
        }
				break;
			}

			// コンティニュー時のデータ読み込み
      default:
        std::ifstream save("res/Savedata.txt");
        if (save) {
          save >> flag
            >> month
            >> player.money
            >> player.fund
            >> player.territory
            >> player.soldier
            >> player.mercenary
            >> player.fame
            >> player.infamy
            >> player_fund[0]
            >> player_fund[1]
            >> player_fund[2]
            >> player_fund[3]
            >> player_fund[4]
            >> player_fund[5]
            >> player_fund[6]
            >> player_fund[7]
            >> player_fund[8]
            >> player_fund[9]
            >> player_fund[10]
            >> player_fund[11]
            >> player_fund[12]
            >> player_fund[13]
            >> player_item[0]
            >> player_item[1]
            >> player_item[2]
            >> player_item[3]
            >> player_item[4]
            >> player_item[5]
            >> player_item[6]
            >> enemy.money
            >> enemy.fund
            >> enemy.territory
            >> enemy.soldier
            >> enemy.mercenary
            >> enemy.fame
            >> enemy.infamy;
        }
				break;
		}

		//------------------------------------------------------------------------------------------

		float x, y;
		float x0, y0;
    Color angle_b;
    bool territory_buy;

		if (flag == 2) {
			month = 0;
		}

    short i = 0;
    spy = false;
    sabotage = false;
    rumor = false;
    disclos = false;
    rebel = false;
    cmd_flag = false;

    Media mainTM = set_main_bgm(flag);
    mainTM.looping(true);
    mainTM.gain(0.1);

		// メインゲーム
		while (1) {
			if (!app_env.isOpen()) return 0;
			app_env.setupDraw();

			mouse_pos = app_env.mousePosition();
			drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
				0, 0, Window::WIDTH, Window::HEIGHT,
				mainBG, Color(1, 1, 1));

      if (!mainTM.isPlaying()) {
        mainTM.play();
      }

      if (flag >= 2 && month >= 36) {
        ex_stage = false;
      }

      if (flag == 0) {
        mainTM.stop();
        tutorial(app_env);
        break;
      }

      // 勝利条件を満たしているか
      {
        // 王子の城を買収した
        if (flag == 1 && player_fund[0]) {
          mainTM.stop();
          break;
        }
        // ２章で出現した物件をすべて買収した
        else if (flag == 2 && player_fund[3] && player_fund[4] && player_fund[5]) {
          mainTM.stop();
          break;
        }
        // ３章で出現した物件をすべて買収した
        else if (flag == 3 && player_fund[6] && player_fund[7] && player_fund[8] && player_fund[9]) {
          mainTM.stop();
          break;
        }
        // 黒歴史ノートを売却した
        else if (flag == 4 && player_item[6] == 2) {
          mainTM.stop();
          _4thstage_win(app_env);
          break;
        }
        // 相手の領地が０
        else if (enemy.territory <= 0) {
          mainTM.stop();
          if (flag == 5) {
            Exstage_win(app_env);
          }
          break;
        }
        // 反乱成功
        else if (rebel) {
          mainTM.stop();
          if (flag == 3) {
            _3rdstage_win(app_env);
          }
          break;
        }
      }

      // ステータスが上限を超えてないか確認
      {
        // プレイヤー
        if (player.money > 999999) {
          player.money = 999999;
        }
        else if (player.money < 0) {
          player.money = 0;
        }
        if (player.soldier > 999999) {
          player.soldier = 999999;
        }
        else if (player.soldier < 0) {
          player.soldier = 0;
        }
        if (player.mercenary > 999999) {
          player.mercenary = 999999;
        }
        else if (player.mercenary < 0) {
          player.mercenary = 0;
        }
        if (player.fame > 100) {
          player.fame = 100;
        }
        else if (player.fame < 0) {
          player.fame = 0;
        }
        if (player.infamy > 100) {
          player.infamy = 100;
        }
        else if (player.infamy < 0) {
          player.infamy = 0;
        }

        // 相手
        if (enemy.money > 999999) {
          enemy.money = 999999;
        }
        else if (enemy.money < 0) {
          enemy.money = 0;
        }
        if (enemy.soldier > 999999) {
          enemy.soldier = 999999;
        }
        else if (enemy.soldier < 0) {
          enemy.soldier = 0;
        }
        if (enemy.mercenary > 999999) {
          enemy.mercenary = 999999;
        }
        else if (enemy.mercenary < 0) {
          enemy.mercenary = 0;
        }
        if (enemy.fame > 100) {
          enemy.fame = 100;
        }
        else if (enemy.fame < 0) {
          enemy.fame = 0;
        }
        if (enemy.infamy > 100) {
          enemy.infamy = 100;
        }
        else if (enemy.infamy < 0) {
          enemy.infamy = 0;
        }
      }

      // プレイヤー情報の表示
			drawTextureBox(0, (Window::HEIGHT / 2) - 360, 256, 360,
				0, 0, 256, 360,
        s_user, Color(1, 1, 1));
      // あなたの情報
      drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 2), status_width * 2, telop_height,
        0, telop_height * 95, status_width * 2, telop_height,
        system, Color(0, 0, 0));

      // 勝利条件
      drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 14), (Window::WIDTH / 2), telop_height,
        0, telop_height * 0, (Window::WIDTH / 2), telop_height,
        target, Color(0, 0, 0));
      // 領地を占領
      drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 15), (Window::WIDTH / 2), telop_height,
        0, telop_height * 5, (Window::WIDTH / 2), telop_height,
        target, Color(0, 0, 0));
      if (flag >= 1 && flag <= 4) {
        drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 16), (Window::WIDTH / 2), telop_height,
          0, telop_height * flag, (Window::WIDTH / 2), telop_height,
          target, Color(0, 0, 0));
      }

      // 資金
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 3), status_width, telop_height,
				0, telop_height * 87, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // 画像表示位置計算、他
        short tex_x, tex_y;       // 画像切り出し用
        int val = player.money;

        // ステータス情報を取得
        int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 6; i += 1) {
          // i 桁が０でない
          if (money[i] != 0) {
            tex_x = number_width * money[i];
            tex_y = telop_height * 82;
          }

          // i 桁が０だったとき
          else {
            // 配列の最初
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // それ以外
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // 現在参照している桁より上の桁がすべて０か確認
              if (i == 1 && money[i - 1] == 0) {}
              else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
              else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
              else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
              // ０ではなかったとき、スキップしない
              else {
                skip -= 1;
                tex_y = telop_height * 82;
              }
            }
          }

          drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 3), number_width, telop_height,
            tex_x, tex_y, number_width, telop_height,
            system, Color(0, 0, 0));
        }
      }

      // 兵士
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 4), status_width, telop_height,
				0, telop_height * 88, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // 画像表示位置計算、他
        short tex_x, tex_y;       // 画像切り出し用
        int val = player.soldier;

        // ステータス情報を取得
        int soldier[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 6; i += 1) {
          // i 桁が０でない
          if (soldier[i] != 0) {
            tex_x = number_width * soldier[i];
            tex_y = telop_height * 82;
          }

          // i 桁が０だったとき
          else {
            // 配列の最初
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // それ以外
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // 現在参照している桁より上の桁がすべて０か確認
              if (i == 1 && soldier[i - 1] == 0) {}
              else if (i == 2 && (soldier[i - 2] || soldier[i - 1]) == 0) {}
              else if (i == 3 && (soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
              else if (i == 4 && (soldier[i - 4] || soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
              // ０ではなかったとき、スキップしない
              else {
                skip -= 1;
                tex_y = telop_height * 82;
              }
            }
          }

          drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 4), number_width, telop_height,
            tex_x, tex_y, number_width, telop_height,
            system, Color(0, 0, 0));
        }
      }

      // 義勇兵（義兵）
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 5), status_width, telop_height,
				0, telop_height * 89, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // 画像表示位置計算、他
        short tex_x, tex_y;       // 画像切り出し用
        int val = player.mercenary;

        // ステータス情報を取得
        int mercenary[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 6; i += 1) {
          // i 桁が０でない
          if (mercenary[i] != 0) {
            tex_x = number_width * mercenary[i];
            tex_y = telop_height * 82;
          }

          // i 桁が０だったとき
          else {
            // 配列の最初
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // それ以外
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // 現在参照している桁より上の桁がすべて０か確認
              if (i == 1 && mercenary[i - 1] == 0) {}
              else if (i == 2 && (mercenary[i - 2] || mercenary[i - 1]) == 0) {}
              else if (i == 3 && (mercenary[i - 3] || mercenary[i - 2] || mercenary[i - 1]) == 0) {}
              else if (i == 4 && (mercenary[i - 4] || mercenary[i - 3] || mercenary[i - 2] || mercenary[i - 1]) == 0) {}
              // ０ではなかったとき、スキップしない
              else {
                skip -= 1;
                tex_y = telop_height * 82;
              }
            }
          }

          drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 5), number_width, telop_height,
            tex_x, tex_y, number_width, telop_height,
            system, Color(0, 0, 0));
        }
      }

      // 名声
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 6), status_width, telop_height,
				0, telop_height * 90, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // 画像表示位置計算、他
        short tex_x, tex_y;       // 画像切り出し用
        short val = player.fame;

        // ステータス情報を取得
        short fame[3] = {val / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 3; i += 1) {
          // i 桁が０でない
          if (fame[i] != 0) {
            tex_x = number_width * fame[i];
            tex_y = telop_height * 82;
          }

          // i 桁が０だったとき
          else {
            // 配列の最初
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // それ以外
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // 現在参照している桁より上の桁がすべて０か確認
              if (i == 1 && fame[i - 1] == 0) {}
              // ０ではなかったとき、スキップしない
              else {
                skip -= 1;
                tex_y = telop_height * 82;
              }
            }
          }

          drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 6), number_width, telop_height,
            tex_x, tex_y, number_width, telop_height,
            system, Color(0, 0, 0));
        }
      }

      // 悪名
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 7), status_width, telop_height,
				0, telop_height * 91, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // 画像表示位置計算、他
        short tex_x, tex_y;       // 画像切り出し用
        short val = player.infamy;

        // ステータス情報を取得
        short infamy[3] = {val / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 3; i += 1) {
          // i 桁が０でない
          if (infamy[i] != 0) {
            tex_x = number_width * infamy[i];
            tex_y = telop_height * 82;
          }

          // i 桁が０だったとき
          else {
            // 配列の最初
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // それ以外
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // 現在参照している桁より上の桁がすべて０か確認
              if (i == 1 && infamy[i - 1] == 0) {}
              // ０ではなかったとき、スキップしない
              else {
                skip -= 1;
                tex_y = telop_height * 82;
              }
            }
          }

          drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 7), number_width, telop_height,
            tex_x, tex_y, number_width, telop_height,
            system, Color(0, 0, 0));
        }
      }

      // 相手の情報表示（スパイ活動中のみ）
      if (spy) {
        drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 9), status_width * 2, telop_height,
          0, telop_height * 96, status_width * 2, telop_height,
          system, Color(0, 0, 0));
        // 資金
				drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 10), status_width, telop_height,
					0, telop_height * 87, status_width, telop_height,
          system, Color(0, 0, 0));
        {
          short i, skip = 0;        // 画像表示位置計算、他
          short tex_x, tex_y;       // 画像切り出し用
          int val = enemy.money;

          // ステータス情報を取得
          int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i 桁が０でない
            if (money[i] != 0) {
              tex_x = number_width * money[i];
              tex_y = telop_height * 82;
            }

            // i 桁が０だったとき
            else {
              // 配列の最初
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // それ以外
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // 現在参照している桁より上の桁がすべて０か確認
                if (i == 1 && money[i - 1] == 0) {}
                else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
                else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                // ０ではなかったとき、スキップしない
                else {
                  skip -= 1;
                  tex_y = telop_height * 82;
                }
              }
            }

            drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 10), number_width, telop_height,
              tex_x, tex_y, number_width, telop_height,
              system, Color(0, 0, 0));
          }
        }

        // 兵士
				drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 11), status_width, telop_height,
					0, telop_height * 88, status_width, telop_height,
          system, Color(0, 0, 0));
        {
          short i, skip = 0;        // 画像表示位置計算、他
          short tex_x, tex_y;       // 画像切り出し用
          int val = enemy.soldier;

          // ステータス情報を取得
          int soldier[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i 桁が０でない
            if (soldier[i] != 0) {
              tex_x = number_width * soldier[i];
              tex_y = telop_height * 82;
            }

            // i 桁が０だったとき
            else {
              // 配列の最初
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // それ以外
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // 現在参照している桁より上の桁がすべて０か確認
                if (i == 1 && soldier[i - 1] == 0) {}
                else if (i == 2 && (soldier[i - 2] || soldier[i - 1]) == 0) {}
                else if (i == 3 && (soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
                else if (i == 4 && (soldier[i - 4] || soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
                // ０ではなかったとき、スキップしない
                else {
                  skip -= 1;
                  tex_y = telop_height * 82;
                }
              }
            }

            drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 11), number_width, telop_height,
              tex_x, tex_y, number_width, telop_height,
              system, Color(0, 0, 0));
          }
        }

        // 領地
        drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 12), status_width, telop_height,
          0, telop_height * 92, status_width, telop_height,
          system, Color(0, 0, 0));
        {
          short i, skip = 0;        // 画像表示位置計算、他
          short tex_x, tex_y;       // 画像切り出し用
          short val = enemy.territory;

          // ステータス情報を取得
          short territory[3] = {val / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 3; i += 1) {
            // i 桁が０でない
            if (territory[i] != 0) {
              tex_x = number_width * territory[i];
              tex_y = telop_height * 82;
            }

            // i 桁が０だったとき
            else {
              // 配列の最初
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // それ以外
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // 現在参照している桁より上の桁がすべて０か確認
                if (i == 1 && territory[i - 1] == 0) {}
                // ０ではなかったとき、スキップしない
                else {
                  skip -= 1;
                  tex_y = telop_height * 82;
                }
              }
            }

            drawTextureBox(10 + status_width + ((1 + number_width) * (i - skip)), (Window::HEIGHT / 2) - ((2 + telop_height) * 12), number_width, telop_height,
              tex_x, tex_y, number_width, telop_height,
              system, Color(0, 0, 0));
          }
        }

        if (spy_time >= 10) {
          spy = false;
        }
			}

      if (sabo_time <= 0) {
        sabotage = false;
      }

      cmd_flag = false;

			// 資金繰り
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 0, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// 資産に関するコマンドです。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 0, telop_width, telop_height,
						system, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						angle_b = angle;

            short checker = 0;

						while (1) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							mouse_pos = app_env.mousePosition();
							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH, Window::HEIGHT,
								mainBG, Color(1, 1, 1));
							drawTextureBox(x, y, button_width, button_height,
								0, button_height * 0, button_width, button_height,
								button, angle_b);

              // ボタンを押す前の処理
              if (checker == 0) {
                // やめる
                {
                  on_mouse = on_button(mouse_pos, x, y - ((2 + button_height) * 1), button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x, y - ((2 + button_height) * 1), button_width, button_height,
                    0, button_height * 21, button_width, button_height,
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
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 6, button_width, button_height,
                    button, angle);

                  if (on_mouse) {

                    // 兵士たちにアルバイトさせて資金を得ます。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 14, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // 名声が少し下がります。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                      0, telop_height * 15, telop_width, telop_height,
                      system, Color(0, 0, 0));

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 1;
                    }
                  }
                }

                // 執政
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 7, button_width, button_height,
                    button, angle);

                  if (on_mouse) {

                    // 政治を行い、税収を得ます。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 16, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // 所有資産の価値が高いほど効果が上がります。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                      0, telop_height * 17, telop_width, telop_height,
                      system, Color(0, 0, 0));

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 2;
                    }
                  }
                }

                // 売却
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 8, button_width, button_height,
                    button, angle);

                  if (on_mouse) {

                    // 資産を売却して資金を得ます。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 18, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // 売却した資産はなくなります。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                      0, telop_height * 19, telop_width, telop_height,
                      system, Color(0, 0, 0));

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      sale(app_env);
                      break;
                    }
                  }
                }
              }

              // ボタンを押した後の処理：売却以外
              else {
                if (checker == 1) {
                  // ・アルバイト収入
                  drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                    0, telop_height * 43, telop_width, telop_height,
                    system, Color(0, 0, 0));

                  {
                    short i, skip = 0;        // 画像表示位置計算、他
                    short tex_x, tex_y;       // 画像切り出し用
                    int val = player.soldier * 2;

                    if (val >= (999999 - player.money)) {
                      val = (999999 - player.money);
                    }

                    int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

                    for (i = 0; i < 6; i += 1) {
                      // i 桁が０でない
                      if (money[i] != 0) {
                        tex_x = number_width * money[i];
                        tex_y = telop_height * 82;
                      }

                      // i 桁が０だったとき
                      else {
                        // 配列の最初
                        if (i == 0) {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;
                        }

                        // それ以外
                        else {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;

                          // 現在参照している桁より上の桁がすべて０か確認
                          if (i == 1 && money[i - 1] == 0) {}
                          else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          // ０ではなかったとき、スキップしない
                          else {
                            skip -= 1;
                            tex_y = telop_height * 82;
                          }
                        }
                      }

                      x0 = (-telop_width / 2) + status_width + ((1 + number_width) * (i - skip));
                      drawTextureBox(x0, (-Window::HEIGHT / 2) + (telop_height * 4), number_width, telop_height,
                        tex_x, tex_y, number_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  if (app_env.isPushButton(Mouse::LEFT)) {
                    app_env.flushInput();
                    player.money += player.soldier * 2;
                    player.fame -= 1;

                    if (spy) {
                      spy_time += 1;
                    }

                    if (sabotage) {
                      sabo_time -= 1;
                    }
                    else {
                      enemy.money += (enemy.fund / 2) + (enemy.territory * 100);
                    }

                    month += 0.1;
                    if (player.fame < 0) {
                      player.fame = 0;
                    }
                    break;
                  }
                }

                else {
                  // ・税収
                  drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                    0, telop_height * 44, telop_width, telop_height,
                    system, Color(0, 0, 0));

                  {
                    short i, skip = 0;        // 画像表示位置計算、他
                    short tex_x, tex_y;       // 画像切り出し用
                    int val = (player.fund / 2) + (player.territory * 100);

                    if (val > (999999 - player.money)) {
                      val = (999999 - player.money);
                    }

                    int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

                    for (i = 0; i < 6; i += 1) {
                      // i 桁が０でない
                      if (money[i] != 0) {
                        tex_x = number_width * money[i];
                        tex_y = telop_height * 82;
                      }

                      // i 桁が０だったとき
                      else {
                        // 配列の最初
                        if (i == 0) {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;
                        }

                        // それ以外
                        else {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;

                          // 現在参照している桁より上の桁がすべて０か確認
                          if (i == 1 && money[i - 1] == 0) {}
                          else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          // ０ではなかったとき、スキップしない
                          else {
                            skip -= 1;
                            tex_y = telop_height * 82;
                          }
                        }
                      }

                      x0 = (-telop_width / 2) + status_width + ((1 + number_width) * (i - skip));
                      drawTextureBox(x0, (-Window::HEIGHT / 2) + (telop_height * 4), number_width, telop_height,
                        tex_x, tex_y, number_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  if (app_env.isPushButton(Mouse::LEFT)) {
                    app_env.flushInput();
                    player.money += (player.fund / 2) + (player.territory * 100);

                    if (spy) {
                      spy_time += 1;
                    }

                    if (sabotage) {
                      sabo_time -= 1;
                    }
                    else {
                      enemy.money += (enemy.fund / 2) + (enemy.territory * 100);
                    }

                    month += 0.1;
                    break;
                  }
                }
              }

							app_env.update();
						}
					}
				}
			}

			// 兵力増強
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 1, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// 兵力に関するコマンドです。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 1, telop_width, telop_height,
						system, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						angle_b = angle;

						while (1) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							mouse_pos = app_env.mousePosition();
							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH, Window::HEIGHT,
								mainBG, Color(1, 1, 1));
							drawTextureBox(x, y, button_width, button_height,
								0, button_height * 1, button_width, button_height,
								button, angle_b);

              // やめる
              {
                on_mouse = on_button(mouse_pos, x, y - ((2 + button_height) * 1), button_width, button_height);
                angle = setColor(on_mouse);
                drawTextureBox(x, y - ((2 + button_height) * 1), button_width, button_height,
                  0, button_height * 21, button_width, button_height,
                  button, angle);

                if (on_mouse) {
                  if (app_env.isPushButton(Mouse::LEFT)) {
                    app_env.flushInput();
                    break;
                  }
                }
              }

							// 兵士募集
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 9, button_width, button_height,
									button, angle);

								if (on_mouse) {

									// 正規の兵士を募集します。
									drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
										0, telop_height * 21, telop_width, telop_height,
										system, Color(0, 0, 0));

									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										add_soldier(app_env, 0, flag);
										break;
									}
								}
							}

							// 義勇兵
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 10, button_width, button_height,
									button, angle);

								if (on_mouse) {

									// 義勇兵を集めます。
									drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
										0, telop_height * 22, telop_width, telop_height,
										system, Color(0, 0, 0));
									// 戦闘終了、または次の月になるといなくなります。
									drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
										0, telop_height * 23, telop_width, telop_height,
										system, Color(0, 0, 0));

									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										add_soldier(app_env, 1, flag);
										break;
									}
								}
							}

							// 強制兵役
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 11, button_width, button_height,
									button, angle);

								if (on_mouse) {

									// 強制的に兵士を集めます。
									drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
										0, telop_height * 24, telop_width, telop_height,
										system, Color(0, 0, 0));
									// 人望が下がります。
									drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
										0, telop_height * 25, telop_width, telop_height,
										system, Color(0, 0, 0));

									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										add_soldier(app_env, 2, flag);
										break;
									}
								}
							}

							app_env.update();
						}
					}
				}
			}

			// 計略
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 2, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// 作戦に関するコマンドです。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 2, telop_width, telop_height,
						system, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						angle_b = angle;

            short checker = 0;
            short p_flag = 0;

						while (1) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							mouse_pos = app_env.mousePosition();
							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH, Window::HEIGHT,
								mainBG, Color(1, 1, 1));

              if (checker == 0) {
                drawTextureBox(x, y, button_width, button_height,
                  0, button_height * 2, button_width, button_height,
                  button, angle_b);

                // やめる
                {
                  on_mouse = on_button(mouse_pos, x, y - ((2 + button_height) * 1), button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x, y - ((2 + button_height) * 1), button_width, button_height,
                    0, button_height * 21, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      break;
                    }
                  }
                }

                // スパイ
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  if (spy) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  else if (player.money >= 100) {
                    angle = setColor(on_mouse);
                  }
                  else {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 12, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // ・活動資金：100G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 58, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (spy) {
                      // 既に実行しています。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 98, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }
                    else if (player.money >= 100) {
                      // 相手にスパイを送り込み、情報をさぐります。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 27, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // 相手の状況が少しの間、わかるようになります。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 28, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 資金が足りません。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 5, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }

                    if (on_mouse && app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 1;
                    }
                  }
                }

                // ぼう害工作
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  if (sabotage) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  else if (player.money >= 50) {
                    angle = setColor(on_mouse);
                  }
                  else {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 13, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // ・活動資金：50G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 61, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (sabotage) {
                      // 既に実行しています。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 98, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }
                    else if (player.money >= 50) {
                      // 兵士や資金をしばらく増やせないようにします。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 29, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 資金が足りません。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 5, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }

                    if (on_mouse && app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 2;
                    }
                  }
                }

                // 風説の流布
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  if (rumor) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  else if (player.money >= 100) {
                    angle = setColor(on_mouse);
                  }
                  else {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 14, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // ・活動資金：100G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 58, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (rumor) {
                      // 既に実行しています。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 98, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }
                    else if (player.money >= 100) {
                      // 相手の資産の価値を下げます。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 30, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 資金が足りません。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 5, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }

                    if (on_mouse && app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 3;
                    }
                  }
                }

                // どくを盛る
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  if (player.money >= 200) {
                    angle = setColor(on_mouse);
                  }
                  else {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 15, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // ・活動資金：200G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 66, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (player.money >= 200) {
                      // 料理に どくを盛って、兵士を戦えなくさせます。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 31, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // 発覚すると悪名が大きく上がります。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 32, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 資金が足りません。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 5, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 4;
                    }
                  }
                }

                // 演説
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 7);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  if (player.money >= 100) {
                    angle = setColor(on_mouse);
                  }
                  else {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 16, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // ・活動資金：100G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 58, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (player.money >= 100) {
                      // 正義を説いて、支持を得ます。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 33, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // 名声が上がります。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 34, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 資金が足りません。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 5, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 5;
                    }
                  }
                }

                // ゴシップ
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 8);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  if (player.money >= 200) {
                    angle = setColor(on_mouse);
                  }
                  else {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 17, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // ・活動資金：200G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 66, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (player.money >= 200) {
                      // 相手の悪いウワサを流して、人望を下げます。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 35, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // 失敗すると自分の人望が下がります。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 36, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 資金が足りません。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 5, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 6;
                    }
                  }
                }

                // 反乱
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 9);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 18, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // 相手の国内で反乱をさそいます。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 37, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // 成功すると、国外追放します。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                      0, telop_height * 38, telop_width, telop_height,
                      system, Color(0, 0, 0));

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 7;
                    }
                  }
                }
              }

              // ボタンを押した後の処理
              else {
                // 「はい」を押す前
                if (p_flag == 0) {
                  if (checker == 1) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 12, button_width, button_height,
                      button, angle_b);
                    // スパイを送ります。よろしいですか？
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 57, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 2) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 13, button_width, button_height,
                      button, angle_b);
                    // 妨害工作を行います。よろしいですか？
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 60, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 3) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 14, button_width, button_height,
                      button, angle_b);
                    // デマ情報を流します。よろしいですか？
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 63, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 4) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 15, button_width, button_height,
                      button, angle_b);
                    // 食中毒を発生させます。よろしいですか？
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 65, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 5) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 16, button_width, button_height,
                      button, angle_b);
                    // 演説を始めます。よろしいですか？
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 69, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 6) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 17, button_width, button_height,
                      button, angle_b);
                    // ゴシップ記事を流します。よろしいですか？
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 70, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 7) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 18, button_width, button_height,
                      button, angle_b);
                    // 決起します。よろしいですか？
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 71, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  // はい
                  {
                    y0 = y + (2 + button_height);
                    on_mouse = on_button(mouse_pos, x, y0, button_width, button_height);
                    angle = setColor(on_mouse);
                    drawTextureBox(x, y0, button_width, button_height,
                      0, button_height * 19, button_width, button_height,
                      button, angle);

                    if (on_mouse) {
                      if (app_env.isPushButton(Mouse::LEFT)) {
                        app_env.flushInput();
                        p_flag = 1;
                      }
                    }
                  }

                  // いいえ
                  {
                    on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
                    angle = setColor(on_mouse);
                    drawTextureBox(x, y, button_width, button_height,
                      0, button_height * 20, button_width, button_height,
                      button, angle);

                    if (on_mouse) {
                      if (app_env.isPushButton(Mouse::LEFT)) {
                        app_env.flushInput();
                        checker = 0;
                      }
                    }
                  }
                }

                // 「はい」を押した後
                else {
                  // スパイの実行結果
                  if (checker == 1) {
                    // スパイを送りました。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 59, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (p_flag == 1) {
                      player.money -= 100;
                      spy = true;
                      spy_time = 0;
                      p_flag = 2;
                    }
                  }

                  // 妨害工作の実行結果
                  else if (checker == 2) {
                    // 妨害を始めます。
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 62, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (p_flag == 1) {
                      player.money -= 50;
                      sabotage = true;
                      if (spy) {
                        sabo_time = 6;
                        spy_time += 1;
                      }
                      else {
                        sabo_time = 3;
                      }
                      p_flag = 2;
                    }
                  }

                  // 風説の流布の実行結果
                  else if (checker == 3) {
                    if (p_flag == 1) {
                      srand((unsigned)time(NULL));
                      short prob = rand() % 100;

                      if (spy || prob > 24) {
                        if (spy) {
                          spy_time += 1;
                        }
                        cmd_flag = true;
                        enemy.fund -= enemy.fund / 10;
                        rumor = true;
                      }
                      player.money -= 100;
                      p_flag = 2;
                    }

                    if (cmd_flag) {
                      // 成功しました。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 55, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // 資産価値が10%下がりました。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 64, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 失敗。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // 毒を盛るの実行結果
                  else if (checker == 4) {
                    if (p_flag == 1) {
                      srand((unsigned)time(NULL));
                      short prob = rand() % 100;
                      disclos = false;

                      if (spy) {
                        cmd_flag = true;
                        enemy.soldier -= enemy.soldier / 10;
                        spy_time += 1;
                      }
                      else if (prob > 24) {
                        cmd_flag = true;
                        enemy.soldier -= enemy.soldier / 10;
                      }

                      if (!spy) {
                        srand((unsigned)time(NULL));
                        prob = rand() % 100;
                        if (prob > 49) {
                          disclos = true;
                          player.infamy += 10;
                        }
                      }

                      player.money -= 200;
                      p_flag = 2;
                    }

                    if (cmd_flag) {
                      // 相手の兵士たちを10%戦闘不能にしました。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 67, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 失敗。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }

                    if (disclos) {
                      // 発覚してしまった
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 68, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // 演説の実行結果
                  else if (checker == 5) {
                    if (p_flag == 1) {
                      srand((unsigned)time(NULL));
                      short prob = rand() % 100;

                      if (prob > 19) {
                        cmd_flag = true;
                        if (player.infamy > 0) {
                          player.fame += 1;
                          player.infamy -= 1;
                        }
                        else {
                          player.fame += 2;
                        }
                      }
                      player.money -= 100;
                      p_flag = 2;
                    }

                    if (cmd_flag) {
                      // 成功しました。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 55, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 失敗。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // ゴシップの実行結果
                  else if (checker == 6) {
                    if (p_flag == 1) {
                      srand((unsigned)time(NULL));
                      short prob = rand() % 100;

                      if (spy) {
                        prob += 20;
                        spy_time += 1;
                      }

                      if (prob > 29) {
                        cmd_flag = true;
                        enemy.infamy += 5;
                      }
                      else {
                        player.infamy += 5;
                      }
                      player.money -= 200;
                      p_flag = 2;
                    }

                    if (cmd_flag) {
                      // 成功しました。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 55, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // 失敗。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // 反乱の実行結果
                  else if (checker == 7) {
                    if (p_flag == 1) {
                      // ３章だったとき
                      if (flag == 3) {
                        if (player_item[3] == 0) {
                          rebel = false;
                        }
                        else {
                          rebel = true;
                        }
                      }

                      // ３章以外
                      else {
                        srand((unsigned)time(NULL));
                        short prob = rand() % 100;

                        if (spy) {
                          prob -= 20;
                          spy_time += 1;
                        }

                        if (prob < enemy.infamy) {
                          cmd_flag = true;

                          if (flag != 5) {
                            srand((unsigned)time(NULL));
                            short prob = rand() % 100;

                            if (prob < enemy.infamy + (player.fame - player.infamy) / 2) {
                              rebel = true;
                            }
                          }

                          if (!rebel) {
                            enemy.money = enemy.money / 2;
                            enemy.soldier = enemy.soldier / 2;
                          }
                        }
                      }

                      p_flag = 2;
                    }

                    if (flag == 3) {
                      if (rebel) {
                        // 反乱大成功
                        // 反乱が成功して国外追放した！
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 72, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                      else {
                        Texture telop3("res/png/T05_3rdStage_Story.png");
                        // 反乱失敗
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 3,
                          0, telop_height * 32, telop_width, telop_height * 3,
                          telop3, Color(0, 0, 0));
                      }
                    }
                    else {
                      if (rebel) {
                        // 反乱大成功
                        // 反乱が成功して国外追放した！
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 72, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                      else if (cmd_flag) {
                        // 反乱発動
                        // 反乱は起きたが、国外追放できなかった・・・
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 73, telop_width, telop_height,
                          system, Color(0, 0, 0));
                        // しかし、混乱の影響で大打撃を与えた。
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                          0, telop_height * 74, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                      else {
                        // 反乱失敗
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 75, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                    }
                  }

                  if (app_env.isPushButton(Mouse::LEFT)) {
                    app_env.flushInput();
                    month += 0.1;
                    break;
                  }
                }
              }

							app_env.update();
						}
					}
				}
			}

			// 買収
			{
        x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 3, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// 相手の資産を買取ります。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 4, telop_width, telop_height,
						system, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						angle_b = angle;

						while (1) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							mouse_pos = app_env.mousePosition();
							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH, Window::HEIGHT,
								mainBG, Color(1, 1, 1));
							drawTextureBox(x, y + ((2 + button_height) * 3), button_width, button_height,
								0, button_height * 3, button_width, button_height,
								button, angle_b);

							// どれを買取りますか？
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 77, telop_width, telop_height,
								system, Color(0, 0, 0));

							// やめる
							{
								on_mouse = on_button(mouse_pos, x, y + ((2 + button_height) * 2), button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
									0, button_height * 21, button_width, button_height,
									button, angle);

								if (on_mouse) {
									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										break;
									}
								}
							}

							if (flag >= 1 && flag < 5) {
								// 領地
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
                  switch (flag) {
                    case 1:
                      if (player.money >= (6 - enemy.territory) * 500) {
                        angle = setColor(on_mouse);
                        territory_buy = true;
                      }
                      else {
                        angle = Color(0.5, 0.5, 0.5);
                        territory_buy = false;
                      }
                      break;

                    case 2:
                      if (player.money >= (6 - enemy.territory) * 1000) {
                        angle = setColor(on_mouse);
                        territory_buy = true;
                      }
                      else {
                        angle = Color(0.5, 0.5, 0.5);
                        territory_buy = false;
                      }
                      break;

                    case 3:
                      if (player.money >= (11 - enemy.territory) * 2000) {
                        angle = setColor(on_mouse);
                        territory_buy = true;
                      }
                      else if (enemy.territory < 3) {
                        angle = Color(0.5, 0.5, 0.5);
                        territory_buy = false;
                      }
                      else {
                        angle = Color(0.5, 0.5, 0.5);
                        territory_buy = false;
                      }
                      break;

                    case 4:
                      if (player.money >= (11 - enemy.territory) * 2500) {
                        angle = setColor(on_mouse);
                        territory_buy = true;
                      }
                      else if (enemy.territory < 3) {
                        angle = Color(0.5, 0.5, 0.5);
                        territory_buy = false;
                      }
                      else {
                        angle = Color(0.5, 0.5, 0.5);
                        territory_buy = false;
                      }
                      break;

                    default:
                      angle = Color(0.5, 0.5, 0.5);
                      territory_buy = false;
                      break;
                  }
									angle = setColor(on_mouse);
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 14, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
                    if (!territory_buy) {
                      if (flag == 3 || flag == 4) {
                        if (enemy.territory < 3) {
                          // これ以上買い取れません。
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 94, telop_width, telop_height,
                            system, Color(0, 0, 0));
                        }
                        else {
                          // 資金が足りません。
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 5, telop_width, telop_height,
                            system, Color(0, 0, 0));
                        }
                      }
                      else {
                        // 資金が足りません。
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 5, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                    }
                    else {
                      // 相手の領地を買い取ります。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 85, telop_width, telop_height,
                        system, Color(0, 0, 0));

                      if (app_env.isPushButton(Mouse::LEFT)) {
                        app_env.flushInput();
                        if (buy(app_env) == 1) {
                          switch (flag) {
                            case 1:
                              player.money -= (6 - enemy.territory) * 500;
                              player.territory += 1;
                              enemy.territory -= 1;
                              break;

                            case 2:
                              player.money -= (6 - enemy.territory) * 1000;
                              player.territory += 1;
                              enemy.territory -= 1;
                              break;

                            case 3:
                              player.money -= (11 - enemy.territory) * 2000;
                              player.territory += 1;
                              enemy.territory -= 1;
                              break;

                            case 4:
                              player.money -= (11 - enemy.territory) * 2500;
                              player.territory += 1;
                              enemy.territory -= 1;
                              break;
                          }
                        }
                        break;
                      }
                    }
									}
								}
							}

							// １章から出現
							if (flag >= 1 && flag < 6) {
								// １章のみ
								if (flag == 1) {
									// 城
									{
										x0 = x + 2 + button_width;
										y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

										on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
										if (player_fund[0] == 0) {
											angle = setColor(on_mouse);
                    }
                    else if (player.money < fundinfo[0]) {
                      angle = Color(0.5, 0.5, 0.5);
                    }
										else {
											angle = Color(0.5, 0.5, 0.5);
										}
										drawTextureBox(x0, y0, buybutton_width, button_height,
											0, button_height * 0, buybutton_width, button_height,
											b_menu, angle);

										if (on_mouse) {
											if (player_fund[0] == 0) {
												if (player.money < fundinfo[0]) {
													// 資金が足りません。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[0]) {
													// かつて自分たちが住んでいた城
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 0, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// オバケがでそうなくらいボロボロ・・・
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
														0, telop_height * 1, telop_width, telop_height,
														buytxt, Color(0, 0, 0));

													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														if (buy(app_env) == 1) {
															player.money -= fundinfo[0];
															player.fund += fundinfo[0] / 2;
															enemy.money += fundinfo[0];
															enemy.fund -= fundinfo[0] / 2;
															player_fund[0] = 1;
														}
														break;
													}
												}
											}

											else {
												// すでに所有しています。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 1 end
								} // flag 1 only

								// ３章まで
								if (flag < 4) {
									// 農地
									{
										x0 = x + 2 + button_width;
										y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

										on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
										if (player_fund[1] == 0) {
											angle = setColor(on_mouse);
                    }
                    else if (player.money < fundinfo[1]) {
                      angle = Color(0.5, 0.5, 0.5);
                    }
										else {
											angle = Color(0.5, 0.5, 0.5);
										}
										drawTextureBox(x0, y0, buybutton_width, button_height,
											0, button_height * 1, buybutton_width, button_height,
											b_menu, angle);

										if (on_mouse) {
											if (player_fund[1] == 0) {
												if (player.money < fundinfo[1]) {
													// 資金が足りません。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[1]) {
													// 農地。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 2, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// 牧場が近いので、家畜用のエサを生産している。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
														0, telop_height * 3, telop_width, telop_height,
														buytxt, Color(0, 0, 0));

													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														if (buy(app_env) == 1) {
															player.money -= fundinfo[1];
															player.fund += fundinfo[1] / 2;
															enemy.money += fundinfo[1];
															enemy.fund -= fundinfo[1] / 2;
															player_fund[1] = 1;
														}
														break;
													}
												}
											}

											else {
												// すでに所有しています。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 2 end

									// 牧場
									{
										x0 = x + 2 + button_width;
										y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

										on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
										if (player_fund[2] == 0) {
											angle = setColor(on_mouse);
                    }
                    else if (player.money < fundinfo[2]) {
                      angle = Color(0.5, 0.5, 0.5);
                    }
										else {
											angle = Color(0.5, 0.5, 0.5);
										}
										drawTextureBox(x0, y0, buybutton_width, button_height,
											0, button_height * 2, buybutton_width, button_height,
											b_menu, angle);

										if (on_mouse) {
											if (player_fund[2] == 0) {
												if (player.money < fundinfo[2]) {
													// 資金が足りません。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[2]) {
													// 牧場。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 4, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// 主に牛、ブタを育てているが、規模が小さい。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														buytxt, Color(0, 0, 0));

													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														if (buy(app_env) == 1) {
															player.money -= fundinfo[2];
															player.fund += fundinfo[2] / 2;
															enemy.money += fundinfo[2];
															enemy.fund -= fundinfo[2] / 2;
															player_fund[2] = 1;
														}
														break;
													}
												}
											}

											else {
												// すでに所有しています。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 3 end
								} // flag 1-3
							} // flag 1

							// ２章から出現
							if (flag >= 2 && flag < 6) {
								// ３章まで
								if (flag < 4) {
									// 農地 +1
									{
										x0 = x + 2 + button_width;
										y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

										on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
										if (player_fund[3] == 0) {
											angle = setColor(on_mouse);
                    }
                    else if (player.money < fundinfo[3]) {
                      angle = Color(0.5, 0.5, 0.5);
                    }
										else {
											angle = Color(0.5, 0.5, 0.5);
										}
										drawTextureBox(x0, y0, buybutton_width, button_height,
											0, button_height * 3, buybutton_width, button_height,
											b_menu, angle);

										if (on_mouse) {
											if (player_fund[3] == 0) {
												if (player.money < fundinfo[3]) {
													// 資金が足りません。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[3]) {
													// ちょっと立派な農地。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 6, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// ワイン用の果物を生産している。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
														0, telop_height * 7, telop_width, telop_height,
														buytxt, Color(0, 0, 0));

													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														if (buy(app_env) == 1) {
															player.money -= fundinfo[3];
															player.fund += fundinfo[3] / 2;
															enemy.money += fundinfo[3];
															enemy.fund -= fundinfo[3] / 2;
															player_fund[3] = 1;
														}
														break;
													}
												}
											}

											else {
												// すでに所有しています。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 4 end

									// 牧場 +1
									{
										x0 = x + 2 + button_width;
										y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

										on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
										if (player_fund[4] == 0) {
											angle = setColor(on_mouse);
                    }
                    else if (player.money < fundinfo[4]) {
                      angle = Color(0.5, 0.5, 0.5);
                    }
										else {
											angle = Color(0.5, 0.5, 0.5);
										}
										drawTextureBox(x0, y0, buybutton_width, button_height,
											0, button_height * 4, buybutton_width, button_height,
											b_menu, angle);

										if (on_mouse) {
											if (player_fund[4] == 0) {
												if (player.money < fundinfo[4]) {
													// 資金が足りません。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[4]) {
													// 大きな牧場。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 8, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// 兵士たちが乗る馬を育てている。
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
														0, telop_height * 9, telop_width, telop_height,
														buytxt, Color(0, 0, 0));

													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														if (buy(app_env) == 1) {
															player.money -= fundinfo[4];
															player.fund += fundinfo[4] / 2;
															enemy.money += fundinfo[4];
															enemy.fund -= fundinfo[4] / 2;
															player_fund[4] = 1;
														}
														break;
													}
												}
											}

											else {
												// すでに所有しています。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 5 end
								} // flag 2-3

								// 工場
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[5] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[5]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 5, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[5] == 0) {
											if (player.money < fundinfo[5]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[5]) {
												// 流行りの服を生産している工場。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 10, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// 職人の技が光る、名産品。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 11, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[5];
														player.fund += fundinfo[5] / 2;
														enemy.money += fundinfo[5];
														enemy.fund -= fundinfo[5] / 2;
														player_fund[5] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 1-2 end
							} // flag 2

							if (flag >= 3 && flag < 6) {
								// 金の鉱山
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 7);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[6] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[6]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 6, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[6] == 0) {
											if (player.money < fundinfo[6]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[6]) {
												// 金が採れる鉱山。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 12, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// 量は多いが、品質は高くないようだ。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 17, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[6];
														player.fund += fundinfo[6] / 2;
														enemy.money += fundinfo[6];
														enemy.fund -= fundinfo[6] / 2;
														player_fund[6] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 6 end

								// 銀の鉱山
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 8);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[7] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[7]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 7, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[7] == 0) {
											if (player.money < fundinfo[7]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[7]) {
												// 銀が採れる鉱山。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 14, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// 量は多いが
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 15, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[7];
														player.fund += fundinfo[7] / 2;
														enemy.money += fundinfo[7];
														enemy.fund -= fundinfo[7] / 2;
														player_fund[7] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 7 end

								// 鉄の鉱山
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 9);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[8] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[8]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 8, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[8] == 0) {
											if (player.money < fundinfo[8]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[8]) {
												// 鉄が採れる鉱山。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 16, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// 量は多いが
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 13, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[8];
														player.fund += fundinfo[8] / 2;
														enemy.money += fundinfo[8];
														enemy.fund -= fundinfo[8] / 2;
														player_fund[8] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 8 end

								// 美術館
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 10);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[9] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[9]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 9, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[9] == 0) {
											if (player.money < fundinfo[9]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[9]) {
												// ゴウカな美術館。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 18, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// 飾られてるのは、実はニセモノだらけのガラクタ。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 19, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[9];
														player.fund += fundinfo[9] / 2;
														enemy.money += fundinfo[9];
														enemy.fund -= fundinfo[9] / 2;
														player_fund[9] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 10 end
							} // flag 3

							// ４章から出現
							if (flag >= 4 && flag < 6) {
								// 良質な鉄鉱山
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[10] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[10]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 10, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[10] == 0) {
											if (player.money < fundinfo[10]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[10]) {
												// 良質な鉄が採れる鉱山。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 20, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// 質が高く、良い武具を作るにはかかせない。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 21, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[10];
														player.fund += fundinfo[10] / 2;
														enemy.money += fundinfo[10];
														enemy.fund -= fundinfo[10] / 2;
														player_fund[10] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 2-2 end

								// 武器の鍛冶屋
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[11] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[11]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
                  else {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 11, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[11] == 0) {
											if (player.money < fundinfo[11]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[11]) {
												// 有名な鍛冶屋
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 22, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// ここで作られる武具は名品。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 23, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[11];
														player.fund += fundinfo[11] / 2;
														enemy.money += fundinfo[11];
														enemy.fund -= fundinfo[11] / 2;
														player_fund[11] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 3-2 end

								// 防具の鍛冶屋
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[12] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[12]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 12, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[12] == 0) {
											if (player.money < fundinfo[12]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[12]) {
												// 有名な鍛冶屋
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 22, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// ここで作られる武具は名品。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
													0, telop_height * 23, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[12];
														player.fund += fundinfo[12] / 2;
														enemy.money += fundinfo[12];
														enemy.fund -= fundinfo[12] / 2;
														player_fund[12] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 4-2 end

								// 店
								{
									x0 = x + 2 + button_width;
									y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

									on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
									if (player_fund[13] == 0) {
										angle = setColor(on_mouse);
                  }
                  else if (player.money < fundinfo[13]) {
                    angle = Color(0.5, 0.5, 0.5);
                  }
									else {
										angle = Color(0.5, 0.5, 0.5);
									}
									drawTextureBox(x0, y0, buybutton_width, button_height,
										0, button_height * 13, buybutton_width, button_height,
										b_menu, angle);

									if (on_mouse) {
										if (player_fund[13] == 0) {
											if (player.money < fundinfo[13]) {
												// 資金が足りません。
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[13]) {
												// 彼も
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 24, telop_width, telop_height,
													buytxt, Color(0, 0, 0));

												if (app_env.isPushButton(Mouse::LEFT)) {
													app_env.flushInput();
													if (buy(app_env) == 1) {
														player.money -= fundinfo[13];
														player.fund += 100000;
														enemy.money += 50000;
														enemy.fund -= 50000;
														player_fund[13] = 1;
													}
													break;
												}
											}
										}

										else {
											// すでに所有しています。
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 5-2 end
							} // flag 4

							if (flag <= 0 && flag >= 6) {
								// 買取れる物件がありませんでした。
								drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
									0, telop_height * 80, telop_width, telop_height,
									system, Color(0, 0, 0));
							}

							app_env.update();
						}
					}
				}
			}

			// 侵攻
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 4, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// 相手に戦争をしかけます。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 7, telop_width, telop_height,
						system, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						angle_b = angle;

						while (1) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							mouse_pos = app_env.mousePosition();
							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH, Window::HEIGHT,
								mainBG, Color(1, 1, 1));
							drawTextureBox(x, y, button_width, button_height,
								0, button_height * 4, button_width, button_height,
								button, angle_b);

							// 戦争を始めます。本当によろしいですか？
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
								0, telop_height * 8, telop_width, telop_height,
								system, Color(0, 0, 0));

							// はい
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                if ((player.soldier + player.mercenary) == 0) {
                  angle = Color(0.5, 0.5, 0.5);
                  on_mouse = false;
                  // 戦争できません。
                  drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                    0, telop_height * 99, telop_width, telop_height,
                    system, Color(0, 0, 0));
                }
                else {
                  angle = setColor(on_mouse);
                }
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 19, button_width, button_height,
									button, angle);

								if (on_mouse) {
									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
                    mainTM.stop();

                    if (enemy.soldier == 0) {
                      switch (flag) {
                        case 1:
                          enemy.mercenary = 500;
                          enemy.money -= enemy.mercenary * 5;
                          if (enemy.money < 0) {
                            enemy.money = 0;
                            enemy.infamy += 10;
                          }
                          break;

                        case 2:
                          enemy.mercenary = 1000;
                          enemy.money -= enemy.mercenary * 5;
                          if (enemy.money < 0) {
                            enemy.money = 0;
                            enemy.infamy += 10;
                          }
                          break;

                        case 3:
                          enemy.mercenary = 500;
                          enemy.money -= enemy.mercenary * 5;
                          if (enemy.money < 0) {
                            enemy.money = 0;
                            enemy.infamy += 10;
                          }
                          break;

                        case 4:
                          enemy.mercenary = 10000;
                          enemy.money -= enemy.mercenary * 5;
                          if (enemy.money < 0) {
                            enemy.money = 0;
                            enemy.infamy += 10;
                          }
                          break;

                        default:
                          enemy.soldier = 5000;
                          break;
                      }
                    }

                    srand((unsigned)time(NULL));
                    short count = (rand() % 3) + 3;

										short player_cmd = -1;
                    short enemy_cmd;

										player_lastcmd = -1;

										duel_standby();
										duel_setpowGauge();
										player_gaugecolor = setpowColor(player_powGauge);
										enemy_gaugecolor = setpowColor(enemy_powGauge);

										// ＢＧＭ準備
                    Media a_bgm = set_a_bgm(flag);
                    a_bgm.gain(0.1);
                    Media b_bgm = set_b_bgm(flag);
                    b_bgm.gain(0.1);
                    Media go("res/wav/se_battle.wav");
                    go.gain(0.1);

                    short playtime = 0;
                    go.play();
                    while (playtime < 100) {
                      playtime += 1;
                      if (!app_env.isOpen()) return 0;
                      app_env.setupDraw();
                      drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                        0, 0, Window::WIDTH, Window::HEIGHT,
                        mainBG, Color(1, 1, 1));
                      // 戦闘開始します。
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 0, telop_width, telop_height,
                        battle, Color(0, 0, 0));
                      app_env.update();
                    }

                    a_bgm.looping(true);
                    b_bgm.looping(true);
                    if (flag <= 4) {
                      b_bgm.looping(false);
                    }
										a_bgm.play();

										while (1) {
                      if (!app_env.isOpen()) return 0;
                      app_env.setupDraw();

											mouse_pos = app_env.mousePosition();

                      // どちらかの兵力が０になった
                      if (player_pow <= 0 || enemy_pow <= 0) {
                        break;
                      }

                      // 残り行動回数が０でない
                      if (count != 0) {
                        drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                          0, 0, Window::WIDTH, Window::HEIGHT,
                          mainBG, Color(1, 1, 1));
                      }

                      // 残り行動回数が０になったとき
                      else if (count == 0) {
                        while (1) {
                          if (!app_env.isOpen()) return 0;
                          app_env.setupDraw();

                          mouse_pos = app_env.mousePosition();

                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                            0, 0, Window::WIDTH, Window::HEIGHT,
                            mainBG, Color(1, 1, 1));

                          // 相手は撤退するようだ・・・
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 9, telop_width, telop_height,
                            battle, Color(0, 0, 0));

                          // 追撃
                          {
                            x = (-Window::WIDTH / 2) + 2;
                            y = ((Window::HEIGHT / 2) - 358) + ((button_height + 2) * 1);

                            on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
                            angle = setColor(on_mouse);
                            drawTextureBox(x, y, button_width, button_height,
                              0, button_height * 25, button_width, button_height,
                              button, angle);

                            if (on_mouse) {
                              if (app_env.isPushButton(Mouse::LEFT)) {
                                app_env.flushInput();
                                a_bgm.stop();
                                b_bgm.play();
                                add_attack = true;
                                count = -1;
                                break;
                              }
                            }
                          }

                          // 撤退
                          {
                            x = (-Window::WIDTH / 2) + 2;
                            y = ((Window::HEIGHT / 2) - 358) + ((button_height + 2) * 0);

                            on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
                            angle = setColor(on_mouse);
                            drawTextureBox(x, y, button_width, button_height,
                              0, button_height * 26, button_width, button_height,
                              button, angle);

                            if (on_mouse) {
                              if (app_env.isPushButton(Mouse::LEFT)) {
                                app_env.flushInput();
                                count = -4;
                                break;
                              }
                            }
                          }

                          app_env.update();
                        }
                      }

                      // 追撃戦突入なら背景を夕暮れ風に
                      if (count == -1) {
                        drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                          0, 0, Window::WIDTH, Window::HEIGHT,
                          mainBG, Color(1, 0.6, 0.2));
                        drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT - 360,
                          0, 360, Window::WIDTH, Window::HEIGHT - 360,
                          mainBG, Color(1, 1, 1));
                      }

                      // 追撃戦終了
                      else if (count == -2) {
                        break;
                      }

                      // 追撃しないならリザルト画面に進む
                      else if (count == -4) {
                        break;
                      }

                      // キャラクター表示（プレイヤー側）
                      {
                        x0 = -chara_width * 4;
                        y0 = -chara_height * 1;

                        float i, j;
                        for (i = 0; i < 5; i += 1) {
                          for (j = 0; j < 5; j += 1) {
                            drawTextureBox(x0 - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                              0, 0, chara_width, chara_height,
                              chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
                          }
                        }
                      }

                      // キャラクター表示（相手側）
                      {
                        x0 = chara_width * 3;
                        y0 = -chara_height * 1;

                        float i, j;
                        for (i = 0; i < 5; i += 1) {
                          for (j = 0; j < 5; j += 1) {
                            drawTextureBox(x0 + ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                              0, 0, chara_width, chara_height,
                              chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
                          }
                        }
                      }

											// プレイヤーの兵力ゲージ表示
											drawBox((-Window::WIDTH / 2) + 18, (Window::HEIGHT / 2) - 32,
												204, 24,
												3, Color(1, 0.8, 0.4));
											drawFillBox((-Window::WIDTH / 2) + 20, (Window::HEIGHT / 2) - 30,
												player_powGauge * 2, 20,
												player_gaugecolor);

											// 相手の兵力ゲージ表示
											drawBox((Window::WIDTH / 2) - 18, (Window::HEIGHT / 2) - 32,
												-204, 24,
												3, Color(1, 0.8, 0.4));
											drawFillBox((Window::WIDTH / 2) - (20 + (enemy_powGauge * 2)), (Window::HEIGHT / 2) - 30,
												enemy_powGauge * 2, 20,
												enemy_gaugecolor);

                      drawTextureBox(-Window::WIDTH / 2, (Window::HEIGHT / 2) - (40 + telop_height), Window::WIDTH, telop_height + 8,
                        0, telop_height * 5, Window::WIDTH, telop_height + 8,
                        battle, Color(0.5, 0.5, 0.5));

											// 突撃
											{
												x = (-Window::WIDTH / 2) + 2;
												y = ((Window::HEIGHT / 2) - 358) + ((button_height + 2) * 4);

												on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
												angle = setColor(on_mouse);
												drawTextureBox(x, y, button_width, button_height,
													0, button_height * 22, button_width, button_height,
													button, angle);

												if (on_mouse) {
													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														player_cmd = 0;
													}
												}
											}

											// 守り
											{
												x = (-Window::WIDTH / 2) + 2;
												y = ((Window::HEIGHT / 2) - 358) + ((button_height + 2) * 3);

												on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
												angle = setColor(on_mouse);
												drawTextureBox(x, y, button_width, button_height,
													0, button_height * 23, button_width, button_height,
													button, angle);

												if (on_mouse) {
													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														player_cmd = 1;
													}
												}
											}

											// ゆみ
											{
												x = (-Window::WIDTH / 2) + 2;
												y = ((Window::HEIGHT / 2) - 358) + ((button_height + 2) * 2);

												on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
												angle = setColor(on_mouse);
												drawTextureBox(x, y, button_width, button_height,
													0, button_height * 24, button_width, button_height,
													button, angle);

												if (on_mouse) {
													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();
														player_cmd = 2;
													}
												}
											}

											// 撤退
                      // 追撃戦突入したら表示しない
											if (count > -1) {
												x = (-Window::WIDTH / 2) + 2;
												y = ((Window::HEIGHT / 2) - 358) + ((button_height + 2) * 0);

												on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
												angle = setColor(on_mouse);
												drawTextureBox(x, y, button_width, button_height,
													0, button_height * 26, button_width, button_height,
													button, angle);

												if (on_mouse) {
													if (app_env.isPushButton(Mouse::LEFT)) {
														app_env.flushInput();

                            while (1) {
                              if (!app_env.isOpen()) return 0;
                              app_env.setupDraw();
                              drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                                0, 0, Window::WIDTH, Window::HEIGHT,
                                mainBG, Color(1, 1, 1));
                              // 撤退しました。
                              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                                0, telop_height * 7, telop_width, telop_height,
                                battle, Color(0, 0, 0));

                              if (app_env.isPushButton(Mouse::LEFT)) {
                                app_env.flushInput();
                                month += 0.3;
                                spy_time += 1;
                                count = -3;
                                a_bgm.stop();
                                break;
                              }
                              app_env.update();
                            }

														break;
													}
												}
											}

											// 戦闘処理
											{
                        // コマンド入力があったとき
												if (player_cmd != (-1)) {
													duel_turnstart();
                          Attack_limit();
                          enemy_cmd = duel_enemycommand(flag);
													duel_fight(player_cmd, enemy_cmd);
                          battleAnime(app_env, player_cmd, enemy_cmd, count);
													duel_turnend();
													duel_setpowGauge();
													player_gaugecolor = setpowColor(player_powGauge);
													enemy_gaugecolor = setpowColor(enemy_powGauge);
													player_cmd = -1;
                          count -= 1;
                          add_attack = false;
												}
											}

											app_env.update();
										}

                    // 戦闘終了の処理と結果画面の表示
                    if (count != -3) {
                      bool win = false;
                      bool bgm = false;

                      if (a_bgm.isPlaying()) {
                        a_bgm.stop();
                      }
                      else if (b_bgm.isPlaying()) {
                        b_bgm.stop();
                      }

                      while (1) {
                        if (!app_env.isOpen()) return 0;
                        app_env.setupDraw();

                        mouse_pos = app_env.mousePosition();

                        // 追撃戦をしていたら背景を夕暮れ風に
                        if (count == -2) {
                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                            0, 0, Window::WIDTH, Window::HEIGHT,
                            mainBG, Color(1, 0.6, 0.2));
                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT - 360,
                            0, 360, Window::WIDTH, Window::HEIGHT - 360,
                            mainBG, Color(1, 1, 1));
                        }

                        // そうでなければ通常の背景を表示
                        else {
                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                            0, 0, Window::WIDTH, Window::HEIGHT,
                            mainBG, Color(1, 1, 1));
                        }

                        if (player_powGauge >= enemy_powGauge) {
                          // 勝利
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 8, telop_width, telop_height,
                            battle, Color(0, 0, 0));

                          win = true;
                        }
                        else {
                          // 敗北
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 12, telop_width, telop_height,
                            battle, Color(0, 0, 0));
                        }

                        if (win && !bgm) {
                          if (!se_win.isPlaying()) {
                            se_win.play();
                          }
                        }
                        else if (!bgm) {
                          if (!se_lose.isPlaying()) {
                            se_lose.play();
                          }
                        }

                        bgm = true;

                        if (app_env.isPushButton(Mouse::LEFT)) {
                          if (win) {
                            enemy.territory -= 1;
                            player.territory += 1;
                            player.fame += 1;

                            // 敵を全滅させていれば、ボーナス追加
                            if (enemy_powGauge <= 0) {
                              enemy.territory -= 1;
                              player.territory += 1;
                              player.fame += 1;
                            }
                          }
                          app_env.flushInput();
                          player.mercenary = 0;
                          enemy.mercenary = 0;
                          month += 0.5;
                          spy_time += 2;
                          short sabo_flag = 2;

                          // 敵兵増加処理
                          if (sabotage) {
                            sabo_time -= 1;
                            sabo_flag = 1;
                          }

                          int value;
                          switch (flag) {
                            case 0:
                              break;

                            case 1:
                              if (enemy.soldier < 200) {
                                value = 200 - enemy.soldier;
                                if (value > 50 * sabo_flag) {
                                  value = 50 * sabo_flag;
                                }
                                if (enemy.money >= value * 10) {
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                                else {
                                  value = enemy.money / 10;
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                              }
                              break;

                            case 2:
                              if (enemy.soldier < 200) {
                                value = 200 - enemy.soldier;
                                if (value > 50 * sabo_flag) {
                                  value = 50 * sabo_flag;
                                }
                                if (enemy.money >= value * 10) {
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                                else {
                                  value = enemy.money / 10;
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                              }
                              break;

                            case 3:
                              if (enemy.money >= 2500 * sabo_flag) {
                                enemy.money -= 2500 * sabo_flag;
                                enemy.soldier += 250 * sabo_flag;
                              }
                              else {
                                value = enemy.money / 10;
                                enemy.money -= value * 10;
                                enemy.soldier += value;
                              }
                              break;

                            case 4:
                              if (enemy.soldier > 20000) {
                                if (enemy.money >= 2000 * sabo_flag) {
                                  enemy.money -= 2000 * sabo_flag;
                                  enemy.soldier += 200 * sabo_flag;
                                }
                                else {
                                  value = enemy.money / 10;
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                              }
                              else if (enemy.soldier > 10000) {
                                if (enemy.money >= 5000 * sabo_flag) {
                                  enemy.money -= 5000 * sabo_flag;
                                  enemy.soldier += 500 * sabo_flag;
                                }
                                else {
                                  value = enemy.money / 10;
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                              }
                              else {
                                value = 20000 - enemy.soldier;
                                if (enemy.money > value * 10) {
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                                else {
                                  value = enemy.money / 10;
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                              }
                              break;

                            default:
                              value = 10000 - enemy.soldier;
                              if (value > 0) {
                                if (enemy.money >= (value * 10) * 2) {
                                  enemy.money -= (value * 10) * 2;
                                  enemy.soldier += value * 2;
                                }
                                else {
                                  value = enemy.money / 10;
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                              }
                              else {
                                if (enemy.money >= 5000 * sabo_flag) {
                                  enemy.money -= 5000 * sabo_flag;
                                  enemy.soldier += 500 * sabo_flag;
                                }
                                else {
                                  value = enemy.money / 10;
                                  enemy.money -= value * 10;
                                  enemy.soldier += value;
                                }
                              }
                              break;
                          }

                          // アイテムのドロップ判定
                          int dropflag = (enemy_pow * 100) / enemy_pow0;
                          Media itemdrop("res/wav/se_itemget.wav");
                          itemdrop.gain(0.1);
                          switch (flag) {
                            case 1:
                              if ((100 - dropflag) > 25) {
                                player_item[1] = 1;
                                itemdrop.play();
                              }
                              break;

                            case 2:
                              if ((100 - dropflag) > 50) {
                                player_item[2] = 1;
                                itemdrop.play();
                              }
                              break;

                            case 3:
                              if (!player_item[3]) {
                                if (enemy_pow0 - enemy_pow > 10000) {
                                  player_item[3] = 1;
                                  itemdrop.play();
                                }
                              }
                              else {
                                if (enemy_pow0 - enemy_pow > 20000) {
                                  player_item[4] = 1;
                                  itemdrop.play();
                                }
                              }
                              break;

                            case 4:
                              if ((100 - dropflag) > 50) {
                                player_item[6] = 1;
                                itemdrop.play();
                              }
                              else if ((100 - dropflag) > 25) {
                                player_item[5] = 1;
                                itemdrop.play();
                              }
                              break;

                            default:
                              if ((100 - dropflag) > 50) {
                                player_item[2] = 1;
                                itemdrop.play();
                              }
                              else if ((100 - dropflag) > 25) {
                                player_item[0] = 1;
                                itemdrop.play();
                              }
                              break;
                          }

                          break;
                        }
                        app_env.update();
                      }
                    }

										break;
									}
								}
							}

							// いいえ
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 20, button_width, button_height,
									button, angle);

								if (on_mouse) {
									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										break;
									}
								}
							}

							app_env.update();
						}
					}
				}
			}

			// セーブ
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 5, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// ここまでのゲーム内容を保存します。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 10, telop_width, telop_height,
						system, Color(0, 0, 0));
					// 注意：すでにデータがある場合は上書きします。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 11, telop_width, telop_height,
						system, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						angle_b = angle;

						while (1) {
							if (!app_env.isOpen()) return 0;
							app_env.setupDraw();

							mouse_pos = app_env.mousePosition();
							drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
								0, 0, Window::WIDTH, Window::HEIGHT,
								mainBG, Color(1, 1, 1));
							drawTextureBox(x, y, button_width, button_height,
								0, button_height * 5, button_width, button_height,
								button, angle_b);

							// セーブします。本当によろしいですか？
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
								0, telop_height * 12, telop_width, telop_height,
								system, Color(0, 0, 0));

							// はい
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 19, button_width, button_height,
									button, angle);

								if (on_mouse) {
									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										std::ofstream save("res/Savedata.txt");
                    if (save) {
                      save << flag << std::endl;
                      save << month << std::endl;

											save << player.money << std::endl;
											save << player.fund << std::endl;
											save << player.territory << std::endl;
											save << player.soldier << std::endl;
											save << player.mercenary << std::endl;
											save << player.fame << std::endl;
                      save << player.infamy << std::endl;

                      save << player_fund[0] << std::endl;
                      save << player_fund[1] << std::endl;
                      save << player_fund[2] << std::endl;
                      save << player_fund[3] << std::endl;
                      save << player_fund[4] << std::endl;
                      save << player_fund[5] << std::endl;
                      save << player_fund[6] << std::endl;
                      save << player_fund[7] << std::endl;
                      save << player_fund[8] << std::endl;
                      save << player_fund[9] << std::endl;
                      save << player_fund[10] << std::endl;
                      save << player_fund[11] << std::endl;
                      save << player_fund[12] << std::endl;
                      save << player_fund[13] << std::endl;

                      save << player_item[0] << std::endl;
                      save << player_item[1] << std::endl;
                      save << player_item[2] << std::endl;
                      save << player_item[3] << std::endl;
                      save << player_item[4] << std::endl;
                      save << player_item[5] << std::endl;
                      save << player_item[6] << std::endl;

                      save << enemy.money << std::endl;
                      save << enemy.fund << std::endl;
                      save << enemy.territory << std::endl;
                      save << enemy.soldier << std::endl;
                      save << enemy.mercenary << std::endl;
                      save << enemy.fame << std::endl;
                      save << enemy.infamy << std::endl;
										}
										break;
									}
								}
							}

							// いいえ
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 7);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 20, button_width, button_height,
									button, angle);

								if (on_mouse) {
									if (app_env.isPushButton(Mouse::LEFT)) {
										app_env.flushInput();
										break;
									}
								}
							}

							app_env.update();
						}
					}
				}
			}

			app_env.update();
		}

    // 勝利条件達成
    {
      mainTM.stop();
      if (flag != 5) {
        Texture bgBlack("res/png/BG0_Black.png");
        BG_fadeout(app_env, mainBG, 1.0, 0, 0.02);
      }
      clearCall(app_env, flag);
      flag += 1;
      player.fund += 500;

      if (player.infamy > 0) {
        player.fame += 2;
        player.infamy -= 2;
      }
      else {
        player.fame += 5;
      }
      
      if (flag == 2 && player_fund[0] == 0) {
        player_fund[0] = 1;
        player.fund += fundinfo[0] / 2;
      }

      player.money += (player.fund / 10);

      if (flag == 5) {
        if (month > 36.0) {
          break;
        }
        else {
          ex_stage = true;
        }
      }
      else if (flag > 5) {
        break;
      }
    }
	}

  // エンディング処理
  {
    Texture titleED("res/png/Sp4_Ending.png");
    Texture telopED("res/png/T08_Ending.png");
    Texture slideED("res/png/T08_Ending_telop.png");

    Media ending("res/wav/Event_Ending.wav");
    ending.looping(true);
    ending.gain(0.1);
    ending.play();

    float time = 0;
    int click_checker = 0;
    float animetime = 0;
    float teloptime = 0;
    bool stop = false;
    bool chara = false;
    bool telop = false;
    bool story_end = false;

    BG_fullslide(app_env, blackBG, eventBG);

    while (1) {
      if (!app_env.isOpen()) return 0;
      app_env.setupDraw();

      if (!stop) {
        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          click_checker += 1;
          chara = false;
          telop = false;
        }
      }

      if (click_checker < 10) {
        drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          eventBG, Color(1, 1, 1));
      }
      else {
        drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
          0, 0, Window::WIDTH, Window::HEIGHT,
          titleED, Color(1, 1, 1));
      }

      switch (click_checker) {
        case 0:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
            0, telop_height * 0, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        case 1:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
            0, telop_height * 1, telop_width, telop_height * 2,
            telopED, Color(0, 0, 0));
          break;

        case 2:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
            0, telop_height * 3, telop_width, telop_height * 2,
            telopED, Color(0, 0, 0));
          break;

        case 3:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
            0, telop_height * 5, telop_width, telop_height * 2,
            telopED, Color(0, 0, 0));
          break;

        case 4:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
            0, telop_height * 7, telop_width, telop_height * 2,
            telopED, Color(0, 0, 0));
          break;

        case 5:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
            0, telop_height * 9, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        case 6:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 21, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
            0, telop_height * 10, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        case 7:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
            0, telop_height * 11, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        case 8:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
            0, telop_height * 12, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        case 9:
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
            0, telop_height * 13, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        case 10:
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2) + (512 -365), Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            blackBG, Color(1, 1, 1));
          drawTextureBox((-Window::WIDTH / 2), (-Window::HEIGHT / 2), Window::WIDTH, Window::HEIGHT - 365,
            0, 365, Window::WIDTH, Window::HEIGHT - 365,
            eventBG, Color(1, 1, 1));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height * 1,
            0, telop_height * 15, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 1,
            0, telop_height * 16, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        case 11:
          drawTextureBox((-Window::WIDTH / 2), (Window::HEIGHT / 2) - (512 - 365), Window::WIDTH, Window::HEIGHT - 365,
            0, 365, Window::WIDTH, Window::HEIGHT - 365,
            eventBG, Color(1, 1, 1));
          drawTextureBox(-telop_width / 2, (Window::HEIGHT / 2) - (telop_height * 2), telop_width, telop_height,
            0, telop_height * 20, telop_width, telop_height,
            telopED, Color(0, 0, 0));
          drawTextureBox(-telop_width / 2, (Window::HEIGHT / 2) - (telop_height * 3), telop_width, telop_height * 1,
            0, telop_height * 18, telop_width, telop_height * 1,
            telopED, Color(0, 0, 0));
          break;

        default:
          if (!stop) {
            BG_fadeout(app_env, titleED, 1.0 , 0.5, 0.02);
            stop = true;
          }

          if (time < Window::HEIGHT + 6) {
            time += 0.2;
          }

          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            titleED, Color(0.5, 0.5, 0.5));
          drawTextureBox(-Window::WIDTH / 2, (-Window::HEIGHT * 1.5) - 6 + time, Window::WIDTH, Window::HEIGHT,
            0, 0, Window::WIDTH, Window::HEIGHT,
            slideED, Color(1, 1, 1));
          break;
      }

      app_env.update();
    }
  }

	return 0;
}

// 背景スライド（全画面）
void BG_fullslide(AppEnv& app_env, Texture& tex_before, Texture& tex_after) {
	float x;
	for (x = 512; x >= 0; x -= 4) {
		if (!app_env.isOpen()) return;

		if (app_env.isPushButton(Mouse::LEFT)) {
			x = 0;
			app_env.flushInput();
		}

		app_env.setupDraw();
		drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
			0, 0, Window::WIDTH, Window::HEIGHT,
			tex_before, Color(1, 1, 1));
		drawTextureBox((-Window::WIDTH / 2) + x, -Window::HEIGHT / 2, Window::WIDTH - x, Window::HEIGHT,
			x, 0, Window::WIDTH - x, Window::HEIGHT,
			tex_after, Color(1, 1, 1));
		app_env.update();
	};
}

// 背景スライド（背景のみ）
void BG_slide(AppEnv& app_env, Texture& tex_before, Texture& tex_after) {
	float x;
	for (x = 512; x >= 0; x -= 4) {
		if (!app_env.isOpen()) return;

		if (app_env.isPushButton(Mouse::LEFT)) {
			x = 0;
			app_env.flushInput();
		}

		app_env.setupDraw();
		drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, 365,
			0, 0, Window::WIDTH, 365,
			tex_before, Color(1, 1, 1));
		drawTextureBox((-Window::WIDTH / 2) + x, -Window::HEIGHT / 2, Window::WIDTH - x, 365,
			x, 0, Window::WIDTH - x, 365,
			tex_after, Color(1, 1, 1));
		app_env.update();
	};
}

// 背景フェードアウト
void BG_fadeout(AppEnv& app_env, Texture& texture, float base, float limit, float angle) {
	float fade;
	for (fade = base; fade > limit; fade -= angle) {
		if (!app_env.isOpen()) return;

		if (app_env.isPushButton(Mouse::LEFT)) {
			fade = limit;
			app_env.flushInput();
		}

		app_env.setupDraw();
		drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
			0, 0, Window::WIDTH, Window::HEIGHT,
			texture, Color(fade, fade, fade));
		app_env.update();
	};
}

// 背景フェードイン
void BG_fadein(AppEnv& app_env, Texture& texture, float base, float limit, float angle) {
	float fade;
	for (fade = base; fade < limit; fade += angle) {
		if (!app_env.isOpen()) return;

		if (app_env.isPushButton(Mouse::LEFT)) {
			fade = limit;
			app_env.flushInput();
		}

		app_env.setupDraw();
		drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
			0, 0, Window::WIDTH, Window::HEIGHT,
			texture, Color(fade, fade, fade));
		app_env.update();
	};
}

// 各章タイトル画面呼び出し
void titleCall(AppEnv& app_env, short flag) {
	// 各章タイトル背景
	Texture Eptitle("res/png/Sp5_BG_Title.png");
	Texture Eptelop("res/png/Sp6_BG_Telop.png");
  Texture bgBlack("res/png/BG0_Black.png");
  Texture storybg("res/png/BG1_Castle.png");

	float title_width = 352;
	float title_height = 28;

  BG_fullslide(app_env, bgBlack, Eptitle);
  if (!app_env.isOpen()) return;

  float y = 0;
	float base = title_height * (flag * 2);

  short time = 0;
  while (1) {
    if (!app_env.isOpen()) return;
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      break;
    }

    app_env.setupDraw();

    drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      Eptitle, Color(1, 1, 1));

    // 章の番号
    if (y < title_height) {
      y += 1;
      drawTextureBox(-title_width / 2, Window::HEIGHT / 4 + (title_height - y), title_width, y,
        0, base, title_width, y,
        Eptelop, Color(0.2, 0.1, 0));
    }
    else if (y >= title_height) {
      drawTextureBox(-title_width / 2, Window::HEIGHT / 4, title_width, title_height,
        0, base, title_width, title_height,
        Eptelop, Color(0.2, 0.1, 0));
    }

    // 章タイトル
    if (time > title_height && time <= (title_height * 2)) {
      drawTextureBox(-title_width / 2, Window::HEIGHT / 8 + (title_height - (time - title_height)), title_width, time - title_height,
        0, base + title_height, title_width, time - title_height,
        Eptelop, Color(0.2, 0.1, 0));
    }
    else if (time >= (title_height * 2)) {
      drawTextureBox(-title_width / 2, Window::HEIGHT / 8, title_width, title_height,
        0, base + title_height, title_width, title_height,
        Eptelop, Color(0.2, 0.1, 0));
    }

    app_env.update();

    if (time < 10000)
      time += 1;
  }

	BG_fadeout(app_env, Eptitle, 1.0, 0, 0.02);
	BG_fullslide(app_env, bgBlack, storybg);
	if (!app_env.isOpen()) return;
}

// 各章クリア画面呼び出し
void clearCall(AppEnv& app_env, short flag) {
  // 各章タイトル背景
  Texture Eptitle("res/png/Sp5_BG_Title.png");
  Texture Eptelop("res/png/Sp6_BG_Telop.png");
  Texture bgBlack("res/png/BG0_Black.png");
  Texture storybg("res/png/BG1_Castle.png");

  Media clear("res/wav/se_clear.wav");
  clear.gain(0.1);
  clear.play();

  float title_width = 352;
  float title_height = 28;

  BG_fullslide(app_env, bgBlack, Eptitle);
  if (!app_env.isOpen()) return;

  float y = 0;
  float y0 = 0;
  float base = title_height * (flag * 2);

  short time = 0;
  while (1) {
    if (!app_env.isOpen()) return;
    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      clear.stop();
      break;
    }

    app_env.setupDraw();

    drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      Eptitle, Color(1, 1, 1));

    // 章の番号
    if (y < title_height) {
      y += 1;
      drawTextureBox(-title_width / 2, Window::HEIGHT / 4 + (title_height - y), title_width, y,
        0, base, title_width, y,
        Eptelop, Color(0.2, 0.1, 0));
    }
    else if (y >= title_height) {
      drawTextureBox(-title_width / 2, Window::HEIGHT / 4, title_width, title_height,
        0, base, title_width, title_height,
        Eptelop, Color(0.2, 0.1, 0));
    }

    // 章タイトル
    if (time > title_height && time < (title_height * 2)) {
      drawTextureBox(-title_width / 2, Window::HEIGHT / 8 + (title_height - (time - title_height)), title_width, time - title_height,
        0, base + title_height, title_width, time - title_height,
        Eptelop, Color(0.2, 0.1, 0));
    }
    else if (time >= (title_height * 2)) {
      drawTextureBox(-title_width / 2, Window::HEIGHT / 8, title_width, title_height,
        0, base + title_height, title_width, title_height,
        Eptelop, Color(0.2, 0.1, 0));
    }

    // クリア表示
    if (time > title_height * 2 && time < title_height * 3) {
      y0 = title_height * 2;
      drawTextureBox(-title_width / 2, -Window::HEIGHT / 8 + (title_height - (time - y0)), title_width, time - y0,
        0, title_height * 13, title_width, time - y0,
        Eptelop, Color(1, 1, 0));
    }
    else if (time >= title_height * 3) {
      drawTextureBox(-title_width / 2, -Window::HEIGHT / 8, title_width, title_height,
        0, title_height * 13, title_width, title_height,
        Eptelop, Color(1, 1, 0));
    }

    app_env.update();

    if (time < 10000)
      time += 1;
  }

  BG_fadeout(app_env, Eptitle, 1.0, 0, 0.02);
  if (!app_env.isOpen()) return;
}

// 戦闘前の処理
void duel_standby() {
	// 人望値の計算
	player_popularity = player.fame - player.infamy;
	enemy_popularity = enemy.fame - enemy.infamy;

	// プレイヤーの兵力と攻撃力の計算
	player_pow = player.soldier * 10 + player.mercenary * 5;
	player_pow0 = player_pow;

	// 相手の兵力と攻撃力の計算
	enemy_pow = enemy.soldier * 10 + enemy.mercenary * 5;
	enemy_pow0 = enemy_pow;
}

// 攻撃力上限
void Attack_limit() {
  if (player_Attack > 10000) {
    player_Attack = 10000;
  }
  if (enemy_Attack > 10000) {
    enemy_Attack = 10000;
  }
}

// 兵力ゲージの計算
void duel_setpowGauge() {
	player_powGauge = (player_pow * 100) / player_pow0;
	enemy_powGauge = (enemy_pow * 100) / enemy_pow0;
}

// 戦闘ターン開始時の処理
void duel_turnstart() {
  int val_p = 2 + (player_popularity / 20);
  if (val_p < 0) {
    val_p = 1;
  }
	player_Attack = (player.soldier * val_p) + player.mercenary * 4;
	player_start = player_pow;

  int val_e = 2 + (enemy_popularity / 20);
  if (val_e < 0) {
    val_e = 1;
  }
	enemy_Attack = (enemy.soldier * val_e) + enemy.mercenary * 4;
	enemy_start = enemy_pow;
}

// 戦闘実行
void duel_fight(short player_cmd, short enemy_cmd) {
	switch (enemy_cmd) {

		// 相手：突撃
		case 0:
			// 自分が突撃
			if (player_cmd == 0) {
				player_damage = player_Attack * 1.0;
				enemy_damage = enemy_Attack * 1.0;
			}
			// 自分が守り
			else if (player_cmd == 1) {
				player_damage = player_Attack * 1.0;
				enemy_damage = 0;
			}
			// 自分がゆみ
			else {
				player_damage = player_Attack * 0.5;
				enemy_damage = enemy_Attack * 1.5;
			}
			break;

			// 相手：守り
		case 1:
			// 自分が突撃
			if (player_cmd == 0) {
				player_damage = 0;
				enemy_damage = enemy_Attack * 1.0;
			}
			// 自分が守り
			else if (player_cmd == 1) {
				player_damage = 0;
				enemy_damage = 0;
			}
			// 自分がゆみ
			else {
				player_damage = player_Attack * 1.0;
				enemy_damage = 0;
			}
			break;

			// 相手：ゆみ
		case 2:
			// 自分が突撃
			if (player_cmd == 0) {
				player_damage = player_Attack * 1.5;
				enemy_damage = enemy_Attack * 0.5;
			}
			// 自分が守り
			else if (player_cmd == 1) {
				player_damage = 0;
				enemy_damage = enemy_Attack * 1.0;
			}
			// 自分がゆみ
			else {
				player_damage = player_Attack * 0.5;
				enemy_damage = enemy_Attack * 0.5;
			}
			break;
	}

  if (add_attack) {
    player_damage *= 3;
  }

	player_pow -= enemy_damage;
	enemy_pow -= player_damage;
	player_lastcmd = player_cmd;
}

// 戦闘ターン終了後の処理
void duel_turnend() {
	int damage0;

	// プレイヤー側の兵士を減らす
	player_end = player_pow;
	if (player_start != player_end) {
		damage0 = enemy_damage - player.mercenary * 5;

		if (damage0 > 0) {
			player.mercenary = 0;
			damage0 = damage0 - player.soldier * 10;

			if (damage0 > 0) {
				player.soldier = 0;
			}
			else if (damage0 <= 0) {
				damage0 = (-damage0);
				player.soldier = damage0 / 10;
			}
		}
		else if (damage0 <= 0) {
			damage0 = (-damage0);
			player.mercenary = damage0 / 5;
		}
	}

	// 相手側の兵士を減らす
	enemy_end = enemy_pow;
	if (enemy_start != enemy_end) {
		damage0 = player_damage - enemy.mercenary * 5;

		if (damage0 > 0) {
			enemy.mercenary = 0;
			damage0 = damage0 - enemy.soldier * 10;

			if (damage0 > 0) {
				enemy.soldier = 0;
			}
			else if (damage0 <= 0) {
				damage0 = (-damage0);
				enemy.soldier = damage0 / 10;
			}
		}
		else if (damage0 <= 0) {
			damage0 = (-damage0);
			enemy.mercenary = damage0 / 5;
		}
	}
}

// 相手の行動パターン
int duel_enemycommand(short flag) {
	switch (flag) {
		case 0:
			return 0;

		case 1:
			if (player_pow > enemy_pow)
				return 1;
			else
				return 0;

		case 2:
			if (player_pow < enemy_Attack)
				return 0;
			else if (player_pow > enemy_pow)
				return 1;
			else
				return 2;

		case 3:
			if (player_lastcmd == 0)
				return 1;
			else if (player_lastcmd == 2)
				return 0;
			else
				return 2;

		case 4:
			if (player_lastcmd == 0) {
				if (enemy_powGauge > 100)
					return 0;
				return 1;
			}
			else if (player_lastcmd == 1) {
				if (enemy_powGauge > 100)
					return 1;
				return 2;
			}
			else if (player_lastcmd == 2) {
				if (enemy_powGauge > 100)
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

// 兵力ゲージのカラー変更
Color setpowColor(float pow) {
	if (pow == 100) {
		return Color(0.5, 0.5, 1);
	}
	else if (pow < 100 && pow >= 50) {
		return Color(0.5, 1, 0.5);
	}
	else if (pow < 50 && pow >= 25) {
		return Color(1, 1, 0.5);
	}
	else {
		return Color(1, 0.5, 0.5);
	}
}

// 通常戦闘のＢＧＭ設定
Media set_a_bgm(short flag) {
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

// 追撃戦のＢＧＭ設定
Media set_b_bgm(short flag) {
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

// メインテーマのＢＧＭ設定
Media set_main_bgm(short flag) {
  switch (flag) {
    case 0:
      return Media("res/wav/Stage_1.wav");
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

// 兵力増強
void add_soldier(AppEnv& app_env, short genre, short flag) {
	Texture mainBG("res/png/BG2_GrassField.png");
	Texture button("res/png/Me1_MenuButton.png");
	Texture system("res/png/Me2_System.png");

	float x, y;
	float x0, y0;
	Vec2f mouse_pos;
	bool on_mouse;
	Color angle, angle_b(1, 0.8, 0.4);

	bool checker = false;

	x = (-Window::WIDTH / 2) + 2;
	y = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

	while (1) {
		if (!app_env.isOpen()) return;
		app_env.setupDraw();

		mouse_pos = app_env.mousePosition();

		drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
			0, 0, Window::WIDTH, Window::HEIGHT,
			mainBG, Color(1, 1, 1));

		if (!checker) {
			// 何人募集しますか？
			drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
				0, telop_height * 50, telop_width, telop_height,
				system, Color(0, 0, 0));

			// やめる
			on_mouse = on_button(mouse_pos, x, y - (2 + button_height), button_width, button_height);
			angle = setColor(on_mouse);
			drawTextureBox(x, y - (2 + button_height), button_width, button_height,
				0, button_height * 21, button_width, button_height,
				button, angle);

			if (on_mouse) {
				if (app_env.isPushButton(Mouse::LEFT)) {
					app_env.flushInput();
					break;
				}
			}

			// 兵士募集
			if (genre == 0) {
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 9, button_width, button_height,
					button, angle_b);

				// 兵士：１人　10G
				drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
					0, telop_height * 51, telop_width, telop_height,
					system, Color(0, 0, 0));

				// 10人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					if (player.money >= 100) {
						angle = setColor(on_mouse);
					}
					else {
						angle = Color(0.5, 0.5, 0.5);
					}
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 27, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (player.money >= 100) {
							if (app_env.isPushButton(Mouse::LEFT)) {
								app_env.flushInput();
								player.money -= 100;
								player.soldier += 10;
								checker = true;
							}
						}
						else {
							// 資金が足りません。
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 100人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					if (player.money >= 1000) {
						angle = setColor(on_mouse);
					}
					else {
						angle = Color(0.5, 0.5, 0.5);
					}
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 28, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (player.money >= 1000) {
							if (app_env.isPushButton(Mouse::LEFT)) {
								app_env.flushInput();
								player.money -= 1000;
								player.soldier += 100;
								checker = true;
							}
						}
						else {
							// 資金が足りません。
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 1000人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					if (player.money >= 10000) {
						angle = setColor(on_mouse);
					}
					else {
						angle = Color(0.5, 0.5, 0.5);
					}
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 29, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (player.money >= 10000) {
							if (app_env.isPushButton(Mouse::LEFT)) {
								app_env.flushInput();
								player.money -= 10000;
								player.soldier += 1000;
								checker = true;
							}
						}
						else {
							// 資金が足りません。
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}
			} // 兵士募集

			// 義勇兵
			else if (genre == 1) {
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 10, button_width, button_height,
					button, angle_b);

				// 義勇兵：１人　5G
				drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
					0, telop_height * 52, telop_width, telop_height,
					system, Color(0, 0, 0));

				// 10人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					if (player.money >= 50) {
						angle = setColor(on_mouse);
					}
					else {
						angle = Color(0.5, 0.5, 0.5);
					}
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 27, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (player.money >= 50) {
							if (app_env.isPushButton(Mouse::LEFT)) {
								app_env.flushInput();
								player.money -= 50;
								player.mercenary += 10;
								checker = true;
							}
						}
						else {
							// 資金が足りません。
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 100人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					if (player.money >= 500) {
						angle = setColor(on_mouse);
					}
					else {
						angle = Color(0.5, 0.5, 0.5);
					}
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 28, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (player.money >= 500) {
							if (app_env.isPushButton(Mouse::LEFT)) {
								app_env.flushInput();
								player.money -= 500;
								player.mercenary += 100;
								checker = true;
							}
						}
						else {
							// 資金が足りません。
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 1000人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					if (player.money >= 5000) {
						angle = setColor(on_mouse);
					}
					else {
						angle = Color(0.5, 0.5, 0.5);
					}
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 29, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (player.money >= 5000) {
							if (app_env.isPushButton(Mouse::LEFT)) {
								app_env.flushInput();
								player.money -= 5000;
								player.mercenary += 1000;
								checker = true;
							}
						}
						else {
							// 資金が足りません。
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}
			} // 義勇兵

			// 強制兵役
			else {
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 11, button_width, button_height,
					button, angle_b);

				// 10人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					angle = setColor(on_mouse);
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 27, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (app_env.isPushButton(Mouse::LEFT)) {
							app_env.flushInput();
							player.soldier += 10;
							checker = true;
						}
					}
				}

				// 100人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					angle = setColor(on_mouse);
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 28, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (app_env.isPushButton(Mouse::LEFT)) {
							app_env.flushInput();
							player.soldier += 100;
							player.infamy += 1;
							checker = true;
						}
					}
				}

				// 1000人
				{
					x0 = x + button_width + 2;
					y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

					on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
					angle = setColor(on_mouse);
					drawTextureBox(x0, y0, button_width, button_height,
						0, button_height * 29, button_width, button_height,
						button, angle);

					if (on_mouse) {
						if (app_env.isPushButton(Mouse::LEFT)) {
							app_env.flushInput();
							player.soldier += 1000;
							player.infamy += 10;
							checker = true;
						}
					}
				}
			} // 強制兵役
		}

		else {
			// 募集しました。
			drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
				0, telop_height * 53, telop_width, telop_height,
				system, Color(0, 0, 0));

			if (app_env.isPushButton(Mouse::LEFT)) {
				app_env.flushInput();
				month += 0.1;
				spy_time += 1;
        if (sabotage) {
          sabo_time -= 1;
        }
        else {
          int value;
          switch (flag) {
            case 0:
              break;

            case 1:
              if (enemy.soldier < 200) {
                value = 200 - enemy.soldier;
                if (value > 100) {
                  value = 100;
                }
                if (enemy.money >= value * 10) {
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
                else {
                  value = enemy.money / 10;
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
              }
              break;

            case 2:
              if (enemy.soldier < 200) {
                value = 200 - enemy.soldier;
                if (value > 100) {
                  value = 100;
                }
                if (enemy.money >= value * 10) {
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
                else {
                  value = enemy.money / 10;
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
              }
              break;

            case 3:
              if (enemy.money >= 5000) {
                enemy.money -= 5000;
                enemy.soldier += 500;
              }
              else {
                value = enemy.money / 10;
                enemy.money -= value * 10;
                enemy.soldier += value;
              }
              break;

            case 4:
              if (enemy.soldier > 20000) {
                if (enemy.money >= 5000) {
                  enemy.money -= 5000;
                  enemy.soldier += 500;
                }
                else {
                  value = enemy.money / 10;
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
              }
              else if (enemy.soldier > 10000) {
                if (enemy.money >= 10000) {
                  enemy.money -= 10000;
                  enemy.soldier += 1000;
                }
                else {
                  value = enemy.money / 10;
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
              }
              else {
                value = 20000 - enemy.soldier;
                if (enemy.money > value * 10) {
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
                else {
                  value = enemy.money / 10;
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
              }
              break;

            default:
              value = 10000 - enemy.soldier;
              if (value > 0) {
                if (enemy.money >= (value * 10) * 2) {
                  enemy.money -= (value * 10) * 2;
                  enemy.soldier += value * 2;
                }
                else {
                  value = enemy.money / 10;
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
              }
              else {
                if (enemy.money >= 10000) {
                  enemy.money -= 10000;
                  enemy.soldier += 1000;
                }
                else {
                  value = enemy.money / 10;
                  enemy.money -= value * 10;
                  enemy.soldier += value;
                }
              }
              break;
          }
        }
				break;
			}
		}

		app_env.update();
	}
}

// 買い物の最終確認
int buy(AppEnv& app_env) {
	Texture mainBG("res/png/BG2_GrassField.png");
	Texture button("res/png/Me1_MenuButton.png");
	Texture system("res/png/Me2_System.png");

	Vec2f mouse_pos = app_env.mousePosition();
	Color angle;
	Color angle_b(1, 0.8, 0.4);

	float x, y;
	bool on_mouse;

	while (1) {
		if (!app_env.isOpen()) return 0;
		app_env.setupDraw();

		mouse_pos = app_env.mousePosition();

		drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
			0, 0, Window::WIDTH, Window::HEIGHT,
			mainBG, Color(1, 1, 1));

		x = (-Window::WIDTH / 2) + 2;
		y = (Window::HEIGHT / 2) - ((2 + button_height) * 1);
		drawTextureBox(x, y, button_width, button_height,
			0, button_height * 3, button_width, button_height,
			button, angle_b);

		// 買取ります。よろしいですか？
		drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
			0, telop_height * 78, telop_width, telop_height,
			system, Color(0, 0, 0));

		// Yes
		{
			x = (-Window::WIDTH / 2) + 2;
			y = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

			on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
			angle = setColor(on_mouse);
			drawTextureBox(x, y, button_width, button_height,
				0, button_height * 19, button_width, button_height,
				button, angle);

			if (on_mouse) {
				if (app_env.isPushButton(Mouse::LEFT)) {
					app_env.flushInput();
					month += 0.1;
					return 1;
				}
			}
		}

		// No
		{
			x = (-Window::WIDTH / 2) + 2;
			y = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

			on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
			angle = setColor(on_mouse);
			drawTextureBox(x, y, button_width, button_height,
				0, button_height * 20, button_width, button_height,
				button, angle);

			if (on_mouse) {
				if (app_env.isPushButton(Mouse::LEFT)) {
					app_env.flushInput();
					return 2;
				}
			}
		}

		app_env.update();
	}

	return 0;
}

// 売却
int sale(AppEnv& app_env) {
	Texture mainBG("res/png/BG2_GrassField.png");
	Texture button("res/png/Me1_MenuButton.png");
	Texture system("res/png/Me2_System.png");
	Texture buytxt("res/png/Me5_BuyTxt.png");
	Texture s_menu("res/png/Me6_fundlist.png");

	Vec2f mouse_pos = app_env.mousePosition();
	Color angle;
	Color angle_b(1, 0.8, 0.4);

	float x, y;
	float x0, y0;
	bool on_mouse;
	short checker = -1;

	while (1) {
		if (!app_env.isOpen()) return 0;
		app_env.setupDraw();

		mouse_pos = app_env.mousePosition();

		drawTextureBox((-Window::WIDTH / 2), -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
			0, 0, Window::WIDTH, Window::HEIGHT,
			mainBG, Color(1, 1, 1));

		x = (-Window::WIDTH / 2) + 2;
		y = (Window::HEIGHT / 2) - ((2 + button_height) * 1);
		drawTextureBox(x, y, button_width, button_height,
			0, button_height * 8, button_width, button_height,
			button, angle_b);

		if (checker == -1) {
			// どれを売却しますか？
			drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
				0, telop_height * 45, telop_width, telop_height,
				system, Color(0, 0, 0));

			// やめる
			on_mouse = on_button(mouse_pos, x, y - (2 + button_height), button_width, button_height);
			angle = setColor(on_mouse);
			drawTextureBox(x, y - (2 + button_height), button_width, button_height,
				0, button_height * 21, button_width, button_height,
				button, angle);

			if (on_mouse) {
				if (app_env.isPushButton(Mouse::LEFT)) {
					app_env.flushInput();
					return 2;
				}
			}

			// 小物細工
			if (player_item[0] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 0, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// 小さいがキレイな装飾品、数点。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 26, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// 城を出るときに持ってきたもの。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 27, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 0;
					}
				}
			}

			// 勇ましい武具
			if (player_item[1] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 1, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// 男らしさあふれる武具。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 28, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// とても汗臭い。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 29, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 1;
					}
				}
			}

			// 高級ワイン
			if (player_item[2] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 2, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// 地元の果物を使った果実酒。名産品。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 30, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// とても上品な味わいのワイン。らしい。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 31, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 2;
					}
				}
			}

			// 立派なかんむり
			if (player_item[3] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 3, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// すごく立派なつくりのかんむり。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 32, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// かぶれば、誰でも王様のようになれそうな雰囲気。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 33, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 3;
					}
				}
			}

			// ゴテゴテした服
			if (player_item[4] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 4, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// 飾りすぎてゴテゴテしてる服。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 34, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// 持ち主のセンスが知れる、目の毒な服。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 35, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 4;
					}
				}
			}

			// 英雄の武具
			if (player_item[5] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 5, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// 見事な作りの立派な武具。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 36, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// 見てるだけで、勇気がわくような装備。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 37, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 5;
					}
				}
			}

			// 黒歴史ノート
			if (player_item[6] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 7);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 6, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// 誰にも知られたくない、秘密いっぱいのノート。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 38, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// 肌身離さず持っていたが、それが仇になったようだ。
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 39, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 6;
					}
				}
      }

      // 過去に所有していたが、売却済み
      {
        angle = Color(0.5, 0.5, 0.5);
        for (int i = 0; i < 7; i += 1) {
          if (player_item[i] == 2) {
            x0 = x + button_width + 2;
            y0 = (Window::HEIGHT / 2) - ((2 + button_height) * (i + 1));
            drawTextureBox(x0, y0, buybutton_width, button_height,
              0, button_height * i, buybutton_width, button_height,
              s_menu, angle);

            on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
            if (on_mouse) {
              // すでに売却しています。
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                system, Color(0, 0, 0));
            }
          }
        }
      }
		} // item end

		else {
			// 売却します。よろしいですか？
			drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
				0, telop_height * 46, telop_width, telop_height,
				system, Color(0, 0, 0));

			// Yes
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 19, button_width, button_height,
					button, angle);

				if (on_mouse) {
					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						player.money += iteminfo[checker];
						player_item[checker] = 2;
						return 1;
					}
				}
			}

			// No
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 20, button_width, button_height,
					button, angle);

				if (on_mouse) {
					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = -1;
					}
				}
			}
		}

		app_env.update();
	}

	return 0;
}

// カーソルがボタン上にあるか
bool on_button(Vec2f pos, float x, float y, float tex_width, float tex_height) {
	if (x < pos.x() && pos.x() < x + tex_width &&
		y < pos.y() && pos.y() < y + tex_height) {
		return true;
	}
	return false;
}

// ボタンカラー変更
Color setColor(bool on_mouse) {
	if (on_mouse) {
		return Color(1, 0.8, 0.4);
	}
	return Color(1, 1, 1);
}

// チュートリアル（プロローグ専用、本編）
void tutorial(AppEnv& app_env) {
  Texture mainBG("res/png/BG2_GrassField.png");
  Texture eventP("res/png/T02_Prologue_Event.png");
  Texture button("res/png/Me1_MenuButton.png");
  Texture system("res/png/Me2_System.png");
  Texture battle("res/png/Me3_Battle.png");
  Texture chara6("res/png/06_Rival_C.png");

  Media tutorial("res/wav/Event_tutorial.wav");
  tutorial.looping(true);
  tutorial.gain(0.1);
  Media se("res/wav/se_win.wav");
  se.gain(0.1);
  Media bgm = set_a_bgm(1);
  bgm.looping(true);
  bgm.gain(0.1);

  Vec2f pos;
  bool on_mouse;
  bool se_on = false;
  Color angle;

  float x, y;
  float x0, y0;
  int time = 0;
  short tutorial_flag = 0;
  short genre;
  short guarantee;

  tutorial.play();
  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    pos = app_env.mousePosition();

    // 背景
    drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      mainBG, Color(1, 1, 1));

    x = (-Window::WIDTH / 2) + 2;
    y = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

    if (tutorial_flag == 0 || tutorial_flag == 3) {
      drawTextureBox(x, y, button_width, button_height,
        0, button_height * 0, button_width, button_height,
        button, Color(0.5, 0.5, 0.5));

      drawTextureBox(x, y - (2 + button_height) * 1, button_width, button_height,
        0, button_height * 1, button_width, button_height,
        button, Color(0.5, 0.5, 0.5));

      drawTextureBox(x, y - (2 + button_height) * 2, button_width, button_height,
        0, button_height * 2, button_width, button_height,
        button, Color(0.5, 0.5, 0.5));

      drawTextureBox(x, y - (2 + button_height) * 3, button_width, button_height,
        0, button_height * 3, button_width, button_height,
        button, Color(0.5, 0.5, 0.5));

      drawTextureBox(x, y - (2 + button_height) * 4, button_width, button_height,
        0, button_height * 4, button_width, button_height,
        button, Color(0.5, 0.5, 0.5));

      drawTextureBox(x, y - (2 + button_height) * 5, button_width, button_height,
        0, button_height * 5, button_width, button_height,
        button, Color(0.5, 0.5, 0.5));
    }

    switch (tutorial_flag) {
      case 0:
        on_mouse = on_button(pos, x, y - (2 + button_height) * 1, button_width, button_height);
        angle = setColor(on_mouse);
        drawTextureBox(x, y - (2 + button_height) * 1, button_width, button_height,
          0, button_height * 1, button_width, button_height,
          button, angle);

        // 兵力増強を選んでください。
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 23, telop_width, telop_height,
          eventP, Color(0, 0, 0));

        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            tutorial_flag += 1;
          }
        }

        break;

      case 1:
        drawTextureBox(x, y - (2 + button_height) * 1, button_width, button_height,
          0, button_height * 1, button_width, button_height,
          button, Color(1, 0.8, 0.4));
        // 強制兵役ボタン（選択不可）
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 3, button_width, button_height,
          0, button_height * 11, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));

        // 兵士募集
        on_mouse = on_button(pos, x + (2 + button_width) * 1, y - (2 + button_height) * 1, button_width, button_height);
        angle = setColor(on_mouse);
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 1, button_width, button_height,
          0, button_height * 9, button_width, button_height,
          button, angle);

        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            tutorial_flag += 1;
            genre = 0;
            break;
          }
        }

        // 義勇兵
        on_mouse = on_button(pos, x + (2 + button_width) * 1, y - (2 + button_height) * 2, button_width, button_height);
        angle = setColor(on_mouse);
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 2, button_width, button_height,
          0, button_height * 10, button_width, button_height,
          button, angle);

        // 兵士募集か義勇兵を選んでください。
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 24, telop_width, telop_height,
          eventP, Color(0, 0, 0));
        // ※義勇兵に関する注意
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 25, telop_width, telop_height * 2,
          eventP, Color(0, 0, 0));

        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            tutorial_flag += 1;
            genre = 1;
            break;
          }
        }

        break;

      case 2:
        if (genre == 0) {
          drawTextureBox(x, y - (2 + button_height) * 1, button_width, button_height,
            0, button_height * 9, button_width, button_height,
            button, Color(1, 0.8, 0.4));
          guarantee = 10;
        }
        else {
          drawTextureBox(x, y - (2 + button_height) * 1, button_width, button_height,
            0, button_height * 10, button_width, button_height,
            button, Color(1, 0.8, 0.4));
          guarantee = 5;
        }

        // 募集人数を選んでください。
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 27, telop_width, telop_height,
          eventP, Color(0, 0, 0));
        // 1000人ボタン（選択不可）
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 3, button_width, button_height,
          0, button_height * 29, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));

        // 10人
        on_mouse = on_button(pos, x + (2 + button_width) * 1, y - (2 + button_height) * 1, button_width, button_height);
        if (player.money >= guarantee * 10) {
          angle = setColor(on_mouse);
        }
        else {
          angle = Color(0.5, 0.5, 0.5);
          on_mouse = false;
        }
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 1, button_width, button_height,
          0, button_height * 27, button_width, button_height,
          button, angle);

        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            player.money -= guarantee * 10;
            if (genre == 0) {
              player.soldier += 10;
            }
            else {
              player.mercenary += 10;
            }
            tutorial_flag += 1;
            break;
          }
        }

        // 100人
        on_mouse = on_button(pos, x + (2 + button_width) * 1, y - (2 + button_height) * 2, button_width, button_height);
        if (player.money >= guarantee * 100) {
          angle = setColor(on_mouse);
        }
        else {
          angle = Color(0.5, 0.5, 0.5);
          on_mouse = false;
        }
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 2, button_width, button_height,
          0, button_height * 28, button_width, button_height,
          button, angle);

        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            player.money -= guarantee * 100;
            if (genre == 0) {
              player.soldier += 100;
            }
            else {
              player.mercenary += 100;
            }
            tutorial_flag += 1;
            break;
          }
        }

        break;

      case 3:
        // 侵攻を選んでください。
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 28, telop_width, telop_height,
          eventP, Color(0, 0, 0));

        on_mouse = on_button(pos, x, y - (2 + button_height) * 4, button_width, button_height);
        angle = setColor(on_mouse);
        drawTextureBox(x, y - (2 + button_height) * 4, button_width, button_height,
          0, button_height * 4, button_width, button_height,
          button, angle);

        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            tutorial_flag += 1;
            tutorial.stop();
            bgm.play();
          }
        }

        break;

      case 4:
        time += 1;
        if ((time / 300) % 2) {
          // ヒント
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 2,
            0, telop_height * 36, telop_width, telop_height * 2,
            eventP, Color(0, 0, 0));
          // ヒント２
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
            0, telop_height * 39, telop_width, telop_height,
            eventP, Color(0, 0, 0));
        }
        else {
          // 戦闘開始。攻撃手段を選んでください。
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 2,
            0, telop_height * 29, telop_width, telop_height * 2,
            eventP, Color(0, 0, 0));
        }

        // キャラクター表示（プレイヤー側）
        {
          x0 = -chara_width * 4;
          y0 = -chara_height * 1;

          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              drawTextureBox(x0 - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        // キャラクター表示（相手側）
        {
          x0 = chara_width * 3;
          y0 = -chara_height * 1;

          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              drawTextureBox(x0 + ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        // 突撃（選択不可）
        drawTextureBox(x, (Window::HEIGHT / 2) - 358 + (2 + button_height) * 4, button_width, button_height,
          0, button_height * 22, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));
        // ゆみ（選択不可）
        drawTextureBox(x, (Window::HEIGHT / 2) - 358 + (2 + button_height) * 2, button_width, button_height,
          0, button_height * 24, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));

        // 守り
        on_mouse = on_button(pos, x, (Window::HEIGHT / 2) - 358 + (2 + button_height) * 3, button_width, button_height);
        angle = setColor(on_mouse);
        drawTextureBox(x, (Window::HEIGHT / 2) - 358 + (2 + button_height) * 3, button_width, button_height,
          0, button_height * 23, button_width, button_height,
          button, angle);

        if (on_mouse) {
          if (app_env.isPushButton(Mouse::LEFT)) {
            app_env.flushInput();
            tutorial_flag += 1;
            time = 0;
          }
        }

        break;

      case 5:
        // 攻撃テロップ表示
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 15, telop_width, telop_height,
          battle, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height,
          0, telop_height * 17, telop_width, telop_height,
          battle, Color(0, 0, 0));

        // キャラクター表示（プレイヤー側）
        {
          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              x0 = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j);
              y0 = (-chara_height * 1) + chara_height * (2 - i * 0.5);

              drawTextureBox(x0, y0, chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              // 盾の表示
              drawTextureBox(x0 + 4 + (16 * (1 + i) * 0.125), y0 + 4 + (16 * (1 + i) * 0.1), 16, 16,
                4 + (chara_width * 3) + (16 * 1), 0, 16, 16,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        // キャラクター表示（相手側）
        {
          float i, j;
          float angle0 = 0;
          float alpha = 1;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {

              if (time < 60) {
                x0 = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j);
                y0 = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              }
              else if (time > 60 && time <= 75) {
                x0 = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j) - ((time * 8) - 480);
                y0 = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              }
              else if (time > 75 && time <= 105) {
                x0 = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j) + ((time * 4) - 420);
                y0 = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              }
              else {
                x0 = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j);
                y0 = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              }

              drawTextureBox(x0, y0, chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              // 武器の表示
              drawTextureBox(x0 + 4 + (16 * (1 + i) * 0.125), y0 + 4 + (16 * (1 + i) * 0.125), 16, 16,
                4 + (chara_width * 3) + (16 * 0), 0, 16, 16,
                chara6, Color(1, 0.5, 0.5), M_PI / 2, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

              if (time > 75 && time <= 105) {
                if (j < 3) {
                  angle0 += 1;
                  alpha = (1 - ((time - 75) * 0.05));
                  drawTextureBox(x0 / 2, y0 + 16, 32, 32,
                    4 + (chara_width * 3), 32, 32, 32,
                    chara6, Color(1, 1, 1, alpha), std::sin(angle0 * 15), Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(16, 16));
                  drawTextureBox((x0 / 2) + (time - 75), y0 + 16, 32, 32,
                    4 + (chara_width * 3) + 32, 32, 32, 32,
                    chara6, Color(1, 1, 1, alpha), std::sin(angle0 * 15), Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(16, 16));
                }
              }
            }
          }
        }

        if (time < 180) {
          time += 1;
        }
        else {
          tutorial_flag += 1;
        }

        break;

      case 6:
        // 攻撃テロップ表示
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 15, telop_width, telop_height,
          battle, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height,
          0, telop_height * 17, telop_width, telop_height,
          battle, Color(0, 0, 0));

        // キャラクター表示（プレイヤー側）
        {
          x0 = -chara_width * 4;
          y0 = -chara_height * 1;

          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              drawTextureBox(x0 - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        // キャラクター表示（相手側）
        {
          x0 = chara_width * 3;
          y0 = -chara_height * 1;

          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              drawTextureBox(x0 + ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        // プレイヤーの攻撃力計算
        {
          short i, skip = 0;        // 画像表示位置計算、他
          short tex_x, tex_y;       // 画像切り出し用
          int val = player.soldier * (2 + (player.fame - player.infamy) / 20) + player.mercenary * 4;

          // ステータス情報を取得
          int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i 桁が０でない
            if (damage[i] != 0) {
              tex_x = number_width * damage[i];
              tex_y = telop_height * 82;
            }

            // i 桁が０だったとき
            else {
              // 配列の最初
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // それ以外
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // 現在参照している桁より上の桁がすべて０か確認
                if (i == 1 && damage[i - 1] == 0) {}
                else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                // ０ではなかったとき、スキップしない
                else {
                  skip -= 1;
                  tex_y = telop_height * 82;
                }
              }
            }

            drawTextureBox((-telop_width / 2) + ((1 + number_width) * i), (-Window::HEIGHT / 2) + (telop_height * 4), number_width, telop_height,
              tex_x, tex_y, number_width, telop_height,
              system, Color(0, 0, 0));
          }
          drawTextureBox((-telop_width / 2) + number_width * 7, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
            0, telop_height * 21, telop_width, telop_height,
            battle, Color(0, 0, 0));
        }

        // 相手の攻撃力計算
        {
          short i, skip = 0;        // 画像表示位置計算、他
          short tex_x, tex_y;       // 画像切り出し用
          int val = 0;

          // ステータス情報を取得
          int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i 桁が０でない
            if (damage[i] != 0) {
              tex_x = number_width * damage[i];
              tex_y = telop_height * 82;
            }

            // i 桁が０だったとき
            else {
              // 配列の最初
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // それ以外
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // 現在参照している桁より上の桁がすべて０か確認
                if (i == 1 && damage[i - 1] == 0) {}
                else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                // ０ではなかったとき、スキップしない
                else {
                  skip -= 1;
                  tex_y = telop_height * 82;
                }
              }
            }

            drawTextureBox((-telop_width / 2) + ((1 + number_width) * i), (-Window::HEIGHT / 2) + (telop_height * 2), number_width, telop_height,
              tex_x, tex_y, number_width, telop_height,
              system, Color(0, 0, 0));
          }
          drawTextureBox((-telop_width / 2) + number_width * 7, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
            0, telop_height * 22, telop_width, telop_height,
            battle, Color(0, 0, 0));
        }

        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          tutorial_flag += 1;
        }

        break;

      case 7:
        // 戦闘ルールの説明
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 3,
          0, telop_height * 40, telop_width, telop_height * 3,
          eventP, Color(0, 0, 0));

        // キャラクター表示（プレイヤー側）
        {
          x0 = -chara_width * 4;
          y0 = -chara_height * 1;

          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              drawTextureBox(x0 - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        // キャラクター表示（相手側）
        {
          x0 = chara_width * 3;
          y0 = -chara_height * 1;

          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              drawTextureBox(x0 + ((chara_width * (1 + i * 0.25)) * j), y0 + chara_height * (2 - i * 0.5), chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          tutorial_flag += 1;
        }

        break;

      case 8:
        // 戦闘ルールの説明
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 4,
          0, telop_height * 31, telop_width, telop_height * 4,
          eventP, Color(0, 0, 0));

        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          tutorial_flag += 1;
        }

        break;

      case 9:
        // 兵士たちはおとなしくなった
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 2,
          0, telop_height * 44, telop_width, telop_height * 2,
          eventP, Color(0, 0, 0));

        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          tutorial_flag += 1;
          bgm.stop();
        }

        break;

      case 10:
        if (!se.isPlaying() && !se_on) {
          se.play();
          se_on = true;
        }

        // 勝利
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 8, telop_width, telop_height,
          battle, Color(0, 0, 0));

        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          tutorial_flag += 1;
        }

        break;

      default:
        break;
    }

    if (tutorial_flag > 10) {
      break;
    }

    app_env.update();
  }
}

// 戦闘中のメッセージ表示とアニメーション
void battleAnime(AppEnv& app_env, short player_cmd, short enemy_cmd, short count) {
  Texture mainBG("res/png/BG2_GrassField.png");
  Texture system("res/png/Me2_System.png");
  Texture battle("res/png/Me3_Battle.png");
  Texture chara6("res/png/06_Rival_C.png");

  float i, j;
  float x, y;
  float x0, y0;
  float x1, y1 = 0;
  float angle = 0;
  float alpha = 1;
  int time = 0;

  while (1) {
    if (!app_env.isOpen()) return;
    app_env.setupDraw();

    // 背景
    if (count == -1) {
      drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 0.6, 0.2));
      drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT - 360,
        0, 360, Window::WIDTH, Window::HEIGHT - 360,
        mainBG, Color(1, 1, 1));
    }
    else {
      drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
        0, 0, Window::WIDTH, Window::HEIGHT,
        mainBG, Color(1, 1, 1));
    }

    // プレイヤーの行動
    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
      0, telop_height * (14 + player_cmd), telop_width, telop_height,
      battle, Color(0, 0, 0));

    // 相手の行動
    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height,
      0, telop_height * (17 + enemy_cmd), telop_width, telop_height,
      battle, Color(0, 0, 0));

    if (time < 180) {
      time += 1;
    }

    // プレイヤー側のアニメーション
    {
      for (i = 0; i < 5; i += 1) {
        for (j = 0; j < 5; j += 1) {
          // 突撃
          // 武器と一緒にキャラクターも移動
          if (player_cmd == 0) {
            if (time > 60 && time <= 75) {
              x = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j) + (time * 8) - 480;
              y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              x0 = x + 8 + (16 * (1 + i) * 0.25);
              y0 = y + 4 + (16 * (1 + i) * 0.1);
            }
            else if (time > 75 && time <= 105) {
              x = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j) - (time * 4) + 420;
              y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              x0 = x + 8 + (16 * (1 + i) * 0.25);
              y0 = y + 4 + (16 * (1 + i) * 0.1);
            }
            else {
              x = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j);
              y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              x0 = x + 8 + (16 * (1 + i) * 0.25);
              y0 = y + 4 + (16 * (1 + i) * 0.1);
            }
          }

          // ゆみ
          // 武器（矢のみ）移動
          else if (player_cmd == 2) {
            x = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j);
            y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
            if (time > 60 && time <= 85) {
              x0 = x + (10 * (1 + i) * 0.125);
              y0 = y + 8 + (16 * (1 + i) * 0.2);
              x1 = x0 + (time * 6) - 360;
            }
            else if (time > 85) {
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
          }

          // 守り
          // そのまま動かない
          else {
            x = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j);
            y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
            x0 = x + 6 + (16 * (1 + i) * 0.1);
            y0 = y + (16 * (1 + i) * 0.1);
          }

          // キャラクター表示（プレイヤー側）
          drawTextureBox(x, y, chara_width, chara_height,
            0, 0, chara_width, chara_height,
            chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          // 剣の表示
          if (player_cmd == 0) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 0), 0, 16, 16,
              chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // 盾の表示
          else if (player_cmd == 1) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 1), 0, 16, 16,
              chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // 弓矢の表示
          else if (player_cmd == 2) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 3), 0, 16, 16,
              chara6, Color(0.5, 1, 1), -(M_PI / 4), Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            drawTextureBox(x1, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 2), y1, 16, 16,
              chara6, Color(0.5, 1, 1), -(M_PI / 4), Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }
        }
      }
    }

    // 相手側のアニメーション
    {
      for (i = 0; i < 5; i += 1) {
        for (j = 0; j < 5; j += 1) {
          // 突撃
          // 武器と一緒にキャラクターも移動
          if (enemy_cmd == 0) {
            if (time > 60 && time <= 75) {
              x = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j) - ((time * 8) - 480);
              y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              x0 = x + 4 + (16 * (1 + i) * 0.125);
              y0 = y + 4 + (16 * (1 + i) * 0.125);
            }
            else if (time > 75 && time <= 105) {
              x = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j) + ((time * 4) - 420);
              y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              x0 = x + 4 + (16 * (1 + i) * 0.125);
              y0 = y + 4 + (16 * (1 + i) * 0.125);
            }
            else {
              x = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j);
              y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
              x0 = x + 4 + (16 * (1 + i) * 0.125);
              y0 = y + 4 + (16 * (1 + i) * 0.125);
            }
          }

          // ゆみ
          // 武器（矢のみ）移動
          else if (enemy_cmd == 2) {
            x = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j);
            y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
            if (time > 60 && time <= 85) {
              x0 = x + 20 + (16 * (1 + i) * 0.25);
              y0 = y + 8 + (16 * (1 + i) * 0.2);
              x1 = x0 - ((time * 6) - 360);
            }
            else if (time > 85) {
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
          }

          // 守り
          // そのまま動かない
          else {
            x = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j);
            y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
            x0 = x - (10 * (1 + i) * 0.125);
            y0 = y + (16 * (1 + i) * 0.1);
          }

          // キャラクター表示（相手側）
          drawTextureBox(x, y, chara_width, chara_height,
            0, 0, chara_width, chara_height,
            chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          // 剣の表示
          if (enemy_cmd == 0) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 0), 0, 16, 16,
              chara6, Color(1, 0.5, 0.5), M_PI / 2, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // 盾の表示
          else if (enemy_cmd == 1) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 1), 0, 16, 16,
              chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // 弓矢の表示
          else if (enemy_cmd == 2) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 3), 0, 16, 16,
              chara6, Color(1, 0.5, 0.5), (M_PI / 4) * 3, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            drawTextureBox(x1, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 2), y1, 16, 16,
              chara6, Color(1, 0.5, 0.5), (M_PI / 4) * 3, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }
        }
      }
    }

    // プレイヤーの攻撃力計算
    {
      short i, skip = 0;        // 画像表示位置計算、他
      short tex_x, tex_y;       // 画像切り出し用
      int val = player_damage;

      // ステータス情報を取得
      int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

      for (i = 0; i < 6; i += 1) {
        // i 桁が０でない
        if (damage[i] != 0) {
          tex_x = number_width * damage[i];
          tex_y = telop_height * 82;
        }

        // i 桁が０だったとき
        else {
          // 配列の最初
          if (i == 0) {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;
          }

          // それ以外
          else {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;

            // 現在参照している桁より上の桁がすべて０か確認
            if (i == 1 && damage[i - 1] == 0) {}
            else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            // ０ではなかったとき、スキップしない
            else {
              skip -= 1;
              tex_y = telop_height * 82;
            }
          }
        }

        drawTextureBox((-telop_width / 2) + ((1 + number_width) * i), (-Window::HEIGHT / 2) + (telop_height * 4), number_width, telop_height,
          tex_x, tex_y, number_width, telop_height,
          system, Color(0, 0, 0));
      }
      drawTextureBox((-telop_width / 2) + number_width * 7, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
        0, telop_height * 21, telop_width, telop_height,
        battle, Color(0, 0, 0));
    }

    // 相手の攻撃力計算
    {
      short i, skip = 0;        // 画像表示位置計算、他
      short tex_x, tex_y;       // 画像切り出し用
      int val = enemy_damage;

      // ステータス情報を取得
      int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

      for (i = 0; i < 6; i += 1) {
        // i 桁が０でない
        if (damage[i] != 0) {
          tex_x = number_width * damage[i];
          tex_y = telop_height * 82;
        }

        // i 桁が０だったとき
        else {
          // 配列の最初
          if (i == 0) {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;
          }

          // それ以外
          else {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;

            // 現在参照している桁より上の桁がすべて０か確認
            if (i == 1 && damage[i - 1] == 0) {}
            else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            // ０ではなかったとき、スキップしない
            else {
              skip -= 1;
              tex_y = telop_height * 82;
            }
          }
        }

        drawTextureBox((-telop_width / 2) + ((1 + number_width) * i), (-Window::HEIGHT / 2) + (telop_height * 2), number_width, telop_height,
          tex_x, tex_y, number_width, telop_height,
          system, Color(0, 0, 0));
      }
      drawTextureBox((-telop_width / 2) + number_width * 7, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
        0, telop_height * 22, telop_width, telop_height,
        battle, Color(0, 0, 0));
    }

    if (app_env.isPushButton(Mouse::LEFT)) {
      app_env.flushInput();
      break;
    }

    app_env.update();
  }
}

// ３章を反乱成功でクリア
void _3rdstage_win(AppEnv& app_env) {
  Texture s_telop("res/png/T05_3rdStage_Story.png");
  Texture n_telop("res/png/T02_Prologue_Story.png");
  Texture storyBG("res/png/BG1_Castle.png");
  Texture chara5("res/png/05_Rival_B.png");
  Texture chara6("res/png/06_Rival_C.png");

  Media castle("res/wav/Event_Mainthema.wav");
  castle.looping(true);
  castle.gain(0.1);

  int click_checker = 0;
  float animetime = 0;
  float teloptime = 0;
  bool stop = false;
  bool chara = false;
  bool telop = false;
  bool story_end = false;

  castle.play();

  while (1) {
    if (!app_env.isOpen()) return;
    if (story_end) break;

    if (!stop) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        click_checker += 1;
        chara = false;
        telop = false;
      }

      if (app_env.isPushKey('S')) {
        app_env.flushInput();
        break;
      }
    }

    app_env.setupDraw();
    drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      storyBG, Color(1, 1, 1));

    switch (click_checker) {
      case 0:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 80, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 36, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 1:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 81, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 37, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 2:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 81, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 38, telop_width, telop_height * 2,
          s_telop, Color(0, 0, 0));
        break;

      case 3:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 80, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 40, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      default:
        story_end = true;
        break;
    }

    app_env.update();
  }
  castle.stop();
}

// ４章を条件達成でクリア
void _4thstage_win(AppEnv& app_env) {
  Texture s_telop("res/png/T06_4thStage_Event.png");
  Texture storyBG("res/png/BG1_Castle.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara6("res/png/06_Rival_C.png");

  Media castle("res/wav/Event_Mainthema.wav");
  castle.looping(true);
  castle.gain(0.1);

  int click_checker = 0;
  float animetime = 0;
  float teloptime = 0;
  bool stop = false;
  bool chara = false;
  bool telop = false;
  bool story_end = false;

  castle.play();

  while (1) {
    if (!app_env.isOpen()) return;
    if (story_end) break;

    if (!stop) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        click_checker += 1;
        chara = false;
        telop = false;
      }

      if (app_env.isPushKey('S')) {
        app_env.flushInput();
        break;
      }
    }

    app_env.setupDraw();
    drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      storyBG, Color(1, 1, 1));

    switch (click_checker) {
      case 0:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 0, telop_width, telop_height * 2,
          s_telop, Color(0, 0, 0));
        break;

      case 1:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 19, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 2, telop_width, telop_height * 2,
          s_telop, Color(0, 0, 0));
        break;

      case 2:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 20, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 4, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 3:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 19, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 5, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 4:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 20, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 6, telop_width, telop_height * 2,
          s_telop, Color(0, 0, 0));
        break;

      case 5:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 19, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 8, telop_width, telop_height * 2,
          s_telop, Color(0, 0, 0));
        break;

      case 6:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 20, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 10, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 7:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 19, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 11, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 8:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 20, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 12, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 9:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 19, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 13, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 10:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 20, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 14, telop_width, telop_height * 2,
          s_telop, Color(0, 0, 0));
        break;

      case 11:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 19, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 16, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 12:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 19, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 17, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      default:
        story_end = true;
        break;
    }

    app_env.update();
  }
  castle.stop();
}

// エクストラステージをクリア
void Exstage_win(AppEnv& app_env) {
  Texture s_telop("res/png/T07_ExStage_Story.png");
  Texture n_telop("res/png/T02_Prologue_Story.png");
  Texture storyBG("res/png/BG4_Shrine.png");
  Texture chara1("res/png/01_Hero.png");
  Texture chara2("res/png/02_Daijin.png");
  Texture chara4("res/png/04_Rival_A.png");
  Texture chara7("res/png/07_Unknown.png");

  int click_checker = 0;
  float animetime = 0;
  float teloptime = 0;
  bool stop = false;
  bool chara = false;
  bool telop = false;
  bool story_end = false;

  while (1) {
    if (!app_env.isOpen()) return;
    if (story_end) break;

    if (!stop) {
      if (app_env.isPushButton(Mouse::LEFT)) {
        app_env.flushInput();
        click_checker += 1;
        chara = false;
        telop = false;
      }

      if (app_env.isPushKey('S')) {
        app_env.flushInput();
        break;
      }
    }

    app_env.setupDraw();
    drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      storyBG, Color(1, 1, 1));

    switch (click_checker) {
      case 0:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 50, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 35, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 1:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 77, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 36, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 2:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 79, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 37, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 3:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 79, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 2,
          0, telop_height * 38, telop_width, telop_height * 2,
          s_telop, Color(0, 0, 0));
        break;

      case 4:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 77, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 40, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 5:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 77, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 41, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 6:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 42, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 7:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 77, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 43, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 8:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 50, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 44, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 9:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 79, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 45, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 10:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 77, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 46, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 11:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 50, telop_width, telop_height,
          s_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 47, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 12:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 48, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      case 13:
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 77, telop_width, telop_height,
          n_telop, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 1,
          0, telop_height * 49, telop_width, telop_height * 1,
          s_telop, Color(0, 0, 0));
        break;

      default:
        story_end = true;
        break;
    }

    app_env.update();
  }

  float fade;
  for (fade = 1.0; fade > 0; fade -= 0.2) {
    if (!app_env.isOpen()) return;

    if (app_env.isPushButton(Mouse::LEFT)) {
      fade = 0;
      app_env.flushInput();
    }

    app_env.setupDraw();
    drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
      0, 0, Window::WIDTH, Window::HEIGHT,
      storyBG, Color(fade, fade, fade));
    app_env.update();
  };
}
