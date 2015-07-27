
#pragma once
#include "SceneBase.hpp"


namespace frameworks {
namespace scene {

class MainGame : public SceneBase {
public:
  MainGame();

  void Update() override;
  void Draw() override;

private:
  // •K—v‚È•Ï”AŠÖ”‚ğ‚±‚±‚Éì‚é
};

}  // end scene
}  // end frameworks
