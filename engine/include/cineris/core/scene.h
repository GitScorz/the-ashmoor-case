#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "game_object.h"
#include "light.h"

namespace cineris {
	class Scene {
	public:
		auto addObject(const GameObject& obj) -> void;
		auto addLight(const Light& light) -> void;

		auto getObjects() const -> const std::vector<GameObject>&;
		auto getLights() const -> const std::vector<Light>&;

		auto clear() -> void;
	private:
		std::vector<GameObject> m_Objects;
		std::vector<Light> m_Lights;
	};
}