
#include "Media.h"


cMedia& md::get() {
  static cMedia s_md;
  return s_md;
}

void md::mediaAppend(const MediaInfo& info) {
  md_info source
    = std::make_pair(info.id, Media(info.path));

  info.id < SE_ID ?
    music_.insert(source) : se_.insert(source);
}

void md::mediaDelete() {
  if (!music_.empty()) music_.clear();
  if (!se_.empty())    se_.clear();
}

void md::mediaGainTuning() {
  const float VOLUME = 0.2f;

  for (unsigned short index = MUSIC_ID;
       index < unsigned(music_.size() + MUSIC_ID);
       ++index) {
    auto& it = music_.find(index)->second;
    it.looping(true);
    it.gain(VOLUME);
  }

  for (unsigned short index = SE_ID;
       index < unsigned(se_.size() + SE_ID);
       ++index) {
    auto& it = se_.find(index)->second;
    it.looping(true);
    it.gain(VOLUME);
  }
}

void md::musicPlay(const short id) {
  playing_number_ = id;
  music_.find(id)->second.play();
}

void md::musicStop() {
  music_.find(playing_number_)->second.stop();
}

void md::sePlay(const short id) {
  se_.find(id)->second.play();
}
