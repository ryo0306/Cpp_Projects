
#include "Gimmick.h"

using namespace frameworks::object;


StageGimmick::StageGimmick(const int direction,
                           const Vec2f& pos,
                           const Vec2f& size) :
direction(direction) {
  pushState = direction != GravityDirection::Bottom ? 0 : 4;

  transform.pos = pos;
  transform.scale = size;
  transform.rotate = transform.angle = 0.0f;
}


void StageGimmick::Draw() {
  const auto id = GameData::Get().GimmickID()[direction + pushState];
  const auto texture = Asset().Find().Texture(id);
  const float Size = 100.0f;

  drawTextureBox(transform.pos.x(), transform.pos.y(),
                 transform.scale.x(), transform.scale.y(),
                 0, 0, Size, Size, *texture);
}
