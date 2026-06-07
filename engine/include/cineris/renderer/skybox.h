#pragma once
#include <cineris/renderer/shader.h>
#include <array>

namespace cineris::renderer {
	class Skybox {
	public:
		Skybox() = default;
		~Skybox();

		Skybox(const Skybox&) = delete;
		auto operator=(const Skybox&) -> Skybox & = delete;

		Skybox(Skybox&& other) noexcept;
		auto operator=(Skybox&& other) noexcept -> Skybox &;

		auto load(const std::array<std::string, 6>& faces, Shader* shader) -> void;
		auto draw(const glm::mat4& view, const glm::mat4& projection) -> void;
	private:
		auto loadCubemap(const std::array<std::string, 6>& faces) -> unsigned int;
		auto createCube() -> void;
	private:
		unsigned int m_textureID = 0;
		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;

		Shader* m_pShader = nullptr;

	};

}