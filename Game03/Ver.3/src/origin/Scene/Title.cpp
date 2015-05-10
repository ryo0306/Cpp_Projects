
#include "../../lib/defines.hpp"
#include "../../lib/graph.hpp"
#include "../Environment/Environment.h"
#include "../Environment/SceneManager.h"
#include "../Resource/Sprite.h"
#include <vector>


cTitle::cTitle() {
  spriteSetup();
  mediaSetup();

  md::get().musicPlay(M_Title);
}

void cTitle::update() {
  if (env::get().isPushButton(Mouse::LEFT)) is_finish_ = true;
}

void cTitle::draw() {
  anim_.drawSea();
  drawBackGround();
  anim_.drawFog();
}

pScene cTitle::nextScene() {
  sprt::get().spriteDelete();
  md::get().musicStop();
  return std::make_shared<cMain>();
}

void cTitle::spriteSetup() {
  const SpriteInfo TABLE[] = {
    { Back_Ground, "res/png/back_title.png",  Vec2f(512, 512) },
  };

  for (auto& it : TABLE) sprt::get().spriteAppend(it);
}

void cTitle::mediaSetup() {
  md::get().mediaDelete();

  const MediaInfo TABLE[] = {
    { M_Title, "res/wav/title.wav" },
  };

  for (auto& it : TABLE) md::get().mediaAppend(it);
  md::get().mediaGainTuning();
}

void cTitle::drawBackGround() {
  const Vec2f& WINDOW = env::getWindowSize();
  const Vec2f POS = WINDOW * 0.5f;

  const Vec2f& SPRT = sprt::get().size(Back_Ground);

  drawTextureBox(-POS.x(), -POS.y(), WINDOW.x(), WINDOW.y(),
                 0, 0, SPRT.x(), SPRT.y(),
                 sprt::get().sprite(Back_Ground),
                 anime.GROUND_COLOR_);
}
