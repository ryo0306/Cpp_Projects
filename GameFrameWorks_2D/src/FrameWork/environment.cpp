
#include "environment.h"


AppEnv* env::get() {
  static AppEnv s_env(WIDTH, HEIGHT, false, true);
  return &s_env;
}
