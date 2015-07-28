
#include "Stage.h"

using namespace frameworks::object;


Stage::Stage() {
  switch (GameData::Get().GetStageID()) {
  default: // Initialized Stage1 Data.
  case StageID::Stage1:
    blocks.push_back(std::make_pair(Vec2f(-700, -200), Vec2f(100, 500)));
    blocks.push_back(std::make_pair(Vec2f(-700, -200), Vec2f(400, 100)));
    blocks.push_back(std::make_pair(Vec2f(-400, -300), Vec2f(400, 100)));
    blocks.push_back(std::make_pair(Vec2f(700, -200), Vec2f(100, 500)));
    break;
  
  case StageID::Stage2:
    break;

  case StageID::Stage3:
    break;
  }
}


void Stage::Update() {
  // do not something.
  // test
}


void Stage::Draw() {
  for (const auto& block : blocks){
    drawFillBox(block.first.x(), block.first.y(),
                block.second.x(), block.second.y(), 
                Color::white);
  }
}
