
#pragma once
#include "Object.hpp"


namespace frameworks {
namespace object {

class Player : public Object {
public:
  Player(const Vec2f&);

  void Update() override;
  void Draw() override;
  void GravityReset();

  bool GetKeyActive() { return time != 0; }

  void SetGravityDirection();

private:
	enum {
		KeyActiveTime = 5,
		Move = 5,
		Gravity = 1,
	};
	enum {
		Top,
		Right,
		Bottom,
		Left,
	};

	int direction;
	Vec2f accela;
	Vec2f velocity;
	int time;
	

};

}  // end object
}  // end frameworks
