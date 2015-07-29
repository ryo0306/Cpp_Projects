
#pragma once
#include "../Common.hpp"
#include "../Resources/Resource.h"
#include "../Utility/GameData.h"
#include <memory>


namespace frameworks {
namespace scene {

enum class SceneName {
  None,

  Title,
  Main,
  Result,
  Ending,

  All_Scene,
};


class SceneBase {
public:
  virtual void Update() {}
  virtual void Draw() = 0;

  const SceneName GetSceneName() const { return own; }
  const SceneName GetNextScene() const { return next; }

  const bool IsFinish() const { return isFinish; }

protected:
  bool isFinish;
  SceneName own, next;

  SceneBase(const SceneName o, const SceneName n) :
    isFinish(false), own(o), next(n) {}
};

}  // end scene
}  // end frameworks

namespace fw = frameworks;
typedef std::shared_ptr<fw::scene::SceneBase>  SceneRef;
