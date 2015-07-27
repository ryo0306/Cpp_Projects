
#include "Resource.h"


namespace frameworks {

class AssetManager {
public:
  static AssetManager& Get() {
    static AssetManager assetManager;
    return assetManager;
  }

  resource::Resource& Resource() { return res; }

private:
  resource::Resource res;

  AssetManager() = default;
};

resource::Resource& Asset() { return AssetManager::Get().Resource(); }

u_int GetUniqueID() {
  static u_int uniqueID = 0;
  return uniqueID++;
}

void AssetManagerSetup() { AssetManager::Get(); }
void AssetRelease() { Asset().Delete().All(); }

}  // end frameworks
