
#include "../../lib/defines.hpp"
#include "../../lib/graph.hpp"
#include "../Environment/Environment.h"
#include "../Resource/Sprite.h"
#include "Anim.h"


cAnim::AnimationPrm::AnimationPrm() :
FOG_SPD_(0.2f),
SEA_SPD_(0.1f),
SCROLL_LIMIT_(256.0f),
FOG_COLOR_(1.0f, 1.0f, 1.0f, 0.5f),
SEA_COLOR_(0.0f, 0.5f, 1.0f, 0.5f) {}

cAnim::cAnim() :
scroll_fog_(0.0f),
scroll_sea_(0.0f) {
  const Vec2f PARTS_SIZE(256, 256);

  const SpriteInfo TABLE[] = {
    { Back_Fog,    "res/png/world_fog.png",   PARTS_SIZE },
    { Back_Sea_1,  "res/png/world_sea_1.png", PARTS_SIZE },
    { Back_Sea_2,  "res/png/world_sea_2.png", PARTS_SIZE },
  };

  for (auto& it : TABLE) sprt::get().spriteAppend(it);
}

void cAnim::drawFog() {
  scroll_fog_ = scroll_fog_ < anim_prm.SCROLL_LIMIT_
    ? scroll_fog_ + anim_prm.FOG_SPD_
    : 0.0f;

  const Texture& FOG = sprt::get().sprite(Back_Fog);
  const Vec2f& SIZE = sprt::get().size(Back_Fog);

  const Vec2f& WINDOW = env::getWindowSize();
  const Vec2f POS = -WINDOW * 0.5f;

  drawTextureBox(POS.x(), POS.y(),
                 WINDOW.x(), WINDOW.y(),
                 scroll_fog_, 0,
                 SIZE.x(), SIZE.y(),
                 FOG, anim_prm.FOG_COLOR_);
}

void cAnim::drawSea() {
  scroll_sea_ = scroll_sea_ < anim_prm.SCROLL_LIMIT_
    ? scroll_sea_ + anim_prm.SEA_SPD_
    : 0.0f;

  const Texture& SEA_1 = sprt::get().sprite(Back_Sea_1);
  const Texture& SEA_2 = sprt::get().sprite(Back_Sea_2);
  const Vec2f& SIZE = sprt::get().size(Back_Sea_1);

  const Vec2f& WINDOW = env::getWindowSize();
  const Vec2f POS = -WINDOW * 0.5f;

  drawTextureBox(POS.x(), POS.y(),
                 WINDOW.x(), WINDOW.y(),
                 scroll_sea_, 0,
                 SIZE.x(), SIZE.y(),
                 SEA_1);

  drawTextureBox(POS.x(), POS.y(),
                 WINDOW.x(), WINDOW.y(),
                 scroll_fog_, 0,
                 SIZE.x(), SIZE.y(),
                 SEA_2, anim_prm.SEA_COLOR_);
}
