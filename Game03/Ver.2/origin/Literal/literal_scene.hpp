
#pragma once


namespace literal_scene {

  // •`‰æƒV[ƒ“
  enum SceneState {
    None,
    Exit,
    Save,
    Load,

    Title,
    Strategy,
    Battle,
    Result,
  };

  // •`‰æ’iŠK
  enum PlayingScene {
    Now,
    Next,
    Last,

    All_State,
  };
}

typedef literal_scene::SceneState    scene;
typedef literal_scene::PlayingScene  play;
