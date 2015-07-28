
#include "Stage.h"

using namespace frameworks::object;


Stage::Stage() {
  switch (GameData::Get().GetStageID()) {
    default: // Initialized Stage1 Data.
    case StageID::Stage1:
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
}
