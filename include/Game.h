#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <gl/GL.h>
#include <string>

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

  private:
    int width;
    int height;
    bool isRunning;

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

    int scoreLeft;
    int scoreRight;
};
