
#pragma once
#include "../../lib/defines.hpp"
#include "../../lib/matrix.hpp"
#include "../../lib/texture.hpp"
#include <string>
#include <map>


// TIPS: リソース追加用の構造体
struct SpriteInfo {
  short       id;
  std::string path;
  Vec2f       size;
};

class cSprite {
  typedef std::pair<short, Texture>  sprt_info;

  cSprite() = default;

  std::map<short, Texture> sprite_;
  std::map<short, Vec2f>   size_;

public:
  static cSprite& get();

  void spriteAppend(const SpriteInfo&);
  void spriteDelete();

  const Texture& sprite(const short) const;
  const Vec2f& size(const short) const;
};

typedef cSprite  sprt;
