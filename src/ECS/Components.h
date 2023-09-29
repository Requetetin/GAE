#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Game/Graphics/PixelShader.h"
#include "Game/Graphics/Texture.h"

struct NameComponent {
  std::string name;
};

struct TransformComponent {
  glm::ivec2 position;
  glm::vec2 scale;
  double rotation;
};

struct SpeedComponent {
  glm::vec2 velocity;
};

struct SpriteComponent {
  std::string name;
  int xIndex = 0;
  int yIndex = 0;
  int size = 16;
  int animationFrames = 0;
  int animationDuration = 0;
  PixelShader shader = { nullptr, "" };
  
  Uint32 lastUpdate = 0;
};

struct TilemapComponent {
  int width;
  int height;
  int tileSize;
};

struct TileComponent {
  Texture* texture;
  int index;
};

struct BackgroundComponent {
  std::string name;
  int width = 1024;
  int height = 576;
  int xIndex = 0;
  int animationFrames = 0;
  int animationDuration = 0;
  Uint32 lastUpdate = 0;
};