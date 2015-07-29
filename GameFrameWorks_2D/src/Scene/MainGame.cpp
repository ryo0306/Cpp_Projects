
#include "MainGame.h"

using namespace frameworks::object;
using namespace frameworks::scene;
using namespace frameworks::utility;


// 初期化をする
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result) {
  Asset().Delete().All();
  player.Setup();
  stage.Setup();

  const auto& stageID = GameData::Get().GetStageID();

  // ギミック画像のテーブル
  const std::string TextureTable[] = {
    "res/png/button_A.png",     // [0]赤スイッチ
    "res/png/button_D.png",     // [1]青スイッチ
    "res/png/button_B.png",     // [2]黄色スイッチ
    "res/png/button_C.png",     // [3]緑スイッチ
    "res/png/button_A_B.png",   // [4]赤押した
    "res/png/button_D_B.png",   // [5]青押した
    "res/png/button_B_B.png",   // [6]黄色押した
    "res/png/button_C_B.png",   // [7]緑押した
  };

  // テーブルからデータを登録、関連付けされた ID を保持
  auto& gimmickID = GameData::Get().GimmickID();
  gimmickID.clear();
  for (auto& data : TextureTable) {
    gimmickID.push_back(Asset().Append().Texture(data));
  }

  // ギミックサイズ
  std::shared_ptr<const float> gimmickSize;

  // ギミック設置
  switch (stageID) {
  default:;
    case StageID::Stage1:
      gimmickSize = std::make_shared<const float>(80.0f);


      gimmicks.push_back(StageGimmick(GravityDirection::Top,
                        Vec2f(-5, 2.5f), Vec2f::Ones() * 0.8f));
      gimmicks.push_back(StageGimmick(GravityDirection::Bottom,
        Vec2f(-5, -0.5f), Vec2f::Ones() * 0.8f));
      gimmicks.push_back(StageGimmick(GravityDirection::Right,
                   Vec2f(0, -2.5f), Vec2f::Ones() * 0.8f));
      gimmicks.push_back(StageGimmick(GravityDirection::Top,
                    Vec2f(3, -3.5f), Vec2f::Ones() * 0.8f));
      gimmicks.push_back(StageGimmick(GravityDirection::Left,
        Vec2f(0, 1.5f), Vec2f::Ones() * 0.8f));
      break;
    
    case StageID::Stage2:
      gimmickSize = std::make_shared<const float>(50.0f);

      gimmicks.push_back(StageGimmick(GravityDirection::Right,
        Vec2f(-6.5, -0.5f), Vec2f::Ones() * 1));
      gimmicks.push_back(StageGimmick(GravityDirection::Top,
        Vec2f(-4.5, -3.5f), Vec2f::Ones() * 1));
      gimmicks.push_back(StageGimmick(GravityDirection::Bottom,
        Vec2f(-1.5, 2.5f), Vec2f::Ones() * 1));
      gimmicks.push_back(StageGimmick(GravityDirection::Bottom,
        Vec2f(-0.5, -2.5f), Vec2f::Ones() * 1));
      gimmicks.push_back(StageGimmick(GravityDirection::Right,
        Vec2f(3.5, 4.5f), Vec2f::Ones() * 1));
      gimmicks.push_back(StageGimmick(GravityDirection::Top,
        Vec2f(1.5, -2.5f), Vec2f::Ones() * 1));
      gimmicks.push_back(StageGimmick(GravityDirection::Left,
        Vec2f(6.5, -0.5f), Vec2f::Ones() * 1));
      break;

    case StageID::Stage3:
      gimmickSize = std::make_shared<const float>(50.0f);
      break;
  }

  // ギミックサイズを反映
  for (auto& gimmick : gimmicks) {
    gimmick.GetTransform().pos *= (*gimmickSize);
    gimmick.GetTransform().scale *= (*gimmickSize);
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

  // プレイヤーとゴールの座標を設定
  float playerScale;
  float goalScale;
  switch (stageID) {
  default:;
    case StageID::Stage1:
      playerScale = 50.0f;
      goalScale = 150.0f;

      player.Start(Vec2f(-2, -2.5) * playerScale, playerScale);
      stage.GoalSetup({ Vec2f(-2.0, -1) * goalScale, Vec2f::Ones() * goalScale, 0 },
                      GravityDirection::Bottom);
      break;
   
   
    case StageID::Stage2:
      playerScale = 50.0f;
      goalScale = 80.0f;

      player.Start(Vec2f(-5.5,-3.5) * playerScale, playerScale);
      stage.GoalSetup({ Vec2f(3.75, -2.5) * goalScale, Vec2f::Ones() * goalScale * 1.5, 0 },
                      GravityDirection::Right);
      break;

    case StageID::Stage3:
      playerScale = 50.0f;
      goalScale = 80.0f;

      player.Start(Vec2f(0, 0) * playerScale, playerScale);
      stage.GoalSetup({ Vec2f(0, 0) * goalScale, Vec2f::Ones() * goalScale, 0 },
                      GravityDirection::Top);
      break;
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

  const auto goal = stage.GetGoalPos();
  if (IsHitRectToRect(playerPos, playerSize, goal.pos, goal.size)) {
    isFinish = true;
    Asset().Find().Media(mediaID[0])->stop();
  }
}


// 描画
void MainGame::Draw() {
  stage.BackDraw();
  stage.Draw();
  stage.GoalDraw();
  for (auto& gimmick : gimmicks) { gimmick.Draw(); }
  player.Draw();
}
