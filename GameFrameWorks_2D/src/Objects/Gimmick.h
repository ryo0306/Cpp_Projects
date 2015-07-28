
#pragma once
#include "Object.hpp"


namespace frameworks {
namespace object {

class StageGimmick {
public:
  StageGimmick(int direction, Vec2f pos);

  void Draw();
  Transform GetTransform(){ return transform; }
  int GetDirection(){ return direction; }

private:
	Transform transform;
	int direction;

	Color GetColor();
};

}
}
