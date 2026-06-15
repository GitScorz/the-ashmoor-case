#include <cineris/core/scene.h>

namespace cineris {
	auto Scene::addObject(const GameObject& obj) -> void {
		m_Objects.push_back(obj);
	}

	auto Scene::addLight(const Light& light) -> void {
		m_Lights.push_back(light);
	}

	auto Scene::getObjects() const -> const std::vector<GameObject>& {
		return m_Objects;
	}

	auto Scene::getLights() const -> const std::vector<Light>& {
		return m_Lights;
	}

	auto Scene::clear() -> void {
		m_Objects.clear();
		m_Lights.clear();
	}
}