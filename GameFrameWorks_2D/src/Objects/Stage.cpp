
#include "Stage.h"

using namespace frameworks::object;


Stage::Stage() {
  blocks.clear();
  textureID.clear();

  // 画像のテーブル
  // NOTICE: どんな画像かは、リソースフォルダを確認してください
  const std::string TextureTable[] = {
    "res/png/ashiba_A.png",    // [0]ブロックの上を歩ける
    "res/png/ashiba_B.png",    // [1]左
    "res/png/ashiba_C.png",    // [2]下
    "res/png/ashiba_D.png",    // [3]右
    "res/png/ashiba_E.png",    // [4]なし：表示専用
    "res/png/ashiba_kA.png",   // [5]上と右
    "res/png/ashiba_kB.png",   // [6]右と下
    "res/png/ashiba_kC.png",   // [7]下と左
    "res/png/ashiba_kD.png",   // [8]左と上
  };

  // テーブルからデータを登録、関連付けされた ID を保持
  for (auto& data : TextureTable) {
    textureID.push_back(Asset().Append().Texture(data));
  }

  // ステージの初期化
  switch (GameData::Get().GetStageID()) {
    default: // *Debug Mode Only* Initialized Stage Data.

    case StageID::Stage1:
      // 左
      blocks.push_back({ Vec2f(-600, 250), Vec2f(100, 100), textureID[5] });
      blocks.push_back({ Vec2f(-600, 150), Vec2f(100, 100), textureID[3] });
      blocks.push_back({ Vec2f(-600, 50), Vec2f(100, 100), textureID[3] });
      blocks.push_back({ Vec2f(-600, -50), Vec2f(100, 100), textureID[3] });
      blocks.push_back({ Vec2f(-600, -150), Vec2f(100, 100), textureID[3] });
      blocks.push_back({ Vec2f(-600, -250), Vec2f(100, 100), textureID[4] });
      blocks.push_back({ Vec2f(-500, -250), Vec2f(100, 100), textureID[0] });
      blocks.push_back({ Vec2f(-400, -250), Vec2f(100, 100), textureID[0] });
      blocks.push_back({ Vec2f(-300, -250), Vec2f(100, 100), textureID[5] });
      blocks.push_back({ Vec2f(-300, -350), Vec2f(100, 100), textureID[4] });
      blocks.push_back({ Vec2f(-200, -350), Vec2f(100, 100), textureID[0] });
      blocks.push_back({ Vec2f(-100, -350), Vec2f(100, 100), textureID[0] });
      blocks.push_back({ Vec2f(0, -350), Vec2f(100, 100), textureID[0] });
      blocks.push_back({ Vec2f(100, -350), Vec2f(100, 100), textureID[5] });

      // 右
      blocks.push_back({ Vec2f(400, -350), Vec2f(100, 100), textureID[7] });
      blocks.push_back({ Vec2f(400, -250), Vec2f(100, 100), textureID[1] });
      blocks.push_back({ Vec2f(400, -150), Vec2f(100, 100), textureID[1] });
      blocks.push_back({ Vec2f(400, -50), Vec2f(100, 100), textureID[1] });
      blocks.push_back({ Vec2f(400, 50), Vec2f(100, 100), textureID[1] });
      blocks.push_back({ Vec2f(400, 150), Vec2f(100, 100), textureID[1] });
      blocks.push_back({ Vec2f(400, 250), Vec2f(100, 100), textureID[4] });
      blocks.push_back({ Vec2f(300, 250), Vec2f(100, 100), textureID[2] });
      blocks.push_back({ Vec2f(200, 250), Vec2f(100, 100), textureID[2] });
      blocks.push_back({ Vec2f(100, 250), Vec2f(100, 100), textureID[2] });
      blocks.push_back({ Vec2f(0, 250), Vec2f(100, 100), textureID[7] });
      break;

    case StageID::Stage2:
      // 左下スタート地点
      blocks.push_back({ Vec2f(-600, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-550, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-500, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-450, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-350, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-300, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-600, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-550, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-500, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-450, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-400, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-350, -250), Vec2f(50, 50), textureID[0] });

      // 真ん中下
      blocks.push_back({ Vec2f(-200, -350), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-150, -350), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-100, -350), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-100, -300), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-100, -250), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-100, -200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-50, -200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(0, -200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(50, -200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(100, -200), Vec2f(50, 50), textureID[0] });

      // 右下
      blocks.push_back({ Vec2f(300, -350), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(350, -350), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(400, -350), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(450, -350), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(500, -350), Vec2f(50, 50), textureID[0] });

      // 左上
      blocks.push_back({ Vec2f(-450, 200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-400, 200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-350, 200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-300, 200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-250, 200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-200, 200), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-150, 200), Vec2f(50, 50), textureID[0] });

      // 真ん中左
      blocks.push_back({ Vec2f(-150, 50), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(-150, 0), Vec2f(50, 50), textureID[0] });

      // 真ん中右
      blocks.push_back({ Vec2f(100, -50), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(150, -50), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(200, -50), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(250, -50), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(300, -50), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(300, -0), Vec2f(50, 50), textureID[0] });

      // 真ん中上
      blocks.push_back({ Vec2f(150, 100), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(150, 150), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(200, 150), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(250, 150), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(300, 150), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(350, 150), Vec2f(50, 50), textureID[0] });

      // 左上
      blocks.push_back({ Vec2f(50, 300), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(100, 300), Vec2f(50, 50), textureID[0] });
      blocks.push_back({ Vec2f(150, 300), Vec2f(50, 50), textureID[0] });
      break;

    case StageID::Stage3:
      break;
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
