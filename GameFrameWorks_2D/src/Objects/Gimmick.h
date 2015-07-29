
#pragma once
#include "Object.hpp"


namespace frameworks {
namespace object {

class StageGimmick {
public:
  StageGimmick(const int, const Vec2f&);

  void Draw();

  const Transform& GetTransform() const { return transform; }
  const int GetDirection() const { return direction; }

private:
  Transform transform;
  int direction;

  Color GetColor();
};

}
}
