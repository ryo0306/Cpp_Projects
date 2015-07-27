
#pragma once
#include "SceneBase.hpp"


namespace frameworks {
namespace scene {

class Title : public SceneBase {
public:
  Title();

  void Update() override;
  void Draw() override;

private:
  // •K—v‚È•Ï”AŠÖ”‚ğ‚±‚±‚Éì‚é
  u_int textureID;
  u_int mediaID;
};

}  // end scene
}  // end frameworks
