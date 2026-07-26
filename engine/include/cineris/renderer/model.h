#pragma once

#include <cineris/renderer/mesh.h>
#include <cineris/math/aabb.h>

#include <memory>
#include <vector>

namespace cineris::renderer {
	class Model {
	public:
		auto addMesh(std::unique_ptr<Mesh> mesh) -> void;
		auto draw() const -> void;

		auto getLocalBounds() const -> const math::AABB& {
			return m_LocalBounds;
		}
	private:
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		math::AABB m_LocalBounds{};
		bool m_bHasBounds = false;
	};
}