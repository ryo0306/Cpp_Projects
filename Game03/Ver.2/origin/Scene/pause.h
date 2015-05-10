
#pragma once
#include "../System/environment.h"
#include "../System/draw_gadget.h"
#include "../System/data_depot.h"
#include "../Literal/literal_font.hpp"


class cPause {
public:
  cPause();

  bool update();
  void draw();
};
