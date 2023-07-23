#include <print.h>
#include "Game.h"

SDL_Rect ball;
SDL_Rect left;
SDL_Rect right;

Game::Game(const char* name, int width, int height)
  : width(width), height(height) {
  int maxFPS = 60;
  frameDuration = (1.0f / maxFPS) * 1000.0f;

  frameCount = 0;
  lastFPSUpdateTime = 0;
  FPS = 0;
  scoreLeft = 0;
  scoreRight = 0;

  SDL_Init(SDL_INIT_EVERYTHING);

  window = SDL_CreateWindow(name, 0, 0, width, height, 0);
  renderer = SDL_CreateRenderer(window, 0, 0);

  SDL_SetRenderDrawColor(renderer, 200, 255, 255, 1);
  isRunning = true;
};

Game::~Game() {
  print("Destructor\n");

  SDL_DestroyWindow(window);
};

void Game::setup() {
  ball.x = (width / 2);
  ball.y = (height / 2);
  ball.w = 15;
  ball.h = 15;

  left.h = 250;
  left.w = 20;
  left.y = (height / 2) - (left.h / 2);
  left.x = 0;

  right.h = 250;
  right.w = 20;
  right.y = (height / 2) - (right.h / 2);
  right.x = width - 20;
};
void Game::frameStart() {
  // system("cls");
  frameStartTimeStamp = SDL_GetTicks();
  deltaTime = frameEndTimestamp - frameStartTimeStamp;
};
void Game::frameEnd() {
  frameEndTimestamp = SDL_GetTicks();

  float actualFrameDuration = frameEndTimestamp - frameStartTimeStamp;

  if (actualFrameDuration < frameDuration)
  {
    SDL_Delay(frameDuration - actualFrameDuration);
  }
  
  frameCount++;
  // Update FPS counter every second
  Uint32 currentTime = SDL_GetTicks();
  if (currentTime - lastFPSUpdateTime > 1000) // 1000 milliseconds in 1 second
  {
    FPS = frameCount / ((currentTime - lastFPSUpdateTime) / 1000.0f);
    lastFPSUpdateTime = currentTime;
    frameCount = 0;
  }

};
void Game::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      isRunning = false;
    }

    if (event.type == SDL_KEYDOWN) {
      switch(event.key.keysym.sym) {
        case SDLK_UP:
          if (right.y - 10 >= 0)
            right.y -= 10;
          else 
            right.y = 0;
          break;
        case SDLK_DOWN:
          if (right.y + 10 + right.h <= height)
            right.y += 10;
          else
            right.y = height - right.h;
          break;
        case SDLK_w:
          if (left.y - 10 >= 0)
            left.y -= 10;
          else
            left.y = 0;
          break;
        case SDLK_s:
          if (left.y + 10 + left.h <= height)
            left.y += 10;
          else
            left.y = height - left.h;
          break;
      }
    }
  }
};

int sx = 2;
int sy = 2;

void Game::update() {
  if (ball.x <= 20) {
    if (ball.y >= left.y && (ball.y + ball.h) <= left.y + left.h) {
      sx *= -1.1;
    } else {
      scoreRight++;
      ball.x = (width / 2);
      ball.y = (height / 2);
      sy = 2;
      sx = -2;
    }
  }
  if (ball.x + ball.w >= width - 20) {
    if (ball.y >= right.y && (ball.y + ball.h) <= right.y + right.h) {
      sx *= -1.1;
    } else {
      scoreLeft++;
      ball.x = (width / 2);
      ball.y = (height / 2);
      sy = 2;
      sx = 2;
    }
  }
  if (ball.y <= 0) {
    sy *= -1;
  }
  if (ball.y + ball.h >= height) {
    sy *= -1;
  }

  ball.x += sx;
  ball.y += sy;
};
void Game::render() {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
  SDL_RenderFillRect(renderer, &ball);
  SDL_RenderFillRect(renderer, &left);
  SDL_RenderFillRect(renderer, &right);

  SDL_RenderPresent(renderer);

  print("FPS: ", FPS);
  print("Score: ", scoreLeft, " - ", scoreRight);
};

bool Game::running() {
  return isRunning;
};

void Game::run() {
  setup();
  
  while(isRunning) {
    frameStart();

    handleEvents();

    update();
    render();

    frameEnd();
  }
}

void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}