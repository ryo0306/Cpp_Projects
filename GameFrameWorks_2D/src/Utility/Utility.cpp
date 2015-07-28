
#include "DrawTexture.h"
#include <map>


namespace frameworks {
namespace utility {

const Vec2f& OffsetTextureOrigin(const Origin info) {
  static std::map<Origin, Vec2f> offsetTable = {
    { Origin::LeftTop,      Vec2f(0.0f, 1.0f) },
    { Origin::Left,         Vec2f(0.0f, 0.5f) },
    { Origin::LeftBottom,   Vec2f(0.0f, 0.0f) },
    { Origin::CenterTop,    Vec2f(0.5f, 1.0f) },
    { Origin::Center,       Vec2f(0.5f, 0.5f) },
    { Origin::CenterBottom, Vec2f(0.5f, 0.0f) },
    { Origin::RightTop,     Vec2f(1.0f, 1.0f) },
    { Origin::Right,        Vec2f(1.0f, 0.5f) },
    { Origin::RightBottom,  Vec2f(1.0f, 0.0f) },
  };

  return offsetTable.find(info)->second;
}

void DrawTexture(const Transform& transform,
                 const Sprite& sprite,
                 const Vec2f& offset) {
  const Vec2f& scale = transform.scale;
  const auto origin = Vec2f(scale.x() * offset.x(),
                            scale.y() * offset.y());

  drawTextureBox(transform.pos.x(), transform.pos.y(), 1, 1,
                 sprite.start.x(), sprite.start.y(),
                 sprite.size.x(), sprite.size.y(),
                 sprite.texture, sprite.color,
                 transform.rotate, transform.scale, origin);
}

bool IsHitRectToRect(const Transform& obj1,
                     const Transform& obj2) {
  const auto& obj1Pos = obj1.pos;
  const auto& obj1Size = obj1.scale;
  const auto& obj2Pos = obj2.pos;
  const auto& obj2Size = obj2.scale;

  //const bool hitL = (obj2Pos.x() - (obj1Pos.x() + obj1Size.x())) < 0;
  const bool hitL = obj1Pos.x() + obj1Size.x() > obj2Pos.x();
  const bool hitR = obj1Pos.x() < obj2Pos.x() + obj2Size.x();

  const bool hitT = obj1Pos.y() < obj2Pos.y() + obj2Size.y();
  const bool hitB = obj1Pos.y() + obj1Size.y() > obj2Pos.y();

  return ((hitL && hitR) && (hitT && hitB)) || ((hitL && hitR) && (hitT && hitB));
}

}  // end utility
}  // end frameworks