
#include "Stage.h"

using namespace frameworks::object;


void Stage::Setup() {
  const auto& stageID = GameData::Get().GetStageID();

  const std::string GoalTable[] = {
    "res/png/nyu_ton1.png",
    "res/png/nyu_ton1.png",
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
        "res/png/ashiba_A.png",    // [0]ブロックの上を歩ける
        "res/png/ashiba_B.png",    // [1]左
        "res/png/ashiba_C.png",    // [2]下
        "res/png/ashiba_D.png",    // [3]右
        "res/png/ashiba_E.png",    // [4]なし：表示専用
        "res/png/ashiba_F.png",    // [5]左右
        "res/png/ashiba_G.png",    // [6]上下
        "res/png/ashiba_H.png",    // [7]下だけ無し
        "res/png/ashiba_I.png",    // [8]右だけ無し
        "res/png/ashiba_J.png",    // [9]上だけ無し
        "res/png/ashiba_K.png",    // [10]左だけ無し
        "res/png/ashiba_kA.png",   // [11]上と右
        "res/png/ashiba_kB.png",   // [12]右と下
        "res/png/ashiba_kC.png",   // [13]下と左
        "res/png/ashiba_kD.png",   // [14]左と上
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
        "res/png/ashiba_yA.png",    // [0]ブロックの上を歩ける
        "res/png/ashiba_yB.png",    // [1]左
        "res/png/ashiba_yC.png",    // [2]下
        "res/png/ashiba_yD.png",    // [3]右
        "res/png/ashiba_yE.png",    // [4]なし：表示専用
        "res/png/ashiba_yF.png",    // [5]左右
        "res/png/ashiba_yG.png",    // [6]上下
        "res/png/ashiba_yH.png",    // [7]下だけ無し
        "res/png/ashiba_yI.png",    // [8]右だけ無し
        "res/png/ashiba_yJ.png",    // [9]上だけ無し
        "res/png/ashiba_yK.png",    // [10]左だけ無し
        "res/png/ashiba_ykA.png",   // [11]上と右
        "res/png/ashiba_ykB.png",   // [12]右と下
        "res/png/ashiba_ykC.png",   // [13]下と左
        "res/png/ashiba_ykD.png",   // [14]左と上
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
    default:; // *Debug Mode Only* Initialized Stage Data.

    case StageID::Stage1:
      Scale = std::make_shared<const float>(80.0f);

      // 左
      blocks.push_back({ Vec2f(-6, 2.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(-6, 1.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, 0.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, -0.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, -1.5f), Vec2f::Ones(), blockID[3] });
      blocks.push_back({ Vec2f(-6, -2.5f), Vec2f::Ones(), blockID[4] });
      blocks.push_back({ Vec2f(-5, -2.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-4, -2.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-3, -2.5f), Vec2f::Ones(), blockID[5] });
      blocks.push_back({ Vec2f(-3, -3.5f), Vec2f::Ones(), blockID[4] });
      blocks.push_back({ Vec2f(-2, -3.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-1, -3.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(0, -3.5f), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(1, -3.5f), Vec2f::Ones(), blockID[5] });

      // 右
      blocks.push_back({ Vec2f(4, -3.5f), Vec2f::Ones(), blockID[7] });
      blocks.push_back({ Vec2f(4, -2.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, -1.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, -0.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, 0.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, 1.5f), Vec2f::Ones(), blockID[1] });
      blocks.push_back({ Vec2f(4, 2.5f), Vec2f::Ones(), blockID[4] });
      blocks.push_back({ Vec2f(3, 2.5f), Vec2f::Ones(), blockID[2] });
      blocks.push_back({ Vec2f(2, 2.5f), Vec2f::Ones(), blockID[2] });
      blocks.push_back({ Vec2f(1, 2.5f), Vec2f::Ones(), blockID[2] });
      blocks.push_back({ Vec2f(0, 2.5f), Vec2f::Ones(), blockID[7] });
      break;

    case StageID::Stage2:
      Scale = std::make_shared<const float>(50.0f);

      // 左下スタート地点
      blocks.push_back({ Vec2f(-600, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-550, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-500, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-450, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-350, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-300, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-600, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-550, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-500, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-450, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-400, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-350, -250), Vec2f::Ones(), blockID[0] });

      // 真ん中下
      blocks.push_back({ Vec2f(-200, -350), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-150, -350), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-100, -350), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-100, -300), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-100, -250), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-100, -200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-50, -200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(0, -200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(50, -200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(100, -200), Vec2f::Ones(), blockID[0] });

      // 右下
      blocks.push_back({ Vec2f(300, -350), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(350, -350), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(400, -350), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(450, -350), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(500, -350), Vec2f::Ones(), blockID[0] });

      // 左上
      blocks.push_back({ Vec2f(-450, 200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-400, 200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-350, 200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-300, 200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-250, 200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-200, 200), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-150, 200), Vec2f::Ones(), blockID[0] });

      // 真ん中左
      blocks.push_back({ Vec2f(-150, 50), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(-150, 0), Vec2f::Ones(), blockID[0] });

      // 真ん中右
      blocks.push_back({ Vec2f(100, -50), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(150, -50), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(200, -50), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(250, -50), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(300, -50), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(300, -0), Vec2f::Ones(), blockID[0] });

      // 真ん中上
      blocks.push_back({ Vec2f(150, 100), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(150, 150), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(200, 150), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(250, 150), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(300, 150), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(350, 150), Vec2f::Ones(), blockID[0] });

      // 左上
      blocks.push_back({ Vec2f(50, 300), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(100, 300), Vec2f::Ones(), blockID[0] });
      blocks.push_back({ Vec2f(150, 300), Vec2f::Ones(), blockID[0] });
      break;

    case StageID::Stage3:
      Scale = std::make_shared<const float>(50.0f);
      break;
  }

  // スケールをステージに反映
  for (auto& block : blocks) {
    block.pos *= (*Scale);
    block.size *= (*Scale);
  }

  // 背景画像のテーブル
  const std::string BackTable[] = {
    "res/png/test.png",
    "res/png/test.png",
    "res/png/test.png",
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
  const float Size = 256.0f;
  const auto texture = Asset().Find().Texture(goalID[drawID]);
  drawTextureBox(goal.pos.x(), goal.pos.y(),
                 goal.size.x(), goal.size.y(),
                 0, 0, Size, Size, *texture,
                 Color::white,
                 direction * (M_PI / 2),
                 Vec2f::Ones(), Vec2f::Zero());
}


void Stage::BackDraw() {
  //const auto stageID = GameData::Get().GetStageID();
  const auto stageID = 0;
  const auto texture = Asset().Find().Texture(backID[stageID]);
  const Vec2f window(WIDTH, HEIGHT);

  drawTextureBox(-window.x() / 2, -window.y() / 2, window.x(), window.y(),
                 0, 0, window.x(), window.y(), *texture);
}
