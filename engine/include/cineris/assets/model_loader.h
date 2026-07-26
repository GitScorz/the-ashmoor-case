#pragma once

#include <cineris/renderer/model.h>

#include <memory>
#include <vector>

struct aiMesh;

namespace cineris::assets {
	class ModelLoader {
	public:
		static auto loadMesh(const aiMesh* mesh) -> std::unique_ptr<cineris::renderer::Mesh>;
		static auto load(const std::string& path) -> std::unique_ptr<cineris::renderer::Model>;
	};
}