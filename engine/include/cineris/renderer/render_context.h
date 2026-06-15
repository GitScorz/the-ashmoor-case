#pragma once
#include <cineris/core/include.h>
#include <vector>

struct FogSettings {
  bool enabled = true;
  glm::vec3 color;
  float density;
  float start;
  float end;
};

struct RenderContext {
  static constexpr int MAX_LIGHTS = 8;

  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 viewPos;
  std::vector<glm::vec3> lightPositions;
  std::vector<glm::vec3> lightColors;
  glm::vec3 lightColor;
  FogSettings fog;

  glm::vec3 ambientColor;
  float ambientStrength;
};
