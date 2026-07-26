#include <cineris/math/aabb.h>

#include <limits>

namespace cineris::math {
	auto calculateLocalBounds(const std::vector<float>& vertices) -> AABB {
		glm::vec3 minimum(std::numeric_limits<float>::max());
		glm::vec3 maximum(std::numeric_limits<float>::lowest());

		constexpr std::size_t stride = 8;
		for (std::size_t i = 0; i < vertices.size(); i += stride) {
			const glm::vec3 pos{
				vertices[i],
				vertices[i + 1],
				vertices[i + 2]
			};

			minimum = glm::min(minimum, pos);
			maximum = glm::max(maximum, pos);
		}

		return { minimum, maximum };
	}

	auto transformAABB(const AABB& localBounds, const glm::mat4& model) -> AABB {
		const glm::vec3& min = localBounds.min;
		const glm::vec3& max = localBounds.max;

		const glm::vec3 corners[8] = {
			{min.x, min.y, min.z},
			{max.x, min.y, min.z},
			{min.x, max.y, min.z},
			{max.x, max.y, min.z},
			{min.x, min.y, max.z},
			{max.x, min.y, max.z},
			{min.x, max.y, max.z},
			{max.x, max.y, max.z}
		};

		glm::vec3 worldMin(std::numeric_limits<float>::max());
		glm::vec3 worldMax(std::numeric_limits<float>::lowest());

		for (const glm::vec3& corner : corners) {
			const glm::vec3 transformed = glm::vec3(model * glm::vec4(corner, 1.f));

			worldMin = glm::min(worldMin, transformed);
			worldMax = glm::max(worldMax, transformed);
		}

		return { worldMin, worldMax };
	}
}