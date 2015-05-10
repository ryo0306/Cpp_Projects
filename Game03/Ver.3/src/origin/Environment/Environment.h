
#pragma once
#include "../../lib/defines.hpp"
#include "../../lib/appEnv.hpp"


class cEnvironment {
  cEnvironment() = default;

public:
  static AppEnv& get();
  static const Vec2f& getWindowSize();
};

typedef cEnvironment  env;
