
#include "Stage.h"

using namespace frameworks::object;


void Stage::Setup() {
  const auto& stageID = GameData::Get().GetStageID();

  const std::string GoalTable[] = {
    "res/png/newton1.png",
    "res/png/newton2.png",
  };

  for (auto& data : GoalTable) {
    goalID.push_back(Asset().Append().Texture(data));
  }
  drawID = 0;

  // 画像のテーブル
  std::vector<std::string> BlockTable;

  switch (stageID) {
    default:;
    case StageID::Stage1:

      // 昼の足場
      BlockTable = {
        "res/png/ashiba_A.png",      // [0]ブロックの上を歩ける
        "res/png/ashiba_B.png",      // [1]左
        "res/png/ashiba_C.png",      // [2]下
        "res/png/ashiba_D.png",      // [3]右
        "res/png/ashiba_E.png",      // [4]なし：表示専用
        "res/png/ashiba_F.png",      // [5]左右
        "res/png/ashiba_G.png",      // [6]上下
        "res/png/ashiba_H.png",      // [7]下だけ無し
        "res/png/ashiba_I.png",      // [8]右だけ無し
        "res/png/ashiba_J.png",      // [9]上だけ無し
        "res/png/ashiba_K.png",      // [10]左だけ無し
        "res/png/ashiba_kA.png",     // [11]上と右
        "res/png/ashiba_kB.png",     // [12]右と下
        "res/png/ashiba_kC.png",     // [13]下と左
        "res/png/ashiba_kD.png",     // [14]左と上
      };
      break;

    case StageID::Stage2:

      // 夕方の足場
      BlockTable = {
        "res/png/ashiba_yuA.png",    // [0]ブロックの上を歩ける
        "res/png/ashiba_yuB.png",    // [1]左
        "res/png/ashiba_yuC.png",    // [2]下
        "res/png/ashiba_yuD.png",    // [3]右
        "res/png/ashiba_yuE.png",    // [4]なし：表示専用
        "res/png/ashiba_yuF.png",    // [5]左右
        "res/png/ashiba_yuG.png",    // [6]上下
        "res/png/ashiba_yuH.png",    // [7]下だけ無し
        "res/png/ashiba_yuI.png",    // [8]右だけ無し
        "res/png/ashiba_yuJ.png",    // [9]上だけ無し
        "res/png/ashiba_yuK.png",    // [10]左だけ無し
        "res/png/ashiba_yukA.png",   // [11]上と右
        "res/png/ashiba_yukB.png",   // [12]右と下
        "res/png/ashiba_yukC.png",   // [13]下と左
        "res/png/ashiba_yukD.png",   // [14]左と上
      };
      break;

    case StageID::Stage3:

      // 夜の足場
      BlockTable = {
        "res/png/ashiba_yA.png",     // [0]ブロックの上を歩ける
        "res/png/ashiba_yB.png",     // [1]左
        "res/png/ashiba_yC.png",     // [2]下
        "res/png/ashiba_yD.png",     // [3]右
        "res/png/ashiba_yE.png",     // [4]なし：表示専用
        "res/png/ashiba_yF.png",     // [5]左右
        "res/png/ashiba_yG.png",     // [6]上下
        "res/png/ashiba_yH.png",     // [7]下だけ無し
        "res/png/ashiba_yI.png",     // [8]右だけ無し
        "res/png/ashiba_yJ.png",     // [9]上だけ無し
        "res/png/ashiba_yK.png",     // [10]左だけ無し
        "res/png/ashiba_ykA.png",    // [11]上と右
        "res/png/ashiba_ykB.png",    // [12]右と下
        "res/png/ashiba_ykC.png",    // [13]下と左
        "res/png/ashiba_ykD.png",    // [14]左と上
      };
      break;
  }

  // テーブルからデータを登録、関連付けされた ID を保持
  for (auto& data : BlockTable) {
    blockID.push_back(Asset().Append().Texture(data));
  }

  // ステージのスケール
  std::shared_ptr<const float> Scale;

  // ステージの初期化
  switch (stageID) {
    default: // *Debug Mode Only* Initialized Stage Data.

    case StageID::Stage1:
      Scale = std::make_shared<const float>(80.0f);

      // 左
      blocks.push_back({ Vec2f(-6, 2.5f), Vec2f::Ones(), blockID[11] });
      blocks.push_back({ Vec2f(-6, 1.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, 0.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, -0.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, -1.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, -2.5f), Vec2f::Ones(), blockID[4] });
      blocks.push_back({ Vec2f(-5, -2.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-4, -2.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-3, -2.5f), Vec2f::Ones(), blockID[11] });
      blocks.push_back({ Vec2f(-3, -3.5f), Vec2f::Ones(), blockID[4] });
      blocks.push_back({ Vec2f(-2, -3.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-1, -3.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(0, -3.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(1, -3.5f), Vec2f::Ones(), blockID[11] });

      // 右
      blocks.push_back({ Vec2f(4, -3.5f), Vec2f::Ones(), blockID[13] });
      blocks.push_back({ Vec2f(4, -2.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, -1.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, -0.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, 0.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, 1.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, 2.5f), Vec2f::Ones(), blockID[4] });
      blocks.push_back({ Vec2f(3, 2.5f), Vec2f::Ones(), blockID[2] });
      blocks.push_back({ Vec2f(2, 2.5f), Vec2f::Ones(), blockID[2] });
      blocks.push_back({ Vec2f(1, 2.5f), Vec2f::Ones(), blockID[2] });
      blocks.push_back({ Vec2f(0, 2.5f), Vec2f::Ones(), blockID[13] });
      break;

    case StageID::Stage2:
      Scale = std::make_shared<const float>(50.0f);

      //　　左
      blocks.push_back({ Vec2f(-7.5f, -0.5f), Vec2f::Ones(), blockID[7] });
      blocks.push_back({ Vec2f(-7.5f, -1.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(-7.5f, -2.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(-7.5f, -3.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(-7.5f, -4.5f), Vec2f::Ones(), blockID[13] });
      blocks.push_back({ Vec2f(-6.5f, -4.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(-5.5f, -4.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(-4.5f, -4.5f), Vec2f::Ones(), blockID[11] });
      blocks.push_back({ Vec2f(-4.5f, -5.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(-4.5f, -6.5f), Vec2f::Ones(), blockID[9] });


      //　　真ん中上
      blocks.push_back({ Vec2f(-4.5f, 3.5f), Vec2f::Ones(), blockID[8] });
      blocks.push_back({ Vec2f(-3.5f, 3.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(-2.5f, 3.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(-1.5f, 3.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(-0.5f, 3.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(0.5f, 3.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(1.5f, 3.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(2.5f, 3.5f), Vec2f::Ones(), blockID[12] });
      blocks.push_back({ Vec2f(2.5f, 4.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(2.5f, 5.5f), Vec2f::Ones(), blockID[7] });

      //　　真ん中
      blocks.push_back({ Vec2f(0.5f, -0.5f), Vec2f::Ones(), blockID[7] });
      blocks.push_back({ Vec2f(0.5f, -1.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(0.5f, -2.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(0.5f, -3.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(0.5f, -4.5f), Vec2f::Ones(), blockID[9] });

      //　　右
      blocks.push_back({ Vec2f(7.5f, 4.5f), Vec2f::Ones(), blockID[7] });
      blocks.push_back({ Vec2f(7.5f, 3.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(7.5f, 2.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(7.5f, 1.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(7.5f, 0.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(7.5f, -0.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(7.5f, -1.5f), Vec2f::Ones(), blockID[12] });
      blocks.push_back({ Vec2f(6.5f, -1.5f), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(5.5f, -1.5f), Vec2f::Ones(), blockID[14] });
      blocks.push_back({ Vec2f(5.5f, -2.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(5.5f, -3.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(5.5f, -4.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(5.5f, -5.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(5.5f, -6.5f), Vec2f::Ones(), blockID[9] });
      break;

    case StageID::Stage3:
      Scale = std::make_shared<const float>(60.0f);

      //　　左上
      blocks.push_back({ Vec2f(-7, 3), Vec2f::Ones(), blockID[7] });
      blocks.push_back({ Vec2f(-7, 2), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(-7, 1), Vec2f::Ones(), blockID[12] });
      blocks.push_back({ Vec2f(-8, 1), Vec2f::Ones(), blockID[8] });

      //　　左下
      blocks.push_back({ Vec2f(-5, -3), Vec2f::Ones(), blockID[9] });
      blocks.push_back({ Vec2f(-5, -2), Vec2f::Ones(), blockID[14] });
      blocks.push_back({ Vec2f(-4, -2), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(-3, -2), Vec2f::Ones(), blockID[10] });


      //　　真ん中
      blocks.push_back({ Vec2f(0, -1), Vec2f::Ones(), blockID[8] });
      blocks.push_back({ Vec2f(1, -1), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(2, -1), Vec2f::Ones(), blockID[10] });

      //　　右上
      blocks.push_back({ Vec2f(3, 3), Vec2f::Ones(), blockID[8] });
      blocks.push_back({ Vec2f(4, 3), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(5, 3), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(6, 3), Vec2f::Ones(), blockID[12] });
      blocks.push_back({ Vec2f(6, 4), Vec2f::Ones(), blockID[7] });

      //　　右下
      blocks.push_back({ Vec2f(3, -5), Vec2f::Ones(), blockID[8] });
      blocks.push_back({ Vec2f(4, -5), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(5, -5), Vec2f::Ones(), blockID[6] });
      blocks.push_back({ Vec2f(6, -5), Vec2f::Ones(), blockID[10] });
      break;
  }

  // スケールをステージに反映
  for (auto& block : blocks) {
    block.pos *= (*Scale);
    block.size *= (*Scale);
  }

  // 背景画像のテーブル
  const std::string BackTable[] = {
    "res/png/back_stage1.png",
    "res/png/back_stage2.png",
    "res/png/back_stage3.png",
  };

  for (auto& data : BackTable) {
    backID.push_back(Asset().Append().Texture(data));
  }
}


void Stage::Update() {
  // do not something.
}


void Stage::Draw() {
  const float Size = 108.0f;
  for (const auto& block : blocks) {
    const auto texture = Asset().Find().Texture(block.id);
    drawTextureBox(block.pos.x(), block.pos.y(),
                   block.size.x(), block.size.y(),
                   0, 0, Size, Size, *texture);
  }
}


void Stage::GoalSetup(const Block& goal,
                      const int direction) {
  this->goal = goal;
  this->direction = direction;
}


void Stage::Goal() {
  drawID = 1;
}


void Stage::GoalDraw() {
  const float Size = 512.0f;
  const auto texture = Asset().Find().Texture(goalID[drawID]);
  const auto pos = goal.pos + goal.size * 0.5f;
  drawTextureBox(pos.x(), pos.y(), goal.size.x(), goal.size.y(),
                 0, 0, Size, Size, *texture, Color::white,
                 direction * (M_PI / 2),
                 Vec2f::Ones(), goal.size * 0.5f);
}


void Stage::BackDraw() {
  const auto stageID = GameData::Get().GetStageID();
  const auto texture = Asset().Find().Texture(backID[stageID]);
  const Vec2f window(WIDTH, HEIGHT);

  drawTextureBox(-window.x() / 2, -window.y() / 2, window.x(), window.y(),
                 0, 0, window.x(), window.y(), *texture);
}
