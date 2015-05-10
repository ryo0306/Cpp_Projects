
#include "../../lib/defines.hpp"
#include "../../lib/graph.hpp"
#include "../Environment/Environment.h"
#include "../Environment/SceneManager.h"
#include "../Resource/Sprite.h"
#include <vector>


cMain::cMain() {
  spriteSetup();
  mediaSetup();

  md::get().musicPlay(M_Thema);
}

void cMain::update() {
}

void cMain::draw() {
  drawFillCircle(0, 0, 50, 50, 50, Color::white);
}

pScene cMain::nextScene() {
  md::get().musicStop();
  return nullptr;
  //return std::make_shared<>();
}

void cMain::spriteSetup() {
  sprt::get().spriteDelete();

  const SpriteInfo TABLE[] = {
    { Back_Ground, "res/png/back_title.png", Vec2f(512, 512) },
  };

  for (auto& it : TABLE) sprt::get().spriteAppend(it);
}

void cMain::mediaSetup() {
  md::get().mediaDelete();

  const MediaInfo TABLE[] = {
    { M_Thema, "res/wav/strategy.wav" },
  };

  for (auto& it : TABLE) md::get().mediaAppend(it);
  md::get().mediaGainTuning();
}

void cMain::drawBackGround() {
}
