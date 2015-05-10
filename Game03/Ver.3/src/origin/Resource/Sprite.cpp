
#include "Sprite.h"


cSprite& sprt::get() {
  static cSprite s_sprite;
  return s_sprite;
}

void sprt::spriteAppend(const SpriteInfo& info) {
  sprite_.insert(std::make_pair(info.id, Texture(info.path)));
  size_.insert(std::make_pair(info.id, info.size));
}

void sprt::spriteDelete() {
  if (!sprite_.empty()) sprite_.clear();
}

const Texture& sprt::sprite(const short id) const {
  return sprite_.find(id)->second;
}

const Vec2f& sprt::size(const short id) const {
  return size_.find(id)->second;
}
