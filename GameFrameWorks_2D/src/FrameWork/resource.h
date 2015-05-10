
#pragma once
#include <string>
#include <map>
#include "../common.hpp"


namespace resource {

  enum SpriteID {
    All_Sprite,
  };

  enum AudioID {
    All_Audio,
  };
}


struct SpriteInfo {
  resource::SpriteID id;
  std::string file_pass;
};


struct AudioInfo {
  resource::AudioID id;
  std::string file_pass;
};


class cResource {

  std::map<resource::SpriteID, Texture> m_sprite;
  std::map<resource::AudioID, Media>    m_audio;

  resource::AudioID m_current_audio;

  cResource();

public:

  static cResource* get();

  void deleteSprite();
  void deleteAudio();

  void spriteAppend(const SpriteInfo& table);
  Texture* sprite(const resource::SpriteID id);

  void audioAppend(const AudioInfo& table);
  void audioPlay(const resource::AudioID id);
  void audioStop();
};

typedef cResource  res;
