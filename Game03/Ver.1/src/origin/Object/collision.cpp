
#include "collision.h"
#include "../System/data_depot.h"

//
// “–‚½‚è”»’èiŽÀ‘•j
//

bool Collision::rectOnMouse(const Vec2f& pos, const Vec2f& size) {
  if (win::mouse.x() > pos.x() && win::mouse.x() < pos.x() + size.x() &&
      win::mouse.y() > pos.y() && win::mouse.y() < pos.y() + size.y()) {
    return true;
  }
  return false;
}
