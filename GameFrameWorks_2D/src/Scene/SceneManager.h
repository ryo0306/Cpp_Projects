
#pragma once
#include "SceneBase.hpp"
#include "../Utility/Sequencer.hpp"
#include <functional>


namespace frameworks {

class SceneManager {
public:
  SceneManager(const scene::SceneName);

  void Update();
  void Draw();

private:
  SceneRef current;

  utility::Sequencer sequencer;

  float alpha;
  const float alphaSpeed;

  bool isFade;

  void FadeWait();
  void FadeOut();
  void FadeIn();

  void HideScreen();
};

}  // end frameworks
