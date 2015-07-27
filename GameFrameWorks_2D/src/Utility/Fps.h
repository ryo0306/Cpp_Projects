
#pragma once


namespace frameworks {
namespace utility {

// フレームカウンタの更新
void FrameUpdate();

// １フレームあたりの更新にかかった時間を取得
// 精度  1 / ミリ秒
const double GetElapsedFrame();

}  // end utility
}  // end frameworks
