#pragma once

#include <iostream>
#include <constants.h>
#include "ECS/System.h"

class RectRenderSystem : public RenderSystem {
  public:
    void run(SDL_Renderer* renderer) override;
};

class PlayerInputEventSystem : public EventSystem {
  public:
    void run(SDL_Event event);
};

class CollisionDetectionUpdateSystem : public UpdateSystem {
  public:
    void run(double dT);
};

class BounceUpdateSystem : public UpdateSystem {
  public:
    void run(double dT);
};

class SpriteSetupSystem : public SetupSystem {
  public:
    SpriteSetupSystem(SDL_Renderer* renderer);
    ~SpriteSetupSystem();

    void run() override;

  private:
    SDL_Renderer* renderer;
};

class SpriteUpdateSystem : public UpdateSystem {
  public:
    void run(double dT) override;
};

class SpriteRenderSystem : public RenderSystem {
  public:
    void run(SDL_Renderer* renderer) override;
};

class TilemapSetupSystem : public SetupSystem {
  public:
    TilemapSetupSystem(SDL_Renderer* renderer);
    ~TilemapSetupSystem();
    void run() override;

  private:
    SDL_Renderer* renderer;
};

class TilemapRenderSystem : public RenderSystem {
  public:
    void run(SDL_Renderer* renderer) override;
};

class BackgroundSetupSystem : public SetupSystem {
  public:
    BackgroundSetupSystem(SDL_Renderer* renderer);
    ~BackgroundSetupSystem();

    void run() override;

  private:
    SDL_Renderer* renderer;
};

class BackgroundUpdateSystem : public UpdateSystem {
  public:
    void run(double dT) override;
};

class BackgroundRenderSystem : public RenderSystem {
  public:
    void run(SDL_Renderer* renderer) override;
};

class PhysicsSetupSystem : public SetupSystem {
  public:
    PhysicsSetupSystem(SDL_Renderer* renderer);
    ~PhysicsSetupSystem();

    void run() override;

  private:
    SDL_Renderer* renderer;
};

class PhysicsUpdateSystem : public UpdateSystem {
  public:
    void run(double dT) override;
};

class MovementUpdateSystem : public UpdateSystem {
public:
    void run(double dT) override;
};

class FixtureRenderSystem : public RenderSystem {
  public:
    void run(SDL_Renderer* renderer) override;
};

class MovementInputSystem : public EventSystem {
  public:
    void run(SDL_Event event);

private:
  float hForceMagnitude = 1000.0f * PIXELS_PER_METER;
  float vForceMagnitude = 10000.0f * PIXELS_PER_METER;

  void moveCharacter(int direction);

  void stopCharacter(int direction);

  void jumpCharacter();
};

class CharacterSetupSystem : public SetupSystem {
public:
  void run();
};