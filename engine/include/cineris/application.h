#pragma once
#include "include.h"
#include <cineris/camera.h>
#include <cineris/window.h>
#include <cineris/renderer/shader.h>
#include <cineris/renderer/texture.h>
#include <cineris/renderer/text_renderer.h>
#include <cineris/input/input_manager.h>
#include <cineris/resource_manager.h>

namespace cineris {

    class Application
    {
    public:
        double m_dLastFrame = 0.0, m_dDeltaTime = 0.0;

        explicit Application(const std::string& title);
        virtual ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        auto run() -> void;

    protected:
        virtual auto onInit() -> void {}
        virtual auto onUpdate(double deltaTime) -> void {}
        virtual auto onRender() -> void {}
        virtual auto onShutdown() -> void {}

        auto window() -> Window&;
        auto camera() -> Camera&;
        auto input() -> input::InputManager&;
        auto textRenderer() -> renderer::TextRenderer&;
        auto resources() -> ResourceManager&;

    private:
        auto render() -> void;
        auto update(double deltaTime) -> void;
        auto processInput() -> void;
        auto onResize(int width, int height) -> void;

        auto calculateDeltaTime() -> double {
            double dCurrentFrame = glfwGetTime();
            double dDeltaTime = dCurrentFrame - m_dLastFrame;
            m_dLastFrame = dCurrentFrame;

            if (dDeltaTime > 0.1) // max 100 ms per frame
                dDeltaTime = 0.1;

            return dDeltaTime;
        }
    private:
        std::unique_ptr<Window> m_pWindow;
        std::unique_ptr<Camera> m_pCamera;
        std::unique_ptr<input::InputManager> m_pInputManager;
        std::unique_ptr<renderer::TextRenderer> m_pTextRenderer;
    };

}