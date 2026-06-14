#pragma once
#include <cineris/core/include.h>

namespace cineris::renderer {

	class Framebuffer {
	public:
		Framebuffer(int iWidth, int iHeight);
		~Framebuffer();

		auto bind() -> void;
		auto unbind() -> void;
		auto resize(int iWidth, int iHeight) -> void;
		auto getColorTexture() const -> unsigned int;

		auto getWidth() const -> int { return m_iWidth; }
		auto getHeight() const -> int { return m_iHeight; }

	private:
		int m_iWidth;
		int m_iHeight;
		unsigned int m_nFBO;
		unsigned int m_nColorTexture;
		unsigned int m_nRBO;
	};

}