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

  Entity white = scene->createEntity("cat1", 0, 0);
  auto& s = white.addComponent<SpriteComponent>(
    "Sprites/MainChar/SpriteSheet.png",
    0, 0,
    16,
    5,
    1000
  );
  s.lastUpdate = SDL_GetTicks();

  scene->addSetupSystem<TilemapSetupSystem>(renderer);
  scene->addRenderSystem<TilemapRenderSystem>();

  scene->addSetupSystem<SpriteSetupSystem>(renderer);
  scene->addRenderSystem<SpriteRenderSystem>();
  scene->addUpdateSystem<SpriteUpdateSystem>();

  return scene;
}

