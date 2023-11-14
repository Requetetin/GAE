#pragma once
#include "Game/Game.h"

class Pong : public Game {
  public:
    Pong();
    ~Pong();

  private:
    Uint32 collisionEvent;
    Scene* createGameplayScene();
};
