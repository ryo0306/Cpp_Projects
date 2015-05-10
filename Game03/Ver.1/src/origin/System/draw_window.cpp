
#include "draw_window.h"


// �J���[���
std::array<Color, paint::ColorVariation> win::color_type_;


// �E�B���h�E�����t���O
// TIPS: ��x����������ȍ~�̖��߂𖳎�����
bool win::isCreate_ = false;


// �Q�[�����s���̃C���X�^���X
// �`�揀���A�X�V�A���͂Ȃ�
std::unique_ptr<AppEnv> win::app;


// �}�E�X�J�[�\���ʒu�i���A���j
Vec2f win::mouse;


// �E�B���h�E��������
// TIPS: �K�� main() �̈�ԍŏ��ɌĂяo���悤�ɂ���
void win::create() {
  if (isCreate_) { return; }

  app = std::make_unique<AppEnv>(size::WIDTH, size::HEIGHT, false, true);

  color_type_[paint::White]   = Color(1.f, 1.f, 1.f);
  color_type_[paint::Gray]    = Color(0.25f, 0.25f, 0.25f);
  color_type_[paint::Black]   = Color(0.f, 0.f, 0.f);

  color_type_[paint::Red]     = Color(1.f, 0.2f, 0.2f);
  color_type_[paint::Pink]    = Color(1.f, 0.5f, 0.5f);
  color_type_[paint::Purple]  = Color(0.8f, 0.f, 0.8f);
  color_type_[paint::Blue]    = Color(0.f, 0.f, 0.8f);
  color_type_[paint::SkyBlue] = Color(0.f, 0.5f, 1.f);
  color_type_[paint::Green]   = Color(0.f, 1.f, 0.f);
  color_type_[paint::Yellow]  = Color(1.f, 1.f, 0.f);
  color_type_[paint::Orange]  = Color(1.f, 0.75f, 0.f);
  color_type_[paint::Brown]   = Color(0.75f, 0.5f, 0.f);

  isCreate_ = true;
}


// �}�E�X���W�̎擾
void win::mouseTranslate() {
  mouse = app->mousePosition();
}


// �Q�[�����E�B���h�E�̕`��
// TIPS: �t���[���̓����x���ύX����
void win::draw(const Vec2f& pos, const Vec2f& size, const Color& col, const float& alpha) {

  // TIPS: �n���ꂽ�T�C�Y��񂪏���������ꍇ�A�`�悵�Ȃ�
  if (size.x() < 10.f || size.y() < 10.f) { return; }

  // �E�B���h�E�̔w�i�`��
  drawFillBox(pos.x() + 3, pos.y() + 3, size.x() - 6, size.y() - 6, col);

  // TIPS: �t���[���J���[�̈ꊇ�ύX
  Color frame_color = color_type_[paint::White];
  frame_color.alpha() = alpha;

  // �E�B���h�E�t���[���̕`��
  {
    drawBox(pos.x() + 2, pos.y() + 2, size.x() - 4, size.y() - 4, 4, frame_color);

    // TIPS: �t���[���̎l����⊮
    drawFillCircle(pos.x() + 2, pos.y() + 2,
      2, 2, 4, frame_color);
    drawFillCircle(pos.x() + 2, pos.y() + size.y() - 2,
      2, 2, 4, frame_color);
    drawFillCircle(pos.x() + size.x() - 2, pos.y() + 2,
      2, 2, 4, frame_color);
    drawFillCircle(pos.x() + size.x() - 2, pos.y() + size.y() - 2,
      2, 2, 4, frame_color);
  }
}


void win::gaugeDraw(const Vec2f& pos, const Vec2f& size, const float& per, const Color& col) {
  // �Q�[�W�̕`��
  {
    // �w�i
    drawFillBox(pos.x() + 2, pos.y() + 2,
      size.x() - 4, size.y() - 4, color(paint::Black));

    // ���ۂ̃Q�[�W
    drawFillBox(pos.x() + 2, pos.y() + 2,
      (size.x() - 4) * per, size.y() - 4, col);
  }

  // �t���[���̕`��
  {
    Color& frame_color = color_type_[paint::White];

    drawBox(pos.x() + 1.5f, pos.y() + 1.5f,
      size.x() - 3, size.y() - 3, 3, frame_color);
  }
}


Color win::color(const short& mode, const float& alpha) {
  Color temp = color_type_[mode];
  temp.alpha() = alpha;
  return temp;
}
