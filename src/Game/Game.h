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

    void setScene(std::unique_ptr<Scene> newScene);
    Scene* getCurrentScene() const;

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

    std::unique_ptr<Scene> currentScene;
};
