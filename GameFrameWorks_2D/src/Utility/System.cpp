
#include "../Common.hpp"
#include "Fps.h"
#include <chrono>


namespace frameworks {

class App {
public:
  static App& Get() {
    static App s_app;
    return s_app;
  }

  AppEnv& Env() { return *app; }

  void Release() {
    if (!app) return;

    auto p = app.release();
    delete p;
  }

private:
  std::unique_ptr<AppEnv> app;

  App() : app(std::make_unique<AppEnv>(WIDTH, HEIGHT, false, true)) {}
  ~App() { Release(); }
};

class Fps {
public:
  static Fps& Get() {
    static Fps fps;
    return fps;
  }

  void Update() {
    using namespace std::chrono;
    now = std_clock::now();
    elapsedTime = duration_cast<m_sec>(now - prev);
    prev = now;
  }

  // 精度  1 / ミリ秒 (milli seconds)
  const inline double DeltaTime() const {
    // TIPS: 整数型で返されるので、単位を変換して出力
    return static_cast<double>(elapsedTime.count()) * 0.001;
  }

private:
  typedef std::chrono::steady_clock  std_clock;
  typedef std::chrono::milliseconds  m_sec;

  std_clock::time_point now;
  std_clock::time_point prev;

  m_sec elapsedTime;

  Fps() : now(std_clock::now()), prev(std_clock::now()) {}
};


AppEnv& Env() { return App::Get().Env(); }

void AssetManagerSetup();
void AssetRelease();

void ApplicationSetup() {
  App::Get();
  AssetManagerSetup();
}

void ApplicationRelease() {
  App::Get().Release();
  AssetRelease();
}

namespace ut = utility;

void ut::FrameUpdate() { Fps::Get().Update(); }

const double ut::GetElapsedFrame() { return Fps::Get().DeltaTime(); }

}  // end frameworks
