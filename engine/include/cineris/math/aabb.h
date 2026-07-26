#pragma once
#include <cineris/core/include.h>
#include <vector>
#include <iostream>

namespace cineris::math {

	struct AABB {
		glm::vec3 min;
		glm::vec3 max;

		[[nodiscard]]
		auto intersects(const AABB& other) const -> bool {
			return
				min.x <= other.max.x && max.x >= other.min.x &&
				min.y <= other.max.y && max.y >= other.min.y &&
				min.z <= other.max.z && max.z >= other.min.z;
		};
	};

	[[nodiscard]]
	auto calculateLocalBounds(
		const std::vector<float>& vertices
	) -> AABB;

	[[nodiscard]]
	auto transformAABB(
		const AABB& localBounds,
		const glm::mat4& model
	) -> AABB;

}