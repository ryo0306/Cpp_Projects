
#include "FrameWork/game_player.hpp"


int main() {
  env::get();

  cGamePlayer game;

  while (game.isRunning()) {
    game.update();
    game.draw();
  }
}
