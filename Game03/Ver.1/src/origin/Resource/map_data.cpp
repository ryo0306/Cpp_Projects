
#include "map_data.h"

typedef cMapData  mapdata;


cMapData::cMapData() :
back_ground_(Texture("res/png/back_grass.png")) {
  map[0] = Texture("res/png/stage_grass.png");
  map[1] = Texture("res/png/stage_forest.png");
  map[2] = Texture("res/png/stage_mountain.png");
}


// マップ情報の初期化
// TIPS: 侵攻時、受け取った番号に対応したマップ情報を展開
void mapdata::mapInit(const u_short& id) {
  map_id_ = id;
}


// TIPS: スクロール用のデータを引数として渡す
void mapdata::draw(const float& camera_x) {

  // TIPS: 背景描画
  {
    drawTextureBox(-size::Half_W, size::HEIGHT / 4, size::WIDTH, size::HEIGHT / 4,
      camera_x * 0.25f, 0, 512, 128,
      back_ground_, win::color(paint::White));

    drawTextureBox(-size::Half_W, 0, size::WIDTH, size::HEIGHT / 4,
      camera_x * 0.5f, 128, 512, 128,
      back_ground_, win::color(paint::White));
  }

  drawTextureBox(-size::Half_W, -size::Half_H, size::WIDTH, size::HEIGHT,
    camera_x, 0, size::WIDTH, size::HEIGHT,
    map[map_id_], win::color(paint::White));
}
