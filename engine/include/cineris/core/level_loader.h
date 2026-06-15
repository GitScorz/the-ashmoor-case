#pragma once
#include "scene.h"

namespace cineris {
	class LevelLoader {
	public:
		static auto load(const std::string& path, const std::string& levelName, Scene& scene) -> bool;

	};
}