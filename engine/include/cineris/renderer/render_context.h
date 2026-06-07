#pragma once
#include <cineris/include.h>
#include <vector>

struct FogSettings {
  bool bEnabled;
  glm::vec3 v_Color;
  float fDensity;
  float fStart;
  float fEnd;
};

struct RenderContext {
  static constexpr int MAX_LIGHTS = 8;

  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 viewPos;
  std::vector<glm::vec3> lightPositions;
  glm::vec3 lightColor;
  FogSettings fog;
};
