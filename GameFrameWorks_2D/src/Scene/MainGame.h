
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
  // �K�v�ȕϐ��A�֐��������ɍ��
};

}  // end scene
}  // end frameworks
