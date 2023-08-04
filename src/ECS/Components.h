#pragma once
#include <glm/glm.hpp>
#include <string>

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