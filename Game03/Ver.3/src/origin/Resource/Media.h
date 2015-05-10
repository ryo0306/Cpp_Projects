
#pragma once
#include "../../lib/defines.hpp"
#include "../../lib/audio.hpp"
#include <string>
#include <map>


// TIPS: ���\�[�X�̊Ǘ��ԍ�
enum MediaID : short {
  MUSIC_ID = 0,
  SE_ID    = 50,
};

// TIPS: ���\�[�X�ǉ��p�̍\����
struct MediaInfo {
  short       id;
  std::string path;
};

class cMedia {
  typedef std::pair<short, Media>  md_info;

  cMedia() = default;

  std::map<short, Media> music_;
  std::map<short, Media> se_;

  short playing_number_;

public:
  static cMedia& get();

  void mediaAppend(const MediaInfo&);
  void mediaDelete();

  void mediaGainTuning();

  void musicPlay(const short);
  void musicStop();

  void sePlay(const short);
};

typedef cMedia  md;
