
#pragma once
#include "../Common.hpp"


namespace frameworks {

enum class StageID {
  None,
  Stage1,
  Stage2,
  Stage3,
  All_Stage,
};

class GameData {
public:
  static GameData& Get() {
    static GameData gameData;
    return gameData;
  }

  const StageID GetStageID() const { return stage; }
  void SetStageID(const StageID id) { stage = id; }

private:
  StageID stage;

  GameData() : stage(StageID::None) {}
};

}
