#include <cineris/core/window.h>
#include <cineris/core/logger.h>

namespace cineris {
    Window::Window(int w, int h, const std::string& t)
        : m_iWidth(w),
        m_iHeight(h),
        m_Title(t)
    {
        if (!glfwInit())
        {
            LOG_FATAL(log::LogChannel::Engine, "GLFW couldn't start.");
            exit(-1);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        m_Handle = glfwCreateWindow(w, h, t.c_str(), nullptr, nullptr);
        if (!m_Handle)
        {
            LOG_FATAL(log::LogChannel::Engine, "Failed to create GLFW window.");
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(m_Handle);
        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, framebufferSizeCallback);
        glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    Window::~Window() {
        glfwDestroyWindow(m_Handle);
        glfwTerminate();
    }

    auto Window::update() -> void {
        glfwSwapBuffers(m_Handle);
        glfwPollEvents();
    }

    auto Window::shouldClose() const -> bool {
        return glfwWindowShouldClose(m_Handle);
    }

    auto Window::updateFPS() -> void {
        // double currentTime = glfwGetTime();
        // m_iFrames++;

        // if (currentTime - m_dLastFrameTime >= 1.0)
        // {
        //   double fps = double(m_iFrames) / (currentTime - m_dLastFrameTime);

        //   std::stringstream ss;
        //   ss << m_Title << " - FPS: " << std::fixed << std::setprecision(2) << fps;

        //   glfwSetWindowTitle(m_Handle, ss.str().c_str());

        //   m_iFrames = 0;
        //   m_dLastFrameTime = currentTime;
        // }
    }

    auto Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) -> void {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (self) {
            self->onFramerbufferResize(width, height);
        }
    }

    auto Window::onFramerbufferResize(int width, int height) -> void {
        glViewport(0, 0, width, height);

        m_iWidth = width;
        m_iHeight = height;

        m_resizeCallback(width, height);
    }
}