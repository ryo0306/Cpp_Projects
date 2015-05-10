
#pragma once
#include "draw_window.h"
#include "system.hpp"
#include "user.hpp"
#include "../Resource/music.hpp"
#include "../../lib/random.hpp"
#include <ctime>
#include <fstream>

//
// データ管理クラス
//

class cDataManager {
  cDataManager() = delete;

public:
  static cSystem system;
  static cUser   user;
  static Random  random;

  static cMusicResource music;

  static void bugProof();
  static void sceneStart(const state::SceneMode&);
  static void newGame(const short&);
  static void reStart(const short&);

  static void enemySpown();

  static short getTerritory();
};

typedef cDataManager  data;
