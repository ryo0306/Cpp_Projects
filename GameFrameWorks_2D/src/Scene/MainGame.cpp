
#include "MainGame.h"

using namespace frameworks::scene;
using namespace frameworks::utility;


// ������������
MainGame::MainGame() :
SceneBase(SceneName::Main, SceneName::Result) {

  // �M�~�b�N�摜�̃e�[�u��
  const std::string TextureTable[] = {
    "res/png/hoge.png",
  };

  // �e�[�u������f�[�^��o�^�A�֘A�t�����ꂽ ID ��ێ�
  auto& gimmickID = GameData::Get().GimmickID();
  gimmickID.clear();
  for (auto& data : TextureTable) {
    gimmickID.push_back(Asset().Append().Texture(data));
  }

  // BGM, SE �̃e�[�u��
  const std::string MediaTable[] = {
    "res/wav/stage.wav",
    "res/wav/switch.wav",
    "res/wav/goal.wav",
    "res/wav/fall.wav",
    "res/wav/crash.wav",
  };

  // �e�[�u������f�[�^��o�^�A�֘A�t�����ꂽ ID ��ێ�
  for (auto& data : MediaTable) {
    mediaID.push_back(Asset().Append().Media(data));
  }

  // ���ʂ��ꊇ�Œ���
  const float gain = 0.5f;
  for (auto& id : mediaID) {
    Asset().Find().Media(id)->gain(gain);
  }

  // BGM �̂݁A���[�v�Đ�������
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


// �X�V
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


// �`��
void MainGame::Draw() {
  stage.Draw();
  for (auto& gimmick : gimmicks) { gimmick.Draw(); }
  player.Draw();
}
