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

// �v���C���[�Ƒ���̍\���v�f
struct status {
	int money;
	int fund;
	short territory;
	int soldier;
	int mercenary;
	short fame;
	short infamy;
};

// �v���C���[���
// �������A���L���Y�A�̒n�A���m�A�`�E���A�����A����
status player = {500, 500, 0, 10, 0, 0, 0};

// �l�] �� ���� �[ ����
short player_popularity = player.fame - player.infamy;

// ������
// ���e�͂̊J�n���Ƀf�[�^�̏�����
// �������A���L���Y�A�̒n�A���m�A�`�E���A�����A����
status enemy;
short enemy_popularity;

// ���Y�̉��i���
const unsigned short fundinfo[14] = {
  1500, 500, 1000,
  1000, 1500, 3000,
  30000, 10000, 5000, 50000,
  20000, 10000, 10000, 65535
};

// ���p�ł��鎑�Y�̉��i���
const unsigned short iteminfo[7] = {1000, 1500, 3000, 10000, 5000, 10000, 20000};

// �A�C�e���̏��L�t���O
short player_fund[14] = {0};
short player_item[7] = {1, 0};

// �퓬�p�f�[�^�̏���
// pow = ����,  Attack = �U����
int player_pow, player_Attack, enemy_pow, enemy_Attack;

// �퓬�J�n���̕��̓f�[�^�ۑ��p
int player_pow0, enemy_pow0;

// �퓬�^�[���O��̕��͌v�Z�p
int player_start, player_end, player_damage;
int enemy_start, enemy_end, enemy_damage;

// ���O�̃v���C���[�̍s��
short player_lastcmd;

// ���̓Q�[�W�\���p
float player_powGauge, enemy_powGauge;

// ���̓Q�[�W�̐F
Color player_gaugecolor, enemy_gaugecolor;

// �L�����N�^�[�摜�̐؂�o���T�C�Y
const float chara_width = 20.0;
const float chara_height = 32.0;

// �e���b�v�摜�̐؂�o���T�C�Y
const float telop_width = 472.0;
const float telop_height = 20.0;
const float number_width = 14.0;
const float status_width = 56.0;

// �{�^���摜�̐؂�o���T�C�Y�i�Q�[�����j
const float button_width = 112.0;
const float button_height = 32.0;
const float buybutton_width = 240.0;

// �퓬�p�t���O
bool add_attack = false;

// �X�g�[���[�p�t���O

float month = 0.0;       // �Q�͊J�n����̃Q�[�����o�ߎ��� : �R�U�ȏ�ŃG�N�X�g���X�e�[�W�˓��s�\
bool ex_stage = false;    // �G�N�X�g���X�e�[�W�˓��t���O
bool spy = false;        // �X�p�C��������
short spy_time = 0;      // �X�p�C�����J�n����̃Q�[�����s���� : �P�O�^�[���o�߂Ń��Z�b�g
bool sabotage = false;   // �W�Q�H����g�p������
short sabo_time = 0;     // �W�Q�H��̎c��J�E���g : �R��W�Q�ŏI���i�X�p�C�������F�U��j
bool rumor = false;      // �����̗��z���g�p������ : �X�e�[�W�N���A�Ńt���O���Z�b�g
bool disclos = false;    // ���������o������
bool rebel = false;      // �����̐��۔���
bool cmd_flag = false;   // �R�}���h�t���O


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

	// �^�C�g��
	{
		Texture titleBG("res/png/Sp0_TitleBack.png");
		Texture titleLg("res/png/Sp1_TitleLogo.png");
		Texture select("res/png/Sp2_Select.png");

    Media opening("res/wav/Event_Opening.wav");
    opening.looping(true);
    opening.gain(0.1);
    opening.play();

		// �{�^���摜�̐؂�o���T�C�Y�i�^�C�g���j
		float mode_width = 208.0;
		float mode_height = 56.0;

		float x, y;
		float time = -256.0;

		while (1) {
			if (!app_env.isOpen()) return 0;

			app_env.setupDraw();

			mouse_pos = app_env.mousePosition();

			// �^�C�g����ʂ̕\��
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
							// �f�[�^�����[�h���܂��B��낵���ł����H
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
												// �ǂݍ��߂�f�[�^������܂���ł����B
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

						// �e���b�v�\��
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

	// ���C���w�i�ƃ��b�Z�[�W�̓ǂݍ���
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

	// �L�����N�^�[�摜�ǂݍ���
	Texture chara1("res/png/01_Hero.png");     // ���������艤�q
	Texture chara2("res/png/02_Daijin.png");   // ����
	Texture chara3("res/png/03_Heroine.png");  // �Ђ�
	Texture chara4("res/png/04_Rival_A.png");  // �C�P����
	Texture chara5("res/png/05_Rival_B.png");  // �f�u
	Texture chara6("res/png/06_Rival_C.png");  // ���m
  Texture chara7("res/png/07_Unknown.png");  // �{�X

  Media castle("res/wav/Event_Mainthema.wav");
  castle.looping(true);
  castle.gain(0.1);
  Media se_win("res/wav/se_win.wav");
  se_win.gain(0.1);
  Media se_lose("res/wav/se_lose.wav");
  se_lose.gain(0.1);

	// �{��
	while (1) {

		// �̓C�x���g
		switch (flag) {

			// �v�����[�O
			case 0:
			{
				// �e���b�v�摜�ǂݍ���
				Texture s_telop("res/png/T02_Prologue_Story.png");
				Texture e_telop("res/png/T02_Prologue_Event.png");

				// �����񏉊���
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

			// �P��
			case 1:
      {
        Texture s_telop("res/png/T02_Prologue_Story.png");
        Texture e_telop("res/png/T03_1stStage_Event.png");

        // �����񏉊���
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

			// �Q��
			case 2:
      {
        Texture s_telop("res/png/T04_2ndStage_Story.png");

        // �����񏉊���
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

			// �R��
			case 3:
      {
        Texture s_telop("res/png/T05_3rdStage_Story.png");

        // �����񏉊���
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

			// �S��
			case 4:
      {
        Texture s_telop("res/png/T06_4thStage_Story.png");

        // �����񏉊���
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

			// �G�N�X�g��
			case 5:
      {
        // �G�N�X�g���X�e�[�W�˓��ł��邩
        // �ł��Ȃ���΃X�L�b�v

        // �����񏉊���
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

			// �R���e�B�j���[���̃f�[�^�ǂݍ���
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

		// ���C���Q�[��
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

      // ���������𖞂����Ă��邩
      {
        // ���q�̏�𔃎�����
        if (flag == 1 && player_fund[0]) {
          mainTM.stop();
          break;
        }
        // �Q�͂ŏo���������������ׂĔ�������
        else if (flag == 2 && player_fund[3] && player_fund[4] && player_fund[5]) {
          mainTM.stop();
          break;
        }
        // �R�͂ŏo���������������ׂĔ�������
        else if (flag == 3 && player_fund[6] && player_fund[7] && player_fund[8] && player_fund[9]) {
          mainTM.stop();
          break;
        }
        // �����j�m�[�g�𔄋p����
        else if (flag == 4 && player_item[6] == 2) {
          mainTM.stop();
          _4thstage_win(app_env);
          break;
        }
        // ����̗̒n���O
        else if (enemy.territory <= 0) {
          mainTM.stop();
          if (flag == 5) {
            Exstage_win(app_env);
          }
          break;
        }
        // ��������
        else if (rebel) {
          mainTM.stop();
          if (flag == 3) {
            _3rdstage_win(app_env);
          }
          break;
        }
      }

      // �X�e�[�^�X������𒴂��ĂȂ����m�F
      {
        // �v���C���[
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

        // ����
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

      // �v���C���[���̕\��
			drawTextureBox(0, (Window::HEIGHT / 2) - 360, 256, 360,
				0, 0, 256, 360,
        s_user, Color(1, 1, 1));
      // ���Ȃ��̏��
      drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 2), status_width * 2, telop_height,
        0, telop_height * 95, status_width * 2, telop_height,
        system, Color(0, 0, 0));

      // ��������
      drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 14), (Window::WIDTH / 2), telop_height,
        0, telop_height * 0, (Window::WIDTH / 2), telop_height,
        target, Color(0, 0, 0));
      // �̒n����
      drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 15), (Window::WIDTH / 2), telop_height,
        0, telop_height * 5, (Window::WIDTH / 2), telop_height,
        target, Color(0, 0, 0));
      if (flag >= 1 && flag <= 4) {
        drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 16), (Window::WIDTH / 2), telop_height,
          0, telop_height * flag, (Window::WIDTH / 2), telop_height,
          target, Color(0, 0, 0));
      }

      // ����
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 3), status_width, telop_height,
				0, telop_height * 87, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
        short tex_x, tex_y;       // �摜�؂�o���p
        int val = player.money;

        // �X�e�[�^�X�����擾
        int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 6; i += 1) {
          // i �����O�łȂ�
          if (money[i] != 0) {
            tex_x = number_width * money[i];
            tex_y = telop_height * 82;
          }

          // i �����O�������Ƃ�
          else {
            // �z��̍ŏ�
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // ����ȊO
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
              if (i == 1 && money[i - 1] == 0) {}
              else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
              else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
              else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
              // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

      // ���m
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 4), status_width, telop_height,
				0, telop_height * 88, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
        short tex_x, tex_y;       // �摜�؂�o���p
        int val = player.soldier;

        // �X�e�[�^�X�����擾
        int soldier[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 6; i += 1) {
          // i �����O�łȂ�
          if (soldier[i] != 0) {
            tex_x = number_width * soldier[i];
            tex_y = telop_height * 82;
          }

          // i �����O�������Ƃ�
          else {
            // �z��̍ŏ�
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // ����ȊO
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
              if (i == 1 && soldier[i - 1] == 0) {}
              else if (i == 2 && (soldier[i - 2] || soldier[i - 1]) == 0) {}
              else if (i == 3 && (soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
              else if (i == 4 && (soldier[i - 4] || soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
              // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

      // �`�E���i�`���j
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 5), status_width, telop_height,
				0, telop_height * 89, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
        short tex_x, tex_y;       // �摜�؂�o���p
        int val = player.mercenary;

        // �X�e�[�^�X�����擾
        int mercenary[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 6; i += 1) {
          // i �����O�łȂ�
          if (mercenary[i] != 0) {
            tex_x = number_width * mercenary[i];
            tex_y = telop_height * 82;
          }

          // i �����O�������Ƃ�
          else {
            // �z��̍ŏ�
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // ����ȊO
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
              if (i == 1 && mercenary[i - 1] == 0) {}
              else if (i == 2 && (mercenary[i - 2] || mercenary[i - 1]) == 0) {}
              else if (i == 3 && (mercenary[i - 3] || mercenary[i - 2] || mercenary[i - 1]) == 0) {}
              else if (i == 4 && (mercenary[i - 4] || mercenary[i - 3] || mercenary[i - 2] || mercenary[i - 1]) == 0) {}
              // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

      // ����
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 6), status_width, telop_height,
				0, telop_height * 90, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
        short tex_x, tex_y;       // �摜�؂�o���p
        short val = player.fame;

        // �X�e�[�^�X�����擾
        short fame[3] = {val / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 3; i += 1) {
          // i �����O�łȂ�
          if (fame[i] != 0) {
            tex_x = number_width * fame[i];
            tex_y = telop_height * 82;
          }

          // i �����O�������Ƃ�
          else {
            // �z��̍ŏ�
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // ����ȊO
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
              if (i == 1 && fame[i - 1] == 0) {}
              // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

      // ����
			drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 7), status_width, telop_height,
				0, telop_height * 91, status_width, telop_height,
        system, Color(0, 0, 0));
      {
        short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
        short tex_x, tex_y;       // �摜�؂�o���p
        short val = player.infamy;

        // �X�e�[�^�X�����擾
        short infamy[3] = {val / 100, (val % 100) / 10, val % 10};

        for (i = 0; i < 3; i += 1) {
          // i �����O�łȂ�
          if (infamy[i] != 0) {
            tex_x = number_width * infamy[i];
            tex_y = telop_height * 82;
          }

          // i �����O�������Ƃ�
          else {
            // �z��̍ŏ�
            if (i == 0) {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;
            }

            // ����ȊO
            else {
              skip += 1;
              tex_x = 0;
              tex_y = telop_height * 83;

              // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
              if (i == 1 && infamy[i - 1] == 0) {}
              // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

      // ����̏��\���i�X�p�C�������̂݁j
      if (spy) {
        drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 9), status_width * 2, telop_height,
          0, telop_height * 96, status_width * 2, telop_height,
          system, Color(0, 0, 0));
        // ����
				drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 10), status_width, telop_height,
					0, telop_height * 87, status_width, telop_height,
          system, Color(0, 0, 0));
        {
          short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
          short tex_x, tex_y;       // �摜�؂�o���p
          int val = enemy.money;

          // �X�e�[�^�X�����擾
          int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i �����O�łȂ�
            if (money[i] != 0) {
              tex_x = number_width * money[i];
              tex_y = telop_height * 82;
            }

            // i �����O�������Ƃ�
            else {
              // �z��̍ŏ�
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // ����ȊO
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
                if (i == 1 && money[i - 1] == 0) {}
                else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
                else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

        // ���m
				drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 11), status_width, telop_height,
					0, telop_height * 88, status_width, telop_height,
          system, Color(0, 0, 0));
        {
          short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
          short tex_x, tex_y;       // �摜�؂�o���p
          int val = enemy.soldier;

          // �X�e�[�^�X�����擾
          int soldier[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i �����O�łȂ�
            if (soldier[i] != 0) {
              tex_x = number_width * soldier[i];
              tex_y = telop_height * 82;
            }

            // i �����O�������Ƃ�
            else {
              // �z��̍ŏ�
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // ����ȊO
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
                if (i == 1 && soldier[i - 1] == 0) {}
                else if (i == 2 && (soldier[i - 2] || soldier[i - 1]) == 0) {}
                else if (i == 3 && (soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
                else if (i == 4 && (soldier[i - 4] || soldier[i - 3] || soldier[i - 2] || soldier[i - 1]) == 0) {}
                // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

        // �̒n
        drawTextureBox(10, (Window::HEIGHT / 2) - ((2 + telop_height) * 12), status_width, telop_height,
          0, telop_height * 92, status_width, telop_height,
          system, Color(0, 0, 0));
        {
          short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
          short tex_x, tex_y;       // �摜�؂�o���p
          short val = enemy.territory;

          // �X�e�[�^�X�����擾
          short territory[3] = {val / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 3; i += 1) {
            // i �����O�łȂ�
            if (territory[i] != 0) {
              tex_x = number_width * territory[i];
              tex_y = telop_height * 82;
            }

            // i �����O�������Ƃ�
            else {
              // �z��̍ŏ�
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // ����ȊO
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
                if (i == 1 && territory[i - 1] == 0) {}
                // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

			// �����J��
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 0, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// ���Y�Ɋւ���R�}���h�ł��B
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

              // �{�^���������O�̏���
              if (checker == 0) {
                // ��߂�
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

                // �A���o�C�g
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 6, button_width, button_height,
                    button, angle);

                  if (on_mouse) {

                    // ���m�����ɃA���o�C�g�����Ď����𓾂܂��B
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 14, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // ����������������܂��B
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                      0, telop_height * 15, telop_width, telop_height,
                      system, Color(0, 0, 0));

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 1;
                    }
                  }
                }

                // ����
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 7, button_width, button_height,
                    button, angle);

                  if (on_mouse) {

                    // �������s���A�Ŏ��𓾂܂��B
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 16, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // ���L���Y�̉��l�������قǌ��ʂ��オ��܂��B
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                      0, telop_height * 17, telop_width, telop_height,
                      system, Color(0, 0, 0));

                    if (app_env.isPushButton(Mouse::LEFT)) {
                      app_env.flushInput();
                      checker = 2;
                    }
                  }
                }

                // ���p
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 8, button_width, button_height,
                    button, angle);

                  if (on_mouse) {

                    // ���Y�𔄋p���Ď����𓾂܂��B
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 18, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // ���p�������Y�͂Ȃ��Ȃ�܂��B
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

              // �{�^������������̏����F���p�ȊO
              else {
                if (checker == 1) {
                  // �E�A���o�C�g����
                  drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                    0, telop_height * 43, telop_width, telop_height,
                    system, Color(0, 0, 0));

                  {
                    short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
                    short tex_x, tex_y;       // �摜�؂�o���p
                    int val = player.soldier * 2;

                    if (val >= (999999 - player.money)) {
                      val = (999999 - player.money);
                    }

                    int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

                    for (i = 0; i < 6; i += 1) {
                      // i �����O�łȂ�
                      if (money[i] != 0) {
                        tex_x = number_width * money[i];
                        tex_y = telop_height * 82;
                      }

                      // i �����O�������Ƃ�
                      else {
                        // �z��̍ŏ�
                        if (i == 0) {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;
                        }

                        // ����ȊO
                        else {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;

                          // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
                          if (i == 1 && money[i - 1] == 0) {}
                          else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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
                  // �E�Ŏ�
                  drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                    0, telop_height * 44, telop_width, telop_height,
                    system, Color(0, 0, 0));

                  {
                    short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
                    short tex_x, tex_y;       // �摜�؂�o���p
                    int val = (player.fund / 2) + (player.territory * 100);

                    if (val > (999999 - player.money)) {
                      val = (999999 - player.money);
                    }

                    int money[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

                    for (i = 0; i < 6; i += 1) {
                      // i �����O�łȂ�
                      if (money[i] != 0) {
                        tex_x = number_width * money[i];
                        tex_y = telop_height * 82;
                      }

                      // i �����O�������Ƃ�
                      else {
                        // �z��̍ŏ�
                        if (i == 0) {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;
                        }

                        // ����ȊO
                        else {
                          skip += 1;
                          tex_x = 0;
                          tex_y = telop_height * 83;

                          // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
                          if (i == 1 && money[i - 1] == 0) {}
                          else if (i == 2 && (money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 3 && (money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          else if (i == 4 && (money[i - 4] || money[i - 3] || money[i - 2] || money[i - 1]) == 0) {}
                          // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

			// ���͑���
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 1, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// ���͂Ɋւ���R�}���h�ł��B
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

              // ��߂�
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

							// ���m��W
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 9, button_width, button_height,
									button, angle);

								if (on_mouse) {

									// ���K�̕��m���W���܂��B
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

							// �`�E��
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 10, button_width, button_height,
									button, angle);

								if (on_mouse) {

									// �`�E�����W�߂܂��B
									drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
										0, telop_height * 22, telop_width, telop_height,
										system, Color(0, 0, 0));
									// �퓬�I���A�܂��͎��̌��ɂȂ�Ƃ��Ȃ��Ȃ�܂��B
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

							// ��������
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
								angle = setColor(on_mouse);
								drawTextureBox(x0, y0, button_width, button_height,
									0, button_height * 11, button_width, button_height,
									button, angle);

								if (on_mouse) {

									// �����I�ɕ��m���W�߂܂��B
									drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
										0, telop_height * 24, telop_width, telop_height,
										system, Color(0, 0, 0));
									// �l�]��������܂��B
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

			// �v��
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 2, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// ���Ɋւ���R�}���h�ł��B
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

                // ��߂�
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

                // �X�p�C
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
                    // �E���������F100G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 58, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (spy) {
                      // ���Ɏ��s���Ă��܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 98, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }
                    else if (player.money >= 100) {
                      // ����ɃX�p�C�𑗂荞�݁A����������܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 27, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // ����̏󋵂������̊ԁA�킩��悤�ɂȂ�܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 28, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ����������܂���B
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

                // �ڂ��Q�H��
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
                    // �E���������F50G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 61, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (sabotage) {
                      // ���Ɏ��s���Ă��܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 98, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }
                    else if (player.money >= 50) {
                      // ���m�⎑�������΂炭���₹�Ȃ��悤�ɂ��܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 29, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ����������܂���B
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

                // �����̗��z
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
                    // �E���������F100G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 58, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (rumor) {
                      // ���Ɏ��s���Ă��܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 98, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      on_mouse = false;
                    }
                    else if (player.money >= 100) {
                      // ����̎��Y�̉��l�������܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 30, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ����������܂���B
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

                // �ǂ��𐷂�
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
                    // �E���������F200G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 66, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (player.money >= 200) {
                      // ������ �ǂ��𐷂��āA���m��킦�Ȃ������܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 31, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // ���o����ƈ������傫���オ��܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 32, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ����������܂���B
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

                // ����
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
                    // �E���������F100G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 58, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (player.money >= 100) {
                      // ���`������āA�x���𓾂܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 33, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // �������オ��܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 34, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ����������܂���B
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

                // �S�V�b�v
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
                    // �E���������F200G
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
                      0, telop_height * 66, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    if (player.money >= 200) {
                      // ����̈����E���T�𗬂��āA�l�]�������܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 35, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // ���s����Ǝ����̐l�]��������܂��B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 36, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ����������܂���B
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

                // ����
                {
                  x0 = x + 2 + button_width;
                  y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 9);

                  on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                  angle = setColor(on_mouse);
                  drawTextureBox(x0, y0, button_width, button_height,
                    0, button_height * 18, button_width, button_height,
                    button, angle);

                  if (on_mouse) {
                    // ����̍����Ŕ������������܂��B
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 37, telop_width, telop_height,
                      system, Color(0, 0, 0));
                    // ��������ƁA���O�Ǖ����܂��B
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

              // �{�^������������̏���
              else {
                // �u�͂��v�������O
                if (p_flag == 0) {
                  if (checker == 1) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 12, button_width, button_height,
                      button, angle_b);
                    // �X�p�C�𑗂�܂��B��낵���ł����H
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 57, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 2) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 13, button_width, button_height,
                      button, angle_b);
                    // �W�Q�H����s���܂��B��낵���ł����H
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 60, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 3) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 14, button_width, button_height,
                      button, angle_b);
                    // �f�}���𗬂��܂��B��낵���ł����H
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 63, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 4) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 15, button_width, button_height,
                      button, angle_b);
                    // �H���ł𔭐������܂��B��낵���ł����H
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 65, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 5) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 16, button_width, button_height,
                      button, angle_b);
                    // �������n�߂܂��B��낵���ł����H
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 69, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 6) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 17, button_width, button_height,
                      button, angle_b);
                    // �S�V�b�v�L���𗬂��܂��B��낵���ł����H
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 70, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  else if (checker == 7) {
                    drawTextureBox(x, y + ((2 + button_height) * 2), button_width, button_height,
                      0, button_height * 18, button_width, button_height,
                      button, angle_b);
                    // ���N���܂��B��낵���ł����H
                    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                      0, telop_height * 71, telop_width, telop_height,
                      system, Color(0, 0, 0));
                  }

                  // �͂�
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

                  // ������
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

                // �u�͂��v����������
                else {
                  // �X�p�C�̎��s����
                  if (checker == 1) {
                    // �X�p�C�𑗂�܂����B
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

                  // �W�Q�H��̎��s����
                  else if (checker == 2) {
                    // �W�Q���n�߂܂��B
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

                  // �����̗��z�̎��s����
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
                      // �������܂����B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 55, telop_width, telop_height,
                        system, Color(0, 0, 0));
                      // ���Y���l��10%������܂����B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 64, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ���s�B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // �ł𐷂�̎��s����
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
                      // ����̕��m������10%�퓬�s�\�ɂ��܂����B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 67, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ���s�B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }

                    if (disclos) {
                      // ���o���Ă��܂���
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                        0, telop_height * 68, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // �����̎��s����
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
                      // �������܂����B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 55, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ���s�B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // �S�V�b�v�̎��s����
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
                      // �������܂����B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 55, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                    else {
                      // ���s�B
                      drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                        0, telop_height * 56, telop_width, telop_height,
                        system, Color(0, 0, 0));
                    }
                  }

                  // �����̎��s����
                  else if (checker == 7) {
                    if (p_flag == 1) {
                      // �R�͂������Ƃ�
                      if (flag == 3) {
                        if (player_item[3] == 0) {
                          rebel = false;
                        }
                        else {
                          rebel = true;
                        }
                      }

                      // �R�͈ȊO
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
                        // �����听��
                        // �������������č��O�Ǖ������I
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 72, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                      else {
                        Texture telop3("res/png/T05_3rdStage_Story.png");
                        // �������s
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 3,
                          0, telop_height * 32, telop_width, telop_height * 3,
                          telop3, Color(0, 0, 0));
                      }
                    }
                    else {
                      if (rebel) {
                        // �����听��
                        // �������������č��O�Ǖ������I
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 72, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                      else if (cmd_flag) {
                        // ��������
                        // �����͋N�������A���O�Ǖ��ł��Ȃ������E�E�E
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 73, telop_width, telop_height,
                          system, Color(0, 0, 0));
                        // �������A�����̉e���ő�Ō���^�����B
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
                          0, telop_height * 74, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                      else {
                        // �������s
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

			// ����
			{
        x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 3, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// ����̎��Y�𔃎��܂��B
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

							// �ǂ�𔃎��܂����H
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 77, telop_width, telop_height,
								system, Color(0, 0, 0));

							// ��߂�
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
								// �̒n
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
                          // ����ȏ㔃�����܂���B
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 94, telop_width, telop_height,
                            system, Color(0, 0, 0));
                        }
                        else {
                          // ����������܂���B
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 5, telop_width, telop_height,
                            system, Color(0, 0, 0));
                        }
                      }
                      else {
                        // ����������܂���B
                        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                          0, telop_height * 5, telop_width, telop_height,
                          system, Color(0, 0, 0));
                      }
                    }
                    else {
                      // ����̗̒n�𔃂����܂��B
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

							// �P�͂���o��
							if (flag >= 1 && flag < 6) {
								// �P�͂̂�
								if (flag == 1) {
									// ��
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
													// ����������܂���B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[0]) {
													// ���Ď����������Z��ł�����
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 0, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// �I�o�P���ł����Ȃ��炢�{���{���E�E�E
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
												// ���łɏ��L���Ă��܂��B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 1 end
								} // flag 1 only

								// �R�͂܂�
								if (flag < 4) {
									// �_�n
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
													// ����������܂���B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[1]) {
													// �_�n�B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 2, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// �q�ꂪ�߂��̂ŁA�ƒ{�p�̃G�T�𐶎Y���Ă���B
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
												// ���łɏ��L���Ă��܂��B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 2 end

									// �q��
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
													// ����������܂���B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[2]) {
													// �q��B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 4, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// ��ɋ��A�u�^����ĂĂ��邪�A�K�͂��������B
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
												// ���łɏ��L���Ă��܂��B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 3 end
								} // flag 1-3
							} // flag 1

							// �Q�͂���o��
							if (flag >= 2 && flag < 6) {
								// �R�͂܂�
								if (flag < 4) {
									// �_�n +1
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
													// ����������܂���B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[3]) {
													// ������Ɨ��h�Ȕ_�n�B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 6, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// ���C���p�̉ʕ��𐶎Y���Ă���B
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
												// ���łɏ��L���Ă��܂��B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 4 end

									// �q�� +1
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
													// ����������܂���B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 5, telop_width, telop_height,
														system, Color(0, 0, 0));
												}

												else if (player.money >= fundinfo[4]) {
													// �傫�Ȗq��B
													drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
														0, telop_height * 8, telop_width, telop_height,
														buytxt, Color(0, 0, 0));
													// ���m���������n����ĂĂ���B
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
												// ���łɏ��L���Ă��܂��B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 84, telop_width, telop_height,
													system, Color(0, 0, 0));
											}
										}
									} // button 5 end
								} // flag 2-3

								// �H��
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[5]) {
												// ���s��̕��𐶎Y���Ă���H��B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 10, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// �E�l�̋Z������A���Y�i�B
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 1-2 end
							} // flag 2

							if (flag >= 3 && flag < 6) {
								// ���̍z�R
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[6]) {
												// �����̂��z�R�B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 12, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// �ʂ͑������A�i���͍����Ȃ��悤���B
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 6 end

								// ��̍z�R
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[7]) {
												// �₪�̂��z�R�B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 14, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// �ʂ͑�����
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 7 end

								// �S�̍z�R
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[8]) {
												// �S���̂��z�R�B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 16, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// �ʂ͑�����
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 8 end

								// ���p��
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[9]) {
												// �S�E�J�Ȕ��p�فB
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 18, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// �����Ă�̂́A���̓j�Z���m���炯�̃K���N�^�B
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 10 end
							} // flag 3

							// �S�͂���o��
							if (flag >= 4 && flag < 6) {
								// �ǎ��ȓS�z�R
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[10]) {
												// �ǎ��ȓS���̂��z�R�B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 20, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// ���������A�ǂ���������ɂ͂������Ȃ��B
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 2-2 end

								// ����̒b�艮
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[11]) {
												// �L���Ȓb�艮
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 22, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// �����ō���镐��͖��i�B
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 3-2 end

								// �h��̒b�艮
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[12]) {
												// �L���Ȓb�艮
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 22, telop_width, telop_height,
													buytxt, Color(0, 0, 0));
												// �����ō���镐��͖��i�B
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 4-2 end

								// �X
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
												// ����������܂���B
												drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
													0, telop_height * 5, telop_width, telop_height,
													system, Color(0, 0, 0));
											}

											else if (player.money >= fundinfo[13]) {
												// �ނ�
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
											// ���łɏ��L���Ă��܂��B
											drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
												0, telop_height * 84, telop_width, telop_height,
												system, Color(0, 0, 0));
										}
									}
								} // button 5-2 end
							} // flag 4

							if (flag <= 0 && flag >= 6) {
								// �����镨��������܂���ł����B
								drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
									0, telop_height * 80, telop_width, telop_height,
									system, Color(0, 0, 0));
							}

							app_env.update();
						}
					}
				}
			}

			// �N�U
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 4, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// ����ɐ푈���������܂��B
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

							// �푈���n�߂܂��B�{���ɂ�낵���ł����H
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
								0, telop_height * 8, telop_width, telop_height,
								system, Color(0, 0, 0));

							// �͂�
							{
								x0 = x + 2 + button_width;
								y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

								on_mouse = on_button(mouse_pos, x0, y0, button_width, button_height);
                if ((player.soldier + player.mercenary) == 0) {
                  angle = Color(0.5, 0.5, 0.5);
                  on_mouse = false;
                  // �푈�ł��܂���B
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

										// �a�f�l����
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
                      // �퓬�J�n���܂��B
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

                      // �ǂ��炩�̕��͂��O�ɂȂ���
                      if (player_pow <= 0 || enemy_pow <= 0) {
                        break;
                      }

                      // �c��s���񐔂��O�łȂ�
                      if (count != 0) {
                        drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                          0, 0, Window::WIDTH, Window::HEIGHT,
                          mainBG, Color(1, 1, 1));
                      }

                      // �c��s���񐔂��O�ɂȂ����Ƃ�
                      else if (count == 0) {
                        while (1) {
                          if (!app_env.isOpen()) return 0;
                          app_env.setupDraw();

                          mouse_pos = app_env.mousePosition();

                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                            0, 0, Window::WIDTH, Window::HEIGHT,
                            mainBG, Color(1, 1, 1));

                          // ����͓P�ނ���悤���E�E�E
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 9, telop_width, telop_height,
                            battle, Color(0, 0, 0));

                          // �ǌ�
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

                          // �P��
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

                      // �ǌ���˓��Ȃ�w�i��[��ꕗ��
                      if (count == -1) {
                        drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                          0, 0, Window::WIDTH, Window::HEIGHT,
                          mainBG, Color(1, 0.6, 0.2));
                        drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT - 360,
                          0, 360, Window::WIDTH, Window::HEIGHT - 360,
                          mainBG, Color(1, 1, 1));
                      }

                      // �ǌ���I��
                      else if (count == -2) {
                        break;
                      }

                      // �ǌ����Ȃ��Ȃ烊�U���g��ʂɐi��
                      else if (count == -4) {
                        break;
                      }

                      // �L�����N�^�[�\���i�v���C���[���j
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

                      // �L�����N�^�[�\���i���葤�j
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

											// �v���C���[�̕��̓Q�[�W�\��
											drawBox((-Window::WIDTH / 2) + 18, (Window::HEIGHT / 2) - 32,
												204, 24,
												3, Color(1, 0.8, 0.4));
											drawFillBox((-Window::WIDTH / 2) + 20, (Window::HEIGHT / 2) - 30,
												player_powGauge * 2, 20,
												player_gaugecolor);

											// ����̕��̓Q�[�W�\��
											drawBox((Window::WIDTH / 2) - 18, (Window::HEIGHT / 2) - 32,
												-204, 24,
												3, Color(1, 0.8, 0.4));
											drawFillBox((Window::WIDTH / 2) - (20 + (enemy_powGauge * 2)), (Window::HEIGHT / 2) - 30,
												enemy_powGauge * 2, 20,
												enemy_gaugecolor);

                      drawTextureBox(-Window::WIDTH / 2, (Window::HEIGHT / 2) - (40 + telop_height), Window::WIDTH, telop_height + 8,
                        0, telop_height * 5, Window::WIDTH, telop_height + 8,
                        battle, Color(0.5, 0.5, 0.5));

											// �ˌ�
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

											// ���
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

											// ���
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

											// �P��
                      // �ǌ���˓�������\�����Ȃ�
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
                              // �P�ނ��܂����B
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

											// �퓬����
											{
                        // �R�}���h���͂��������Ƃ�
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

                    // �퓬�I���̏����ƌ��ʉ�ʂ̕\��
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

                        // �ǌ�������Ă�����w�i��[��ꕗ��
                        if (count == -2) {
                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                            0, 0, Window::WIDTH, Window::HEIGHT,
                            mainBG, Color(1, 0.6, 0.2));
                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT - 360,
                            0, 360, Window::WIDTH, Window::HEIGHT - 360,
                            mainBG, Color(1, 1, 1));
                        }

                        // �����łȂ���Βʏ�̔w�i��\��
                        else {
                          drawTextureBox(-Window::WIDTH / 2, -Window::HEIGHT / 2, Window::WIDTH, Window::HEIGHT,
                            0, 0, Window::WIDTH, Window::HEIGHT,
                            mainBG, Color(1, 1, 1));
                        }

                        if (player_powGauge >= enemy_powGauge) {
                          // ����
                          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                            0, telop_height * 8, telop_width, telop_height,
                            battle, Color(0, 0, 0));

                          win = true;
                        }
                        else {
                          // �s�k
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

                            // �G��S�ł����Ă���΁A�{�[�i�X�ǉ�
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

                          // �G����������
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

                          // �A�C�e���̃h���b�v����
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

							// ������
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

			// �Z�[�u
			{
				x = (-Window::WIDTH / 2) + 2;
				y = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

				on_mouse = on_button(mouse_pos, x, y, button_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 5, button_width, button_height,
					button, angle);

				if (on_mouse) {

					// �����܂ł̃Q�[�����e��ۑ����܂��B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 10, telop_width, telop_height,
						system, Color(0, 0, 0));
					// ���ӁF���łɃf�[�^������ꍇ�͏㏑�����܂��B
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

							// �Z�[�u���܂��B�{���ɂ�낵���ł����H
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
								0, telop_height * 12, telop_width, telop_height,
								system, Color(0, 0, 0));

							// �͂�
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

							// ������
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

    // ���������B��
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

  // �G���f�B���O����
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

// �w�i�X���C�h�i�S��ʁj
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

// �w�i�X���C�h�i�w�i�̂݁j
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

// �w�i�t�F�[�h�A�E�g
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

// �w�i�t�F�[�h�C��
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

// �e�̓^�C�g����ʌĂяo��
void titleCall(AppEnv& app_env, short flag) {
	// �e�̓^�C�g���w�i
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

    // �͂̔ԍ�
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

    // �̓^�C�g��
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

// �e�̓N���A��ʌĂяo��
void clearCall(AppEnv& app_env, short flag) {
  // �e�̓^�C�g���w�i
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

    // �͂̔ԍ�
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

    // �̓^�C�g��
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

    // �N���A�\��
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

// �퓬�O�̏���
void duel_standby() {
	// �l�]�l�̌v�Z
	player_popularity = player.fame - player.infamy;
	enemy_popularity = enemy.fame - enemy.infamy;

	// �v���C���[�̕��͂ƍU���͂̌v�Z
	player_pow = player.soldier * 10 + player.mercenary * 5;
	player_pow0 = player_pow;

	// ����̕��͂ƍU���͂̌v�Z
	enemy_pow = enemy.soldier * 10 + enemy.mercenary * 5;
	enemy_pow0 = enemy_pow;
}

// �U���͏��
void Attack_limit() {
  if (player_Attack > 10000) {
    player_Attack = 10000;
  }
  if (enemy_Attack > 10000) {
    enemy_Attack = 10000;
  }
}

// ���̓Q�[�W�̌v�Z
void duel_setpowGauge() {
	player_powGauge = (player_pow * 100) / player_pow0;
	enemy_powGauge = (enemy_pow * 100) / enemy_pow0;
}

// �퓬�^�[���J�n���̏���
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

// �퓬���s
void duel_fight(short player_cmd, short enemy_cmd) {
	switch (enemy_cmd) {

		// ����F�ˌ�
		case 0:
			// �������ˌ�
			if (player_cmd == 0) {
				player_damage = player_Attack * 1.0;
				enemy_damage = enemy_Attack * 1.0;
			}
			// ���������
			else if (player_cmd == 1) {
				player_damage = player_Attack * 1.0;
				enemy_damage = 0;
			}
			// ���������
			else {
				player_damage = player_Attack * 0.5;
				enemy_damage = enemy_Attack * 1.5;
			}
			break;

			// ����F���
		case 1:
			// �������ˌ�
			if (player_cmd == 0) {
				player_damage = 0;
				enemy_damage = enemy_Attack * 1.0;
			}
			// ���������
			else if (player_cmd == 1) {
				player_damage = 0;
				enemy_damage = 0;
			}
			// ���������
			else {
				player_damage = player_Attack * 1.0;
				enemy_damage = 0;
			}
			break;

			// ����F���
		case 2:
			// �������ˌ�
			if (player_cmd == 0) {
				player_damage = player_Attack * 1.5;
				enemy_damage = enemy_Attack * 0.5;
			}
			// ���������
			else if (player_cmd == 1) {
				player_damage = 0;
				enemy_damage = enemy_Attack * 1.0;
			}
			// ���������
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

// �퓬�^�[���I����̏���
void duel_turnend() {
	int damage0;

	// �v���C���[���̕��m�����炷
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

	// ���葤�̕��m�����炷
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

// ����̍s���p�^�[��
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

// ���̓Q�[�W�̃J���[�ύX
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

// �ʏ�퓬�̂a�f�l�ݒ�
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

// �ǌ���̂a�f�l�ݒ�
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

// ���C���e�[�}�̂a�f�l�ݒ�
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

// ���͑���
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
			// ���l��W���܂����H
			drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
				0, telop_height * 50, telop_width, telop_height,
				system, Color(0, 0, 0));

			// ��߂�
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

			// ���m��W
			if (genre == 0) {
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 9, button_width, button_height,
					button, angle_b);

				// ���m�F�P�l�@10G
				drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
					0, telop_height * 51, telop_width, telop_height,
					system, Color(0, 0, 0));

				// 10�l
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
							// ����������܂���B
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 100�l
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
							// ����������܂���B
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 1000�l
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
							// ����������܂���B
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}
			} // ���m��W

			// �`�E��
			else if (genre == 1) {
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 10, button_width, button_height,
					button, angle_b);

				// �`�E���F�P�l�@5G
				drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
					0, telop_height * 52, telop_width, telop_height,
					system, Color(0, 0, 0));

				// 10�l
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
							// ����������܂���B
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 100�l
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
							// ����������܂���B
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}

				// 1000�l
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
							// ����������܂���B
							drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
								0, telop_height * 5, telop_width, telop_height,
								system, Color(0, 0, 0));
						}
					}
				}
			} // �`�E��

			// ��������
			else {
				drawTextureBox(x, y, button_width, button_height,
					0, button_height * 11, button_width, button_height,
					button, angle_b);

				// 10�l
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

				// 100�l
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

				// 1000�l
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
			} // ��������
		}

		else {
			// ��W���܂����B
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

// �������̍ŏI�m�F
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

		// �����܂��B��낵���ł����H
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

// ���p
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
			// �ǂ�𔄋p���܂����H
			drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
				0, telop_height * 45, telop_width, telop_height,
				system, Color(0, 0, 0));

			// ��߂�
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

			// �����׍H
			if (player_item[0] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 1);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 0, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// ���������L���C�ȑ����i�A���_�B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 26, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// ����o��Ƃ��Ɏ����Ă������́B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 27, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 0;
					}
				}
			}

			// �E�܂�������
			if (player_item[1] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 2);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 1, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// �j�炵�����ӂ�镐��B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 28, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// �ƂĂ����L���B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 29, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 1;
					}
				}
			}

			// �������C��
			if (player_item[2] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 3);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 2, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// �n���̉ʕ����g�����ʎ����B���Y�i�B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 30, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// �ƂĂ���i�Ȗ��킢�̃��C���B�炵���B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 31, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 2;
					}
				}
			}

			// ���h�Ȃ���ނ�
			if (player_item[3] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 4);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 3, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// ���������h�Ȃ���̂���ނ�B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 32, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// ���Ԃ�΁A�N�ł����l�̂悤�ɂȂꂻ���ȕ��͋C�B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 33, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 3;
					}
				}
			}

			// �S�e�S�e������
			if (player_item[4] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 5);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 4, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// ���肷���ăS�e�S�e���Ă镞�B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 34, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// ������̃Z���X���m���A�ڂ̓łȕ��B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 35, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 4;
					}
				}
			}

			// �p�Y�̕���
			if (player_item[5] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 6);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 5, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// �����ȍ��̗��h�ȕ���B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 36, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// ���Ă邾���ŁA�E�C���킭�悤�ȑ����B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 37, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 5;
					}
				}
			}

			// �����j�m�[�g
			if (player_item[6] == 1) {
				x0 = x + button_width + 2;
				y0 = (Window::HEIGHT / 2) - ((2 + button_height) * 7);

				on_mouse = on_button(mouse_pos, x0, y0, buybutton_width, button_height);
				angle = setColor(on_mouse);
				drawTextureBox(x0, y0, buybutton_width, button_height,
					0, button_height * 6, buybutton_width, button_height,
					s_menu, angle);

				if (on_mouse) {
					// �N�ɂ��m��ꂽ���Ȃ��A�閧�����ς��̃m�[�g�B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
						0, telop_height * 38, telop_width, telop_height,
						buytxt, Color(0, 0, 0));
					// ���g�����������Ă������A���ꂪ�w�ɂȂ����悤���B
					drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height,
						0, telop_height * 39, telop_width, telop_height,
						buytxt, Color(0, 0, 0));

					if (app_env.isPushButton(Mouse::LEFT)) {
						app_env.flushInput();
						checker = 6;
					}
				}
      }

      // �ߋ��ɏ��L���Ă������A���p�ς�
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
              // ���łɔ��p���Ă��܂��B
              drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
                0, telop_height * 47, telop_width, telop_height,
                system, Color(0, 0, 0));
            }
          }
        }
      }
		} // item end

		else {
			// ���p���܂��B��낵���ł����H
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

// �J�[�\�����{�^����ɂ��邩
bool on_button(Vec2f pos, float x, float y, float tex_width, float tex_height) {
	if (x < pos.x() && pos.x() < x + tex_width &&
		y < pos.y() && pos.y() < y + tex_height) {
		return true;
	}
	return false;
}

// �{�^���J���[�ύX
Color setColor(bool on_mouse) {
	if (on_mouse) {
		return Color(1, 0.8, 0.4);
	}
	return Color(1, 1, 1);
}

// �`���[�g���A���i�v�����[�O��p�A�{�ҁj
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

    // �w�i
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

        // ���͑�����I��ł��������B
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
        // ���������{�^���i�I��s�j
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 3, button_width, button_height,
          0, button_height * 11, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));

        // ���m��W
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

        // �`�E��
        on_mouse = on_button(pos, x + (2 + button_width) * 1, y - (2 + button_height) * 2, button_width, button_height);
        angle = setColor(on_mouse);
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 2, button_width, button_height,
          0, button_height * 10, button_width, button_height,
          button, angle);

        // ���m��W���`�E����I��ł��������B
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 24, telop_width, telop_height,
          eventP, Color(0, 0, 0));
        // ���`�E���Ɋւ��钍��
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

        // ��W�l����I��ł��������B
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 27, telop_width, telop_height,
          eventP, Color(0, 0, 0));
        // 1000�l�{�^���i�I��s�j
        drawTextureBox(x + (2 + button_width) * 1, y - (2 + button_height) * 3, button_width, button_height,
          0, button_height * 29, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));

        // 10�l
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

        // 100�l
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
        // �N�U��I��ł��������B
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
          // �q���g
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 2,
            0, telop_height * 36, telop_width, telop_height * 2,
            eventP, Color(0, 0, 0));
          // �q���g�Q
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height,
            0, telop_height * 39, telop_width, telop_height,
            eventP, Color(0, 0, 0));
        }
        else {
          // �퓬�J�n�B�U����i��I��ł��������B
          drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 4), telop_width, telop_height * 2,
            0, telop_height * 29, telop_width, telop_height * 2,
            eventP, Color(0, 0, 0));
        }

        // �L�����N�^�[�\���i�v���C���[���j
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

        // �L�����N�^�[�\���i���葤�j
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

        // �ˌ��i�I��s�j
        drawTextureBox(x, (Window::HEIGHT / 2) - 358 + (2 + button_height) * 4, button_width, button_height,
          0, button_height * 22, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));
        // ��݁i�I��s�j
        drawTextureBox(x, (Window::HEIGHT / 2) - 358 + (2 + button_height) * 2, button_width, button_height,
          0, button_height * 24, button_width, button_height,
          button, Color(0.5, 0.5, 0.5));

        // ���
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
        // �U���e���b�v�\��
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 15, telop_width, telop_height,
          battle, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height,
          0, telop_height * 17, telop_width, telop_height,
          battle, Color(0, 0, 0));

        // �L�����N�^�[�\���i�v���C���[���j
        {
          float i, j;
          for (i = 0; i < 5; i += 1) {
            for (j = 0; j < 5; j += 1) {
              x0 = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j);
              y0 = (-chara_height * 1) + chara_height * (2 - i * 0.5);

              drawTextureBox(x0, y0, chara_width, chara_height,
                0, 0, chara_width, chara_height,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
              // ���̕\��
              drawTextureBox(x0 + 4 + (16 * (1 + i) * 0.125), y0 + 4 + (16 * (1 + i) * 0.1), 16, 16,
                4 + (chara_width * 3) + (16 * 1), 0, 16, 16,
                chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
            }
          }
        }

        // �L�����N�^�[�\���i���葤�j
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
              // ����̕\��
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
        // �U���e���b�v�\��
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
          0, telop_height * 15, telop_width, telop_height,
          battle, Color(0, 0, 0));
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height,
          0, telop_height * 17, telop_width, telop_height,
          battle, Color(0, 0, 0));

        // �L�����N�^�[�\���i�v���C���[���j
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

        // �L�����N�^�[�\���i���葤�j
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

        // �v���C���[�̍U���͌v�Z
        {
          short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
          short tex_x, tex_y;       // �摜�؂�o���p
          int val = player.soldier * (2 + (player.fame - player.infamy) / 20) + player.mercenary * 4;

          // �X�e�[�^�X�����擾
          int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i �����O�łȂ�
            if (damage[i] != 0) {
              tex_x = number_width * damage[i];
              tex_y = telop_height * 82;
            }

            // i �����O�������Ƃ�
            else {
              // �z��̍ŏ�
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // ����ȊO
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
                if (i == 1 && damage[i - 1] == 0) {}
                else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

        // ����̍U���͌v�Z
        {
          short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
          short tex_x, tex_y;       // �摜�؂�o���p
          int val = 0;

          // �X�e�[�^�X�����擾
          int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

          for (i = 0; i < 6; i += 1) {
            // i �����O�łȂ�
            if (damage[i] != 0) {
              tex_x = number_width * damage[i];
              tex_y = telop_height * 82;
            }

            // i �����O�������Ƃ�
            else {
              // �z��̍ŏ�
              if (i == 0) {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;
              }

              // ����ȊO
              else {
                skip += 1;
                tex_x = 0;
                tex_y = telop_height * 83;

                // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
                if (i == 1 && damage[i - 1] == 0) {}
                else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
                // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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
        // �퓬���[���̐���
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height * 3,
          0, telop_height * 40, telop_width, telop_height * 3,
          eventP, Color(0, 0, 0));

        // �L�����N�^�[�\���i�v���C���[���j
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

        // �L�����N�^�[�\���i���葤�j
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
        // �퓬���[���̐���
        drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 2), telop_width, telop_height * 4,
          0, telop_height * 31, telop_width, telop_height * 4,
          eventP, Color(0, 0, 0));

        if (app_env.isPushButton(Mouse::LEFT)) {
          app_env.flushInput();
          tutorial_flag += 1;
        }

        break;

      case 9:
        // ���m�����͂��ƂȂ����Ȃ���
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

        // ����
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

// �퓬���̃��b�Z�[�W�\���ƃA�j���[�V����
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

    // �w�i
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

    // �v���C���[�̍s��
    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 5), telop_width, telop_height,
      0, telop_height * (14 + player_cmd), telop_width, telop_height,
      battle, Color(0, 0, 0));

    // ����̍s��
    drawTextureBox(-telop_width / 2, (-Window::HEIGHT / 2) + (telop_height * 3), telop_width, telop_height,
      0, telop_height * (17 + enemy_cmd), telop_width, telop_height,
      battle, Color(0, 0, 0));

    if (time < 180) {
      time += 1;
    }

    // �v���C���[���̃A�j���[�V����
    {
      for (i = 0; i < 5; i += 1) {
        for (j = 0; j < 5; j += 1) {
          // �ˌ�
          // ����ƈꏏ�ɃL�����N�^�[���ړ�
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

          // ���
          // ����i��̂݁j�ړ�
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

          // ���
          // ���̂܂ܓ����Ȃ�
          else {
            x = (-chara_width * 4) - (chara_width * (i * 0.25)) - ((chara_width * (1 + i * 0.25)) * j);
            y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
            x0 = x + 6 + (16 * (1 + i) * 0.1);
            y0 = y + (16 * (1 + i) * 0.1);
          }

          // �L�����N�^�[�\���i�v���C���[���j
          drawTextureBox(x, y, chara_width, chara_height,
            0, 0, chara_width, chara_height,
            chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          // ���̕\��
          if (player_cmd == 0) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 0), 0, 16, 16,
              chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // ���̕\��
          else if (player_cmd == 1) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 1), 0, 16, 16,
              chara6, Color(0.5, 1, 1), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // �|��̕\��
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

    // ���葤�̃A�j���[�V����
    {
      for (i = 0; i < 5; i += 1) {
        for (j = 0; j < 5; j += 1) {
          // �ˌ�
          // ����ƈꏏ�ɃL�����N�^�[���ړ�
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

          // ���
          // ����i��̂݁j�ړ�
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

          // ���
          // ���̂܂ܓ����Ȃ�
          else {
            x = (chara_width * 3) + ((chara_width * (1 + i * 0.25)) * j);
            y = (-chara_height * 1) + chara_height * (2 - i * 0.5);
            x0 = x - (10 * (1 + i) * 0.125);
            y0 = y + (16 * (1 + i) * 0.1);
          }

          // �L�����N�^�[�\���i���葤�j
          drawTextureBox(x, y, chara_width, chara_height,
            0, 0, chara_width, chara_height,
            chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));

          // ���̕\��
          if (enemy_cmd == 0) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 0), 0, 16, 16,
              chara6, Color(1, 0.5, 0.5), M_PI / 2, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // ���̕\��
          else if (enemy_cmd == 1) {
            drawTextureBox(x0, y0, 16, 16,
              4 + (chara_width * 3) + (16 * 1), 0, 16, 16,
              chara6, Color(1, 0.5, 0.5), 0, Vec2f(1 + (i * 0.25), 1 + (i * 0.25)), Vec2f(0, 0));
          }

          // �|��̕\��
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

    // �v���C���[�̍U���͌v�Z
    {
      short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
      short tex_x, tex_y;       // �摜�؂�o���p
      int val = player_damage;

      // �X�e�[�^�X�����擾
      int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

      for (i = 0; i < 6; i += 1) {
        // i �����O�łȂ�
        if (damage[i] != 0) {
          tex_x = number_width * damage[i];
          tex_y = telop_height * 82;
        }

        // i �����O�������Ƃ�
        else {
          // �z��̍ŏ�
          if (i == 0) {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;
          }

          // ����ȊO
          else {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;

            // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
            if (i == 1 && damage[i - 1] == 0) {}
            else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

    // ����̍U���͌v�Z
    {
      short i, skip = 0;        // �摜�\���ʒu�v�Z�A��
      short tex_x, tex_y;       // �摜�؂�o���p
      int val = enemy_damage;

      // �X�e�[�^�X�����擾
      int damage[6] = {(val / 100000), (val % 100000) / 10000, (val % 10000) / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10};

      for (i = 0; i < 6; i += 1) {
        // i �����O�łȂ�
        if (damage[i] != 0) {
          tex_x = number_width * damage[i];
          tex_y = telop_height * 82;
        }

        // i �����O�������Ƃ�
        else {
          // �z��̍ŏ�
          if (i == 0) {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;
          }

          // ����ȊO
          else {
            skip += 1;
            tex_x = 0;
            tex_y = telop_height * 83;

            // ���ݎQ�Ƃ��Ă��錅����̌������ׂĂO���m�F
            if (i == 1 && damage[i - 1] == 0) {}
            else if (i == 2 && (damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 3 && (damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            else if (i == 4 && (damage[i - 4] || damage[i - 3] || damage[i - 2] || damage[i - 1]) == 0) {}
            // �O�ł͂Ȃ������Ƃ��A�X�L�b�v���Ȃ�
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

// �R�͂𔽗������ŃN���A
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

// �S�͂������B���ŃN���A
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

// �G�N�X�g���X�e�[�W���N���A
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
