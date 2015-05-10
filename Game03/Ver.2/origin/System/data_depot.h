
#pragma once
#include "base.hpp"
#include "../../lib/random.hpp"
#include "user_data.hpp"
#include "system_data.hpp"
#include "../Resource/media.hpp"


class cDataDepot {
  // TIPS: ‚±‚ÌƒNƒ‰ƒX‚ÍéŒ¾‚µ‚È‚¢
  cDataDepot() = delete;

public:
  static Random         rand;
  static cUserData      user;
  static cSystemData    system;
  static cMediaResource media;
};

typedef cDataDepot  data;
