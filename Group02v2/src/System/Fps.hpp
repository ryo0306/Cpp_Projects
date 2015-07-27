
#pragma once
#include <chrono>


namespace common {

class Fps {
public:
  static Fps& get() {
    static Fps s_fps;
    return s_fps;
  }

  void update() {
    using namespace std::chrono;
    now_ = std_clock::now();
    elapsed_time_ = duration_cast<m_sec>(now_ - prev_);
    prev_ = now_;
  }

  const double getElapsedFrame() const {
    return static_cast<double>(elapsed_time_.count()) / frame_rate_;
  }

  void setFrameRate(double fps = 60.0f) { frame_rate_ = fps; }

private:
  typedef std::chrono::steady_clock  std_clock;
  typedef std::chrono::microseconds  m_sec;

  std_clock::time_point prev_, now_;
  m_sec elapsed_time_;
  double frame_rate_;

  Fps() :
    prev_(std_clock::now()),
    now_(std_clock::now()),
    frame_rate_(60.0f) {}
};

}
