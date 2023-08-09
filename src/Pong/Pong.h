#pragma once
#include "Game/Game.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

class Pong : public Game {
  public:
    Pong();
    ~Pong();

  private:
    std::unique_ptr<Scene> createGameplayScene();
};
