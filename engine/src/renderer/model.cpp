#include <cineris/renderer/model.h>

namespace cineris::renderer {
	auto Model::addMesh(std::unique_ptr<Mesh> mesh) -> void {
		const auto& bounds = mesh->getLocalBounds();
		
		if (!m_bHasBounds) {
			m_LocalBounds = bounds;
			m_bHasBounds = true;
		}
		else {
			m_LocalBounds.min = glm::min(m_LocalBounds.min, bounds.min);
			m_LocalBounds.max = glm::max(m_LocalBounds.max, bounds.max);
		}

		m_Meshes.push_back(std::move(mesh));
	}

	auto Model::draw() const -> void {
		for (const auto& mesh : m_Meshes) {
			mesh->draw();
		}
	}
}