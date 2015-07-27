
#pragma once
#include "../Common.hpp"


namespace frameworks {
namespace utility {

// 原点の一覧
enum class Origin {
  LeftTop,
  Left,
  LeftBottom,
  CenterTop,
  Center,
  CenterBottom,
  RightTop,
  Right,
  RightBottom,

  All_Info,
};

// 画像の原点を指定、移動量を返す
const Vec2f& OffsetTextureOrigin(const Origin info);

// 画像の描画：機能追加版
void DrawTexture(const Transform& transform,
                 const Sprite& sprite,
                 const Vec2f& offset = Vec2f::Zero());

}  // end utility
}  // end frameworks
