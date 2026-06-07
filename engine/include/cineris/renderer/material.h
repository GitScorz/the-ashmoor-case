#pragma once
#include <cineris/renderer/shader.h>
#include <cineris/renderer/texture.h>

namespace cineris::renderer {
	class Material {
	public:
		Shader* shader = nullptr;
		Texture* albedo = nullptr;
		glm::vec3 color{ 1.0f, 1.0f, 1.0f };

		float shininess = 32.0f;

		auto bind() const -> void;
	};
}