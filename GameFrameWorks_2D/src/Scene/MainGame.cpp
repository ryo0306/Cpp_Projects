
#include "MainGame.h"

using namespace frameworks::scene;
using namespace frameworks::utility;


// 初期化をする
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result) {

  // ギミック画像のテーブル
  const std::string TextureTable[] = {
    "res/png/hoge.png",
  };

  // テーブルからデータを登録、関連付けされた ID を保持
  auto& gimmickID = GameData::Get().GimmickID();
  gimmickID.clear();
  for (auto& data : TextureTable) {
    gimmickID.push_back(Asset().Append().Texture(data));
  }

  // BGM, SE のテーブル
  const std::string MediaTable[] = {
    "res/wav/stage.wav",
    "res/wav/switch.wav",
    "res/wav/goal.wav",
    "res/wav/fall.wav",
    "res/wav/crash.wav",
  };

  // テーブルからデータを登録、関連付けされた ID を保持
  for (auto& data : MediaTable) {
    mediaID.push_back(Asset().Append().Media(data));
  }

  // 音量を一括で調整
  const float gain = 0.5f;
  for (auto& id : mediaID) {
    Asset().Find().Media(id)->gain(gain);
  }

  // BGM のみ、ループ再生を許可
  auto bgm = Asset().Find().Media(mediaID[0]);
  bgm->looping(true);
  bgm->play();

  switch (GameData::Get().GetStageID()) {
    default:
    case StageID::Stage1: player.Start(Vec2f(0, 0)); break;
    case StageID::Stage2: player.Start(Vec2f(0, 0)); break;
    case StageID::Stage3: player.Start(Vec2f(0, 0)); break;
  }

  player.CollisionSetup(stage.GetTransforms());
}


// 更新
void MainGame::Update() {
  player.Update();

  const auto& playerPos = player.GetTransform().pos;
  const auto& playerSize = player.GetTransform().scale;

  for (auto& gimmick : gimmicks) {
    const auto& gimmickPos = gimmick.GetTransform().pos;
    const auto& gimmickSize = gimmick.GetTransform().scale;

    const auto hit = IsHitRectToRect(playerPos, playerSize,
                                     gimmickPos, gimmickSize);
    if (!hit) { continue; }

    if (player.IsKeyActive()) {
      player.SetGravityDirection(gimmick.GetDirection());
      player.GravityReset();
      ++GameData::Get().GimmickCounter();
      Asset().Find().Media(mediaID[3])->play();
    }
  }
}


// 描画
void MainGame::Draw() {
  stage.Draw();
  for (auto& gimmick : gimmicks) { gimmick.Draw(); }
  player.Draw();
}
