
#pragma once
#include "lib/defines.hpp"
#include "lib/appEnv.hpp"
#include <memory>


namespace common {

enum WindowSize {
  WIDTH = 1200,
  HEIGHT = 900,
};


enum KeyBind {
  ESCAPE = GLFW_KEY_ESCAPE,
};


class App {
public:
  ~App() { destroy(); }

  static App& get() {
    static App s_app;
    return s_app;
  }

  // �A�v���̊Ǘ��N���X���擾
  AppEnv& env() const { return *env_; }

  // �C�ӂ̃^�C�~���O�Ń��\�[�X���������
  void destroy() {
    if (!env_) return;

    // TIPS: release() �͏��L���̕����ł����ĉ���ł͂Ȃ��_�ɒ���
    auto p = env_.release();
    delete p;
  }

private:
  std::unique_ptr<AppEnv> env_;

  App() : env_(std::make_unique<AppEnv>(WIDTH, HEIGHT, false, true)) {}
};

}
