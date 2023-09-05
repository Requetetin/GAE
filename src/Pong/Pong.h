#pragma once
#include "Game/Game.h"

// 64 * 36 tiles
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 576;

class Pong : public Game {
  public:
    Pong();
    ~Pong();

  private:
    Scene* createGameplayScene();
};
