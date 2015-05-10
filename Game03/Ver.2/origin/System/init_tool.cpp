
#include "init_tool.h"
#include "data_depot.h"
#include "draw_gadget.h"
#include <ctime>


void init::randomSeed() {
  data::rand.setSeed(u_int(time(nullptr)));
}


void init::defaultScene(const scene& start) {
  data::system.scene_[play::Next] = start;
  data::system.scene_[play::Last] = scene::None;
  data::system.scene_[play::Now]  = scene::None;

  data::system.pause_ = false;
  data::system.game_clear_ = false;
}


void init::resourceData() {
  draw::colorDataInit();
  data::media.mediaDataInit();
}
