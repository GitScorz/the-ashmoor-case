#pragma once
#include <cineris/core/include.h>
#include <cineris/core/window.h>
#include "freetype.h"
#include <iostream>
#include <unordered_map>
#include <cineris/renderer/shader.h>

namespace cineris::renderer {

	struct Character {
		unsigned int textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	class TextRenderer {
	public:

		TextRenderer(Window* m_pWindow);
		~TextRenderer();

		auto loadFont(const std::string& filepath, unsigned int pixelSize, Shader* shader) -> void;
		auto renderText(const std::string& text, float x, float y, float scale, glm::vec4 color) -> void;

		auto updateProjection() -> void;
		auto setShader(Shader* shader) -> void;
	private:
		Window* m_pWindow = nullptr;
		Shader* m_pShader = nullptr;

		FT_Library m_FreeType = nullptr;
		FT_Face m_Face = nullptr;

		std::unordered_map<char, Character> m_Characters;

		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;
	};

}