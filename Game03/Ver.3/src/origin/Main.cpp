
#include "Environment/GamePlayer.hpp"


int main() {
  env::get();

  cGamePlayer game;
  while (game.isRunning()) {
    game.update();
    game.draw();
  }
}
