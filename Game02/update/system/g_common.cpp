//
// �ėp�֐����X�g
//

#pragma once
#include "../App_Lib/appEnv.hpp"
#include "system.hpp"
#include <vector>


/* �ėp�֐� */
////////////////////////////////////////////////////////////////////////////////

// �}�E�X�J�[�\������
bool CO_on_mouse(Vec2f pos, float x, float y, float tex_w, float tex_h) {

  if (pos.x() > x && pos.x() < (x + tex_w) &&
      pos.y() > y && pos.y() < (y + tex_h)) {
    return true;
  }
  return false;
}

// ���l�̕\��
void CO_disp_number(Texture& sys, short val, float x, float y) {

  // ���I�z��ƃC�e���[�^�̐錾
  std::vector<short> v;
  std::vector<short>::iterator it;

  // �����̊e���̒l�𒊏o����
  do {
    v.emplace_back(val % 10);
    val /= 10;
  } while (val > 0);

  // �z��̏����ʒu����
  it = v.begin();

  // �e���̒l��\��
  for (int i = 0; it != v.end(); ++i, ++it) {
    drawTextureBox(x - (Number_W * i), y, Number_W, Telop_H,
      Number_W * (*it), Telop_H * 2, Number_W, Telop_H,
      sys, Color(1, 1, 1));
  }
}


////////////////////////////////////////////////////////////////////////////////
