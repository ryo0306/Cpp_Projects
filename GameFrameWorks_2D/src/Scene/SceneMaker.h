
#pragma once
#include "SceneBase.hpp"
#include <functional>


namespace frameworks {
namespace scene {

class SceneMaker {
public:
  static SceneRef Create(const SceneName);

private:
  typedef std::function<SceneRef()>  MakeScene;
};

}  // end scene
}  // end common
