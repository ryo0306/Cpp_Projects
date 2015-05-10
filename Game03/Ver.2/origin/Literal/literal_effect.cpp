
#include "literal_effect.h"
#include "literal_common.hpp"
#include "literal_font.hpp"
#include "literal_title.hpp"


const float literal_effect::backAlpha       = 0.50f;
const float literal_effect::scrollSpeed     = 0.25f;
const Vec2f literal_effect::defaultScale    = Vec2f(1, 1);
const Vec2f literal_effect::mapScale        = Vec2f(1.01f, 1.02f);
const Vec2f literal_effect::logoScale       = Vec2f(3.0f, 3.0f);
const Vec2f literal_effect::centerTranslate = Vec2f(WIDTH / 2, HEIGHT / 2);
const Vec2f literal_effect::logoTranslate   = Vec2f(literal_title::Logo_W / 2, 0);

const short literal_effect::descriptDrawTime = spec::Fps * 3;
const short literal_effect::blinkSpeed  = 3;
const float literal_effect::buttonMoveSpeed = 15.0f;
const float literal_effect::buttonAlphaSpeed = 0.05f;
const float literal_effect::buttonAlpha = 0.75f;
const float literal_effect::buttonFrameAlpha = 0.25f;
const float literal_effect::fadeSpeed   = 0.05f;
const Vec2f literal_effect::buttonSize  = Vec2f(button::Small_W, button::Small_H);
const Vec2f literal_effect::iconPosTranslate = Vec2f(5, 10);

const Vec2f literal_effect::unitSize = Vec2f(32, 32);
const Vec2f literal_effect::iconSize = Vec2f(64, 64);
