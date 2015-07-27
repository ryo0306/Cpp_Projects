
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

  // アプリの管理クラスを取得
  AppEnv& env() const { return *env_; }

  // 任意のタイミングでリソースを解放する
  void destroy() {
    if (!env_) return;

    // TIPS: release() は所有権の放棄であって解放ではない点に注意
    auto p = env_.release();
    delete p;
  }

private:
  std::unique_ptr<AppEnv> env_;

  App() : env_(std::make_unique<AppEnv>(WIDTH, HEIGHT, false, true)) {}
};

}
