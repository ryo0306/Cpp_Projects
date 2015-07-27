
#pragma once
#include "../Common.hpp"
#include <memory>


namespace frameworks {
namespace object {

enum ObjectState {
  ALIVE = -1,
  DEAD = 0,

  All_State,
};

class Object {
public:
  Object(const Vec2f& pos) : deadTime(ALIVE) {}
  virtual ~Object() = default;

  virtual void Update() {}
  virtual void Draw() = 0;

  // オブジェクトの削除カウンタを減らす
  void DeadTimeUpdate() { if (deadTime > DEAD) --deadTime; }

  // オブジェクトの削除フラグを返す
  const bool IsDead() const { return deadTime == DEAD; }

  // 座標などを返す
  const Transform& GetTransform() const { return transform; }

protected:
  int deadTime;          // 削除カウンタ：0 のとき、削除される
  Transform transform;   // 座標、サイズ、回転状態、回転速度
};

}  // end object
}  // end frameworks

typedef std::shared_ptr<frameworks::object::Object>  ObjectRef;
