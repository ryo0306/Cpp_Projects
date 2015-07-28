
#include "GamePlayer.hpp"
#include "Utility/Fps.h"

namespace frameworks {
void ApplicationSetup();
void ApplicationRelease();
}

using namespace frameworks;
using namespace frameworks::scene;


int main() {
  ApplicationSetup();

  GamePlayer game(SceneName::Main);
  while (game.IsRunning()) {
    utility::FrameUpdate();
    game.Update();
    game.Draw();
  }

  ApplicationRelease();
}
