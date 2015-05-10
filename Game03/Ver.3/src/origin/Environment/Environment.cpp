
#include "Environment.h"


enum WindowSize {
  WIDTH  = 1024,
  HEIGHT = 768,
};


AppEnv& env::get() {
  static AppEnv s_env(WindowSize::WIDTH,
                      WindowSize::HEIGHT,
                      false, true);
  return s_env;
}


const Vec2f& env::getWindowSize() {
  static const Vec2f s_size(WindowSize::WIDTH,
                            WindowSize::HEIGHT);
  return s_size;
}
