
#pragma once


namespace literal_strategy {

  // 描画状態
  enum ClickState {
    MoveIn_1,
    Select_1,
    Blink_1,
    MoveOut_1,

    Wait,

    MoveIn_2,
    Select_2,
    Blink_2,
    MoveOut_2,
  };

  // ボタンの選択状態
  enum ButtonState {
    s_Funds,
    s_Develop,
    s_Armaments,
    s_Invasion,
    s_SaveLoad,

    s_Neutral,
  };

  // 画像の切り取り、描画サイズ
  enum Size {
    Sea_Fog_WH = 512,
    Map_WH = 1024,
  };

  // 描画位置
  enum DrawPos {
    BtmWindowPos_X = -WIDTH / 2 + 10,
    BtmWindowPos_Y = -HEIGHT / 2 + 10,
    BtmWindow_W = WIDTH - 20,
    BtmWindow_H = 140,

    InfoWindowPos_X = 0,
    InfoWindowPos_Y = BtmWindowPos_Y + BtmWindow_H + 10,
    InfoWindow_W = (WIDTH / 2) - 10,
    InfoWindow_H = HEIGHT - (BtmWindow_H + 30),

    StringPos_Y = 15,
    ModePos_Y = HEIGHT / 2 - (button::Small_H + 10),
    CommandPos_X = BtmWindowPos_X,
    CommandPos_Y = ModePos_Y - (button::Small_H / 2) * 3,
    Command_H = button::Small_H + 10,
    DefaultCommandPos = -300,
    MoveStartPos = -200,

    DescriptPos_X = BtmWindowPos_X + 20,
    DescriptPos_Y = BtmWindowPos_Y + 84,
    DescriptLine_H = 60,

    DataPos_X = 24,
    DataPos_Y = (HEIGHT / 2) - 50,
    Data_W = 320,
    Data_H = 36,
    WeaponCount_X = 40,

    UnitInfoPos_X = DataPos_X + 150,
    UnitInfoPos_Y = DataPos_Y - 100,
  };

  // 描画するパラメータ情報
  enum Information {
    i_Money = 0,
    i_Food,
    i_Battle,
    i_Lose,

    All_CountInfo,

    i_Fire = 0,
    i_Bomb,
    i_Trap,
    i_Poison,

    All_WeaponInfo,
  };

  // システム固有
  enum System {
    ScrollEndPoint = Sea_Fog_WH,

    PriceLength = 5,
  };

  // 兵器の価格
  enum WeaponPrice {
    p_Fire = 1000,
    p_Bomb = 3000,
    p_Trap = 3000,
    p_Poison = 2000,
  };

  // 画像の番号
  enum TextureNumber {
    North_1,
    North_2,
    North_3,
    North_4,
    Center_1,
    Center_2,
    Center_3,
    Center_4,
    South_1,
    South_2,
    South_3,
    South_4,
  };

  // 準備画面の文字配列用データ
  enum StrategyStringData {
    Funds,
    Develop,
    Armaments,
    Invasion,
    SaveLoad,

    All_Command,

    Save = 0,
    Load,
    Back,

    All_Select,

    t_Fire = 0,
    t_Bomb,
    t_Trap,
    t_Poison,

    All_Icon,
  };

  // 資金繰りコマンドの文字配列用データ
  enum FundsStringData {
    Worker,
    Sell_Fire,
    Sell_Bomb,
    Sell_Trap,
    Sell_Poison,
    f_Back,

    All_Funds,
    f_Neutral = -1,
  };

  // コマンド解説テキストのデータ
  struct sDescriptionText {
    std::string first_;    // １行目
    std::string second_;   // ２行目
    std::string unable_;   // コマンド使えない
  };
}
