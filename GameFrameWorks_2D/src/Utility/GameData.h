
#pragma once
#include "../Common.hpp"


namespace frameworks {

enum StageID {
  Stage1,
  Stage2,
  Stage3,
  All_Stage,
  None,
};

typedef std::vector<u_int>  TextureList;

class GameData {
public:
  static GameData& Get() {
    static GameData gameData;
    return gameData;
  }

  TextureList& GimmickID() { return gimmickID; }
  u_int& GimmickCounter() { return gimmickCounter[stage]; }
  void CountReset() { gimmickCounter[stage] = 0; }

  void RankingUpdate() {
    if (rankingCounter[stage] < gimmickCounter[stage])
      rankingCounter[stage] = gimmickCounter[stage];
  }

  std::vector<u_int>& RankingCounter() { return rankingCounter; }

  const StageID GetStageID() const { return stage; }
  void SetStageID(const StageID id) { stage = id; }

private:
  StageID stage;
  TextureList gimmickID;

  std::vector<u_int> gimmickCounter;
  std::vector<u_int> rankingCounter;

  GameData() : stage(StageID::None) {
    gimmickCounter.resize(All_Stage);
    for (auto& count : gimmickCounter) { count = 0; }

    rankingCounter.resize(All_Stage);
    for (auto& count : rankingCounter) { count = 0; }
  }
};

}
