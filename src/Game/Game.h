#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <memory>
#include "Scene/Scene.h"

class Game {
  public:
    // Constructor
    Game(const char* name, int width, int height);
    // Destructor
    ~Game();

    void setup();
    void frameStart();
    void frameEnd();
    void handleEvents();

    void update();
    void render();

    bool running();

    void clean();

    void run();

    void setScene(Scene* newScene);
    Scene* getCurrentScene() const;

    entt::registry r;

  protected:
    bool isRunning;
    int screen_width;
    int screen_height;

    SDL_Window* window;
    SDL_Renderer* renderer;

    // frame management
    Uint32 frameStartTimeStamp;
    Uint32 frameEndTimestamp;
    float frameDuration;
    float deltaTime;
    int frameCount;
    Uint32 lastFPSUpdateTime;
    float FPS;

    Scene* currentScene;

    template<typename T>
    void addSetupSystem(Scene* scene, auto&&... args) {
        auto system = std::make_shared<T>(std::forward<decltype(args)>(args)...);
        system->setScene(scene);
        scene->setupSystems.push_back(system);
    }

    template<typename T>
    void addEventSystem(Scene* scene, auto&&... args) {
        auto system = std::make_shared<T>(std::forward<decltype(args)>(args)...);
        system->setScene(scene);
        scene->eventSystems.push_back(system);
    }

    template<typename T>
    void addUpdateSystem(Scene* scene, auto&&... args) {
        auto system = std::make_shared<T>(std::forward<decltype(args)>(args)...);
        system->setScene(scene);
        scene->updateSystems.push_back(system);
    }

    template<typename T>
    void addRenderSystem(Scene* scene, auto&&... args) {
        auto system = std::make_shared<T>(std::forward<decltype(args)>(args)...);
        system->setScene(scene);
        scene->renderSystems.push_back(system);
    }
};
