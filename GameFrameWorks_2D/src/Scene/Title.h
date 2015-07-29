
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
  std::vector<u_int> textureID;
  std::vector<u_int> mediaID;
};

}  // end scene
}  // end frameworks
