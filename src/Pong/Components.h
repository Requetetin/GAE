#include "ECS/Components.h"
#include "Game/Graphics/Texture.h"
#include "Game/Graphics/PixelShader.h"

struct SizeComponent
{
  int w;
  int h;
};

struct ColliderComponent
{
  bool triggered;
};

struct PlayerComponent
{
  short moveSpeed;
};
