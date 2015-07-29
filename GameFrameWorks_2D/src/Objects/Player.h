
#pragma once
#include "Object.hpp"
#include <vector>


namespace frameworks {
namespace object {

class Player : public Object {
public:
  Player();
  void Setup();

  void Update() override;
  void Draw() override;

  void Start(const Vec2f&, const float);
  void CollisionSetup(const Blocks& blocks) { stageBlocks = blocks; }

  void GravityUpdate();
  void GravityReset() { velocity = 0.0f; }
  void SetGravityDirection(const int dir) { gravityState = dir; }

  const bool IsKeyActive() const { return time > 0; }

private:
  enum {
    KeyActiveTime = 1,
    MoveSpeed = 10,
  };

  Vec2f start;
	
	int time;
  u_int gravityState;
	int direction;

  float acceleration;
	float velocity;

  std::vector<u_int> textureID;

  Blocks stageBlocks;

  void Move(const Vec2f&);
  const bool DisableMove();
};

}  // end object
}  // end frameworks
