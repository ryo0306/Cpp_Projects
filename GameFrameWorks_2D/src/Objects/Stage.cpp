
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

      // 画像 ID の取り出し方
      // textureID[] に、上記テーブル表に対応した番号を入れると ID が入っています

      // textureID[0] で、上を歩けるブロックを取り出せる
      // 左を歩けるブロックは、textureID[1] で取り出せます
      // こんな感じで使いたい画像の ID を紐づけしてください
      blocks.push_back({ Vec2f(0, 0), Vec2f(100, 100), textureID[0] });

      break;

    case StageID::Stage2:
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
