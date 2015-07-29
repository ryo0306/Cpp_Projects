
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

  u_int select;
  std::vector<u_int> icons;
};

}  // end scene
}  // end frameworks
