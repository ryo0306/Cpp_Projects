
#include "Gimmick.h"

using namespace frameworks::object;


StageGimmick::StageGimmick(const int direction,
                           const Vec2f& pos) {
  this->direction = direction;
  transform.pos = pos;
  transform.scale = Vec2f::Ones() * 50.0f;
  transform.rotate = transform.angle = 0.0f;
}


void StageGimmick::Draw() {
  // ‰æ‘œ‘Ò‚¿
  /*
  const auto id = GameData::Get().GetContainer()[direction];
  const auto texture = Asset().Find().Texture(id);
  const float Size = 0.0f;

  drawTextureBox(transform.pos.x(), transform.pos.y(),
                 transform.scale.x(), transform.scale.y(),
                 0, 0, Size, Size, *texture);
  */
  drawFillBox(transform.pos.x(), transform.pos.y(),
              transform.scale.x(), transform.scale.y(),
              Color::yellow);
}


Color StageGimmick::GetColor() {
  Color color;

  switch (direction) {
    case Top:
      color = Color::red;
      break;
    case Right:
      color = Color::blue;
      break;
    case Bottom:
      color = Color::yellow;
      break;
    case Left:
      color = Color::green;
      break;
    default:
      color = Color::white;
      break;
  }

  return color;
}
