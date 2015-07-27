
#include "SceneMaker.h"
#include "Title.h"
#include "MainGame.h"
#include "Result.h"
#include <map>

using namespace frameworks::scene;


SceneRef SceneMaker::Create(const SceneName scene) {
  std::map<SceneName, MakeScene> scene_list = {
    { SceneName::Title,  [] { return std::make_shared<Title>();    } },
    { SceneName::Main,   [] { return std::make_shared<MainGame>(); } },
    { SceneName::Result, [] { return std::make_shared<Result>();   } },
  };
  return (scene_list.find(scene)->second)();
}
