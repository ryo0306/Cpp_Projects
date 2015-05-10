
#pragma once
#include "../System/data_depot.h"

//
// ƒ}ƒbƒvî•ñŠÇ—
//

class cMapData {
  Texture back_ground_;
  Texture map[3];

  u_short map_id_;

public:
  cMapData();

  void mapInit(const u_short&);
  void draw(const float&);
};
