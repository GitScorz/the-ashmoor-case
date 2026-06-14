#pragma once
#include <cineris/core/include.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <functional>

namespace cineris {
	class Window {
	public:
		GLFWwindow* m_Handle;
		int m_iWidth, m_iHeight;
		std::string m_Title;

		Window(int w, int h, const std::string& t);
		~Window();

		auto update() -> void;
		auto shouldClose() const -> bool;
		auto updateFPS() -> void;

		using ResizeCallback = std::function<void(int, int)>;
		auto setResizeCallback(ResizeCallback cb) -> void {
			m_resizeCallback = std::move(cb);
		};

	private:
		double m_dLastFrameTime = 0.0;
		int m_iFrames = 0;

		ResizeCallback m_resizeCallback;
		static auto framebufferSizeCallback(GLFWwindow* window, int width, int height) -> void;
		auto onFramerbufferResize(int width, int height) -> void;
	};
}