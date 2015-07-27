
#include "System/GamePlayer.hpp"

using namespace common;


int main() {
  App::get();
  GamePlayer game;

  while (game.isRunning()) {
    game.update();
    game.draw();
  }
  App::get().destroy();
}
