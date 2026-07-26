#include <cineris/assets/model_loader.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <cineris/core/logger.h>

namespace cineris::assets {
	auto ModelLoader::loadMesh(const aiMesh* mesh) -> std::unique_ptr<cineris::renderer::Mesh> {
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(mesh->mNumVertices * 8); // 3 for position, 3 for normal, 2 for texture coordinates

		for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			const aiVector3D& position = mesh->mVertices[vertexIndex];
			const aiVector3D& normal = mesh->HasNormals() ? mesh->mNormals[vertexIndex] : aiVector3D(0.0f, 0.0f, 0.0f);
			const aiVector3D& texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][vertexIndex] : aiVector3D(0.0f, 0.0f, 0.0f);
			vertices.push_back(position.x);
			vertices.push_back(position.y);
			vertices.push_back(position.z);
			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);
			vertices.push_back(texCoord.x);
			vertices.push_back(texCoord.y);
		}

		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			const aiFace& face = mesh->mFaces[faceIndex];
			for (unsigned int index = 0; index < face.mNumIndices; ++index) {
				indices.push_back(face.mIndices[index]);
			}
		}

		return std::make_unique<cineris::renderer::Mesh>(std::move(vertices), std::move(indices));
	}

	auto ModelLoader::load(const std::string& path) -> std::unique_ptr<cineris::renderer::Model> {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(
			path,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ImproveCacheLocality |
			aiProcess_PreTransformVertices
		);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LOG_ERROR(log::LogChannel::Renderer, "Failed to load model {}: {}", path, importer.GetErrorString());
			return nullptr;
		}

		auto model = std::make_unique<cineris::renderer::Model>();

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			auto loadedMesh = loadMesh(mesh);
			if (loadedMesh) {
				model->addMesh(std::move(loadedMesh));
			}
		}

		return model;
	}
}