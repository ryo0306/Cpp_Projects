
#pragma once


class cAnim {
  enum SpriteID : short {
    Back_Fog = 30,
    Back_Sea_1,
    Back_Sea_2,

    PREFIX,
    All_Sprite = PREFIX - Back_Fog,
  };

  // TIPS: enum Ç≈ÇÕ float å^ÇíËêîÇ…Ç≈Ç´Ç»Ç¢
  struct AnimationPrm {
    const float FOG_SPD_;
    const float SEA_SPD_;
    const float SCROLL_LIMIT_;

    const Color FOG_COLOR_;
    const Color SEA_COLOR_;

    AnimationPrm();
  } anim_prm;

  float scroll_fog_;
  float scroll_sea_;

public:
  cAnim();

  void drawFog();
  void drawSea();
};
