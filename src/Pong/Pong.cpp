#include <print.h>
#include <constants.h>

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
  Scene* scene = new Scene("GAMEPLAY SCENE", r);

  Entity* background = scene->createEntity("bg", 0, 0);
  auto& bg = background->addComponent<BackgroundComponent>(
    "Sprites/Backgrounds/Summer/Summer_Background.png",
    1024, 576,
    0,
    4,
    1500
  );
  bg.lastUpdate = SDL_GetTicks();

  addSetupSystem<PhysicsSetupSystem>(scene, renderer);
  addSetupSystem<CharacterSetupSystem>(scene);
  addSetupSystem<BackgroundSetupSystem>(scene, renderer);

  addUpdateSystem<PhysicsUpdateSystem>(scene);
  addUpdateSystem<MovementUpdateSystem>(scene);
  addEventSystem<MovementInputSystem>(scene);

  addRenderSystem<BackgroundRenderSystem>(scene);
  addUpdateSystem<BackgroundUpdateSystem>(scene);

  addSetupSystem<TilemapSetupSystem>(scene, renderer);
  addRenderSystem<TilemapRenderSystem>(scene);

  addSetupSystem<SpriteSetupSystem>(scene, renderer);
  addRenderSystem<SpriteRenderSystem>(scene);
  addUpdateSystem<SpriteUpdateSystem>(scene);
  addRenderSystem<FixtureRenderSystem>(scene);


  return scene;
}

