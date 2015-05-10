
#pragma once
#include "base.hpp"
#include <memory>


class cEnvironment {
  // TIPS: ���̃N���X�͐錾���Ȃ�
  cEnvironment() = delete;

  static Vec2f mouse_;
  static std::unique_ptr<AppEnv> app_;

public:
  // �A�v���̋N��
  // NOTICE: �K�� main() �̍ŏ��Ɏ��s����
  static void execApplication();

  static void setupDraw();
  static void update();

  static void mousePosUpdate();

  // TIPS: �J�[�\�����w�肳�ꂽ��`�͈͓̔��ɂ���� true ��Ԃ�
  static bool rectOnMouse(const Vec2f& pos,
                          const Vec2f& size);

  // TIPS: ���N���b�N�����u�Ԃ� true ��Ԃ�
  static bool leftClick();

  // TIPS: �E�N���b�N�����u�Ԃ� true ��Ԃ�
  static bool rightClick();

  // TIPS: �Q�[���E�B���h�E���J���Ă���� true ��Ԃ�
  static bool isOpen();
};

typedef cEnvironment  env;
