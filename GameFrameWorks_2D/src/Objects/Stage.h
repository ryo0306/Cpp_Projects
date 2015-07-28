
#pragma once
#include "Object.hpp"
#include <vector>


namespace frameworks {
namespace object {

struct Block {
  Vec2f pos, size;
  u_int id;
};
typedef std::vector<Block>  Blocks;

class Stage {
public:
  Stage();

  void Update();
  void Draw();

  const Blocks& GetTransforms() const { return blocks; }

private:
  Blocks blocks;
  std::vector<u_int> textureID;
};

}
}
