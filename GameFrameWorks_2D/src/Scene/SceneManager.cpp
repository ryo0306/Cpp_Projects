
#include "SceneManager.h"
#include "SceneMaker.h"

using namespace frameworks;
using namespace frameworks::scene;
using namespace frameworks::utility;


SceneManager::SceneManager(const SceneName scene) :
alpha(0.0f),
alphaSpeed(0.02f),
isFade(false) {
  current = SceneMaker::Create(scene);
  sequencer.SetNextSequence(std::bind(&SceneManager::FadeWait, this));
}


void SceneManager::Update() {
  isFade ? sequencer.DoAction() : current->Update();

  if (!current->IsFinish() || isFade) return;

  isFade = true;
  sequencer.SetNextSequence(std::bind(&SceneManager::FadeOut, this));
}


void SceneManager::Draw() {
  current->Draw();
  HideScreen();
}


void SceneManager::FadeWait() {
  // do not something.
}


void SceneManager::FadeOut() {
  alpha += alphaSpeed;
  
  if (alpha < 1.0f) return;

  alpha = 1.0f;
  sequencer.SetNextSequence(std::bind(&SceneManager::FadeIn, this));

  current = SceneMaker::Create(current->GetNextScene());
}


void SceneManager::FadeIn() {
  alpha -= alphaSpeed;
  
  if (alpha > 0.0f) return;

  alpha = 0.0f;
  isFade = false;
  sequencer.SetNextSequence(std::bind(&SceneManager::FadeWait, this));
}


void SceneManager::HideScreen() {
  const auto size = Env().viewSize();
  drawFillBox(-size.x() / 2, -size.y() / 2,
              size.x(), size.y(),
              Color(0, 0, 0, alpha));
}
