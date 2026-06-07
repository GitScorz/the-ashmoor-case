#pragma once
#include <cineris/include.h>
#include <string>

namespace cineris::renderer {

	class Texture {
	public:
		Texture(const std::string& filepath);
		~Texture();

		auto bind(unsigned int slot = 0) const -> void;
		auto unbind() const -> void;

		auto getId() const -> unsigned int { return m_RendererID; }

	private:
		unsigned int m_RendererID = 0;
	};

}