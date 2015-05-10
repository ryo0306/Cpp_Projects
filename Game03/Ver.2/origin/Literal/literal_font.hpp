
#pragma once
#include "../System/base.hpp"
#include "../../lib/font.hpp"
#include <string>
#include <sstream>
#include <iomanip>


#define FONT_STRING  "res/font/arare.ttf"
#define FONT_NUMBER  "res/font/MeiryoConsolas.ttf"

namespace literal_font {

  // フォントサイズの一覧
  enum FontSize {
    Standard = 40,
    Large = 60,
    Info = 24,
    Battle = 20,
  };

  // ゲーム中のボタン描画サイズ
  enum ButtonSize {
    Small_W = 200,
    Small_H = 60,

    Medium_W = 250,
    Medium_H = Small_H,

    Large_W = 400,
    Large_H = 80,

    Pause_W = 350,
    Pause_H = Small_H,
  };
}

typedef literal_font::FontSize    font;
typedef literal_font::ButtonSize  button;
typedef std::stringstream         sstream;
