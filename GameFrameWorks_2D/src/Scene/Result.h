
#pragma once
#include "SceneBase.hpp"


namespace frameworks {
namespace scene {

class Result : public SceneBase {
public:
  Result();

  void Update() override;
  void Draw() override;

private:
  // �K�v�ȕϐ��A�֐��������ɍ��
};

}  // end scene
}  // end frameworks
