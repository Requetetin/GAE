#include "print.h"
#include "Pong.h"
#include "Systems.h"
#include "Components.h"
#include "ECS/Entity.h"

Pong::Pong() : Game("Pong", SCREEN_WIDTH, SCREEN_HEIGHT) {
    std::unique_ptr<Scene> gameplayScene = createGameplayScene();
    setScene(std::move(gameplayScene));
}

Pong::~Pong() {
    // destructor implementation
}

std::unique_ptr<Scene> Pong::createGameplayScene()
{
    // Create a unique_ptr to hold the created scene
    std::unique_ptr<Scene> gameplayScene = std::make_unique<Scene>("Gameplay");

    Entity ball = gameplayScene->createEntity("ball", 100, 100);
    ball.addComponent<SpeedComponent>(glm::vec2(200, 200));
    ball.addComponent<SizeComponent>(30, 30);
    ball.addComponent<ColliderComponent>(false);

    Entity paddle = gameplayScene->createEntity("paddle", (width/2)-50, height-20);
    paddle.addComponent<SpeedComponent>(glm::vec2(0, 0));
    paddle.addComponent<SizeComponent>(100, 20);
    paddle.addComponent<PlayerComponent>(200);

    gameplayScene->addRenderSystem<RectRenderSystem>();
    gameplayScene->addUpdateSystem<MovementUpdateSystem>(SCREEN_WIDTH, SCREEN_HEIGHT);
    gameplayScene->addEventSystem<PlayerInputEventSystem>();
    gameplayScene->addUpdateSystem<CollisionDetectionUpdateSystem>();
    gameplayScene->addUpdateSystem<BounceUpdateSystem>();

    return gameplayScene;
}
