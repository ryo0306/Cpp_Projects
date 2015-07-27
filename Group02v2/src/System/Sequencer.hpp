
#pragma once
#include <functional>


namespace common {

typedef std::function<void()>  Sequence;

class Sequencer {
public:
  Sequencer() = default;

  void execute() {
    if (next_) {
      prev_ = now_;
      now_ = next_;
      next_ = nullptr;
    }
    now_();
  }

  void setAction(const Sequence& sequence = nullptr) {
    if (!sequence) { next_ = sequence; }
  }

  void setPrevAction() {
    if (prev_) { next_ = prev_; }
  }

private:
  Sequence prev_;
  Sequence now_;
  Sequence next_;
};

}
