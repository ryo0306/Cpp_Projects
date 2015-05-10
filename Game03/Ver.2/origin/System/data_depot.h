
#pragma once
#include "base.hpp"
#include "../../lib/random.hpp"
#include "user_data.hpp"
#include "system_data.hpp"
#include "../Resource/media.hpp"


class cDataDepot {
  // TIPS: ���̃N���X�͐錾���Ȃ�
  cDataDepot() = delete;

public:
  static Random         rand;
  static cUserData      user;
  static cSystemData    system;
  static cMediaResource media;
};

typedef cDataDepot  data;
