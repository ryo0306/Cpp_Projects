
#pragma once
#include "../Common.hpp"
#include <map>


namespace frameworks {

// ユニークな ID を返す
// TIPS: 呼び出すたびに ID が更新される
u_int GetUniqueID();

class AssetManager;

namespace resource {

class Resource {
  class AssetBase {
  public:
    AssetBase(Resource& res) : res(res) {}
  protected:
    Resource& res;
  };

  class AssetAppend : protected AssetBase {
  public:
    AssetAppend(Resource& res) : AssetBase(res) {}

    // TIPS: 登録に使った ID を返す
    u_int Texture(const std::string& path) {
      u_int id = GetUniqueID();
      res.texture.emplace(std::make_pair(id, ::Texture(path)));
      return id;
    }

    // TIPS: 登録に使った ID を返す
    u_int Media(const std::string& path) {
      u_int id = GetUniqueID();
      res.media.emplace(std::make_pair(id, ::Media(path)));
      return id;
    }
  };

  class AssetRemove : protected AssetBase {
  public:
    AssetRemove(Resource& res) : AssetBase(res) {}

    void Texture(const u_int id) {
      auto itr = res.texture.find(id);
      if (itr != res.texture.end()) res.texture.erase(itr);
    }

    void Media(const u_int id) {
      auto itr = res.media.find(id);
      if (itr != res.media.end()) res.media.erase(itr);
    }
  };

  class AssetDelete : protected AssetBase {
  public:
    AssetDelete(Resource& res) : AssetBase(res) {}
    void Texture() { res.texture.clear(); }
    void Media() { res.media.clear(); }
    void All() { Texture(); Media(); }
  };

  class AssetFinder : protected AssetBase {
  public:
    AssetFinder(Resource& res) : AssetBase(res) {}

    ::Texture* Texture(const u_int id) const {
      auto itr = res.texture.find(id);
      return (itr != res.texture.end()) ? &itr->second : nullptr;
    }

    ::Media* Media(const u_int id) const {
      auto itr = res.media.find(id);
      return (itr != res.media.end()) ? &itr->second : nullptr;
    }
  };

public:
  AssetAppend& Append() { return append; }
  AssetRemove& Remove() { return remove; }
  AssetDelete& Delete() { return destroy; }
  AssetFinder& Find() { return finder; }

private:
  std::map<u_int, ::Texture> texture;
  std::map<u_int, ::Media>   media;

  AssetAppend append;
  AssetRemove remove;
  AssetDelete destroy;
  AssetFinder finder;

  friend class AssetManager;

  Resource() :
    append(*this), remove(*this),
    destroy(*this), finder(*this) {}
};

}  // end resource

// リソース管理システムにアクセスする
resource::Resource& Asset();

}  // end frameworks
