
#pragma once
#include "../Common.hpp"


namespace common {

class GamePlayer {
public:
  GamePlayer() {
  }

  void update() {
  }

  void draw() {
  }

  const bool isRunning() const {
    auto& app = App::get().env();
    return app.isOpen() && app.isPushKey(ESCAPE);
  }

private:
};

}
