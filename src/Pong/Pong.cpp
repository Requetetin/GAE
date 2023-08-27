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

  Entity black = scene->createEntity("cat2", 20, 0);
  black.addComponent<SpriteComponent>(
    "Sprites/MainChar/SpriteSheet.png", 
    0, 0,
    16,
    5,
    1000,
    PixelShader{
      [](Uint32 color) -> Uint32 { return (color == 0xF3F2C0FF) ? 0xD2B48CFF : color ; },
      "red"
    },
    SDL_GetTicks()
  );

  scene->addSetupSystem<SpriteSetupSystem>(renderer);
  scene->addRenderSystem<SpriteRenderSystem>();
  scene->addUpdateSystem<SpriteUpdateSystem>();

  return scene;
}

