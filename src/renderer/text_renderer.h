#pragma once
#include "freetype.h"
#include "include.h"
#include <iostream>
#include <unordered_map>
#include "shader.h"
#include "core/window.h"

struct Character {
  unsigned int textureID;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

class TextRenderer {
public:
  Shader m_Shader;

  TextRenderer(Window* m_pWindow);
  ~TextRenderer();

  auto loadFont(const std::string& filepath, unsigned int& pixelSize) -> void;
  auto renderText(const std::string& text, float x, float y, float scale, glm::vec3 color) -> void;
private:
  Window* m_pWindow = nullptr;
  FT_Library m_FreeType = nullptr;
  FT_Face m_Face = nullptr;

  std::unordered_map<char, Character> m_Characters;

  unsigned int m_VAO = 0;
  unsigned int m_VBO = 0;
};