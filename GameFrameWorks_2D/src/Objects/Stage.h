
#pragma once
#include "Object.hpp"
#include <vector>


namespace frameworks {
namespace object {

class Stage {
public:
  Stage() = default;
  void Setup();
  void GoalSetup(const Block&, const int);

  void Update();
  void Draw();

  void Goal();
  void GoalDraw();

  const Blocks& GetTransforms() const { return blocks; }
  const Block& GetGoalPos() const { return goal; }

  void BackDraw();

private:
  Blocks blocks;
  std::vector<u_int> blockID;

  Block goal;
  std::vector<u_int> goalID;
  u_int drawID;
  u_int direction;

  std::vector<u_int> backID;
};

}
}
