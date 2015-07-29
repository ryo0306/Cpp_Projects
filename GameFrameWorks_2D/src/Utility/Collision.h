
#pragma once
#include "../Common.hpp"


namespace frameworks {
namespace utility {

/*
// マウスカーソルと四角
bool OnMouseRect(const Vec2f& mouse_pos,
                 const Vec2f& pos,
                 const Vec2f& size);
*/

/*
// 点と円（真円）
bool IsHitPointToCircle(const Vec2f& point,
                        const Vec2f& pos,
                        const float radius);
*/

// 四角と四角
bool IsHitRectToRect(const Vec2f&, const Vec2f&,
                     const Vec2f&, const Vec2f&);

}  // end utility
}  // end frameworks
