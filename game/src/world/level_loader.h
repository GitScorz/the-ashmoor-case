#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace ashmoor {

	class LevelLoader {
	public:
		static auto load(const std::string& fileName) -> std::vector<std::string>;

	};
}