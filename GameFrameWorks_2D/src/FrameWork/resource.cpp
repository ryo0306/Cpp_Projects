
#include "resource.h"


cResource::cResource() {
}


cResource* res::get() {
  static cResource s_res;
  return &s_res;
}


void res::deleteSprite() {
  m_sprite.clear();
}


void res::deleteAudio() {
  m_audio.clear();
}


void res::spriteAppend(const SpriteInfo& table) {
  m_sprite.insert(std::make_pair(table.id, Texture(table.file_pass)));
}


Texture* res::sprite(const resource::SpriteID id) {
  auto& it = m_sprite.find(id);
  return &it->second;
}


void res::audioAppend(const AudioInfo& table) {
  m_audio.insert(std::make_pair(table.id, Media(table.file_pass)));
}


void res::audioPlay(const resource::AudioID id) {
  m_current_audio = id;
  auto& it = m_audio.find(id);
  it->second.play();
}


void res::audioStop() {
  auto& it = m_audio.find(m_current_audio);
  it->second.stop();
}
