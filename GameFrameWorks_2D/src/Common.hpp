
#pragma once
#include "lib/defines.hpp"
#include "lib/appEnv.hpp"


namespace frameworks {

enum WindowSize {
  WIDTH  = 1200,
  HEIGHT = 900,
};

// �L�[�{�[�h���͂̈ꗗ
enum KeyBind {
  ESCAPE = GLFW_KEY_ESCAPE,
  ENTER  = GLFW_KEY_ENTER,
};

// ���W�A�T�C�Y�A��]��ԁA��]���x
struct Transform {
  Vec2f pos, scale;
  float rotate;
  float angle;
};

// �摜�A�؂�o���J�n�ʒu�A�؂���T�C�Y�A�F
struct Sprite {
  Texture texture;
  Vec2f start, size;
  Color color;
};

// �A�v���P�[�V�����@�\�̌Ăяo��
AppEnv& Env();

}  // end frameworks
