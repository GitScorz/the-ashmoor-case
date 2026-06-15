#pragma once
#include <string>
#include <glm/glm.hpp>

namespace cineris {
	enum class LightType {
		Directional,
		Point,
		Spot
	};

	struct Light {
		std::string name;
		LightType type = LightType::Directional;

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 color = glm::vec3(1.0f);

		float intensity = 1.0f;
		float radius = 10.0f;
	};
}