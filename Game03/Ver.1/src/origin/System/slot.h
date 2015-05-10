
#pragma once
#include "data_depot.h"


namespace Slot {
  bool saveUserData(const short&);
  bool loadUserData(const short&);
  void getSlotData(cUser&, const short&);
}
