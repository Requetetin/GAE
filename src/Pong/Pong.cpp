#include <print.h>
#include "Pong.h"
#include "Systems.h"
#include "Components.h"

#include "ECS/Entity.h"

Pong::Pong() : Game("Pong", SCREEN_WIDTH, SCREEN_HEIGHT) {
  Scene* gameplayScene = createGameplayScene();
  setScene(gameplayScene);
}

Pong::~Pong() {
    // destructor implementation
}

Scene* Pong::createGameplayScene()
{
  Scene* scene = new Scene("GAMEPLAY SCENE");

  Entity white = scene->createEntity("cat1", 3*16, 9*16);
  auto& s = white.addComponent<SpriteComponent>(
    "Sprites/MainChar/SpriteSheet.png",
    0, 0,
    16,
    5,
    1000
  );
  s.lastUpdate = SDL_GetTicks();

  Entity background = scene->createEntity("bg", 0, 0);
  auto& bg = background.addComponent<BackgroundComponent>(
    "Sprites/Backgrounds/Summer/Summer_Background.png",
    1024, 576,
    0,
    4,
    1500
  );
  bg.lastUpdate = SDL_GetTicks();

  scene->addSetupSystem<PhysicsSetupSystem>(renderer);

  scene->addSetupSystem<BackgroundSetupSystem>(renderer);
  scene->addRenderSystem<BackgroundRenderSystem>();
  scene->addUpdateSystem<BackgroundUpdateSystem>();

  scene->addSetupSystem<TilemapSetupSystem>(renderer);
  scene->addRenderSystem<TilemapRenderSystem>();

  scene->addSetupSystem<SpriteSetupSystem>(renderer);
  scene->addRenderSystem<SpriteRenderSystem>();
  scene->addUpdateSystem<SpriteUpdateSystem>();


  return scene;
}

