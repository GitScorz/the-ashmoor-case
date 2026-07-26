#pragma once
#include <cineris/core/include.h>
#include <cineris/math/aabb.h>
#include <vector>

namespace cineris::renderer {

	/*struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 texCoords;
	};*/

	//struct Texture {
	//	unsigned int id;
	//	std::string type;
	//};

	class Mesh {
	public:
		// Vertex Buffer Object, Vertex Array Object and Element Buffer Object
		unsigned int VBO, VAO, EBO;
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		//std::vector<Texture> textures;

		Mesh(std::vector<float> verts, std::vector<unsigned int> inds);
		~Mesh();
		auto draw() -> void;

		[[nodiscard]]
		auto getLocalBounds() const -> const cineris::math::AABB& {
			return m_localBounds;
		}

		static auto createCube() -> Mesh*;
		static auto createQuad(float fWidth, float fHeight) -> Mesh*;
		static auto createGrid(float fWidth, float fDepth, int iRows, int iCols) -> Mesh*;
	private:
		cineris::math::AABB m_localBounds{};

	};

}