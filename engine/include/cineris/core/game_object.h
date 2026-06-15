#pragma once
#include <string>
#include <glm/glm.hpp>

namespace cineris {
	enum class GameObjectType {
		Cube,
		Plane,
		Model,
		Player // just for now
	};

	struct Transform {
		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 rotation = glm::vec3(0.f);
		glm::vec3 scale = glm::vec3(1.f);

		glm::vec2 size = glm::vec2(0.f); // plane
	};

	struct GameObject {
		std::string name;
		GameObjectType type = GameObjectType::Cube;

		Transform transform;

		std::string meshId;
		std::string textureId;
		std::string modelPath;

		bool visible = true;
	};
}