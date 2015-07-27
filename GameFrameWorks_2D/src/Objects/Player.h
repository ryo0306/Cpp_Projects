
#pragma once
#include "Object.hpp"


namespace frameworks {
namespace object {

class Player : public Object {
public:
  Player(const Vec2f&);

  void Update() override;
  void Draw() override;

private:
};

}  // end object
}  // end frameworks
