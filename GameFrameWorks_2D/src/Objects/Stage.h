
#pragma once
#include "Object.hpp"
#include <vector>


namespace frameworks {
namespace object {

typedef std::pair<Vec2f, Vec2f>  Collision;
typedef std::vector<Collision>   Collisions;

class Stage {
public:
  Stage();

  void Update();
  void Draw();

  const Collisions& GetTransforms() const { return blocks; }

private:
  Collisions blocks;
};

}
}
