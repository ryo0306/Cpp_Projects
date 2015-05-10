
#pragma once
#include "../common.hpp"


class cEnvironment {

  cEnvironment() = default;

public:

  static AppEnv* get();
};

typedef cEnvironment  env;
