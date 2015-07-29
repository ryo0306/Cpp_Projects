
#pragma once
#include "Object.hpp"


namespace frameworks {
namespace object {

class StageGimmick {
public:
  StageGimmick(const int, const Vec2f&, const Vec2f&);

  void Draw();

  Transform& GetTransform() { return transform; }
  const int GetDirection() const { return direction; }
  int& GetState() { return pushState; }

private:
  Transform transform;
  int direction;
  int pushState;
};

}
}
