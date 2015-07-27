
#pragma once
#include <functional>


namespace frameworks {
namespace utility {

typedef std::function<void()>  Sequence;

class Sequencer {
public:
  void DoAction() {
    if (next) {
      prev = now;
      now = next;
      next = nullptr;
    }
    now();
  }

  void SetNextSequence(const Sequence& sequence = nullptr) {
    next = sequence;
  }

  void BackSequence() {
    if (prev) next = prev;
  }

private:
  Sequence prev;
  Sequence now;
  Sequence next;
};

}  // end utility
}  // end frameworks
