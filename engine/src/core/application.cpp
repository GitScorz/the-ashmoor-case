#include <cineris/core/application.h>
#include <cineris/core/logger.h>

namespace cineris {

    Application::Application(const std::string& title) {
        log::enableConsoleColors();

        m_pWindow = std::make_unique<Window>(1440, 720, title.c_str());

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_FATAL(log::LogChannel::Engine, "Failed to initialize GLAD");
            glfwTerminate();
            exit(-1);
        }

        m_pWindow->setResizeCallback([this](int width, int height) {
            onResize(width, height);
        });

        m_pCamera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));
        m_pInputManager = std::make_unique<input::InputManager>(m_pWindow->m_Handle);
        m_pTextRenderer = std::make_unique<renderer::TextRenderer>(m_pWindow.get());
    }

    Application::~Application() {}

    auto Application::run() -> void {
        onInit();

        while (!m_pWindow->shouldClose()) {
            m_dDeltaTime = calculateDeltaTime();

            processInput();

            update(m_dDeltaTime);
            onUpdate(m_dDeltaTime);

            render();
            onRender();

            m_pWindow->update();
            m_pWindow->updateFPS();
        }

        onShutdown();
    }

    auto Application::update(double deltaTime) -> void {

    }

    auto Application::processInput() -> void {
        m_pInputManager->processInput();
    }

    auto Application::render() -> void {
        
    }

    auto Application::window() -> Window& {
        return *m_pWindow;
    }

    auto Application::camera() -> Camera& {
        return *m_pCamera;
    }

    auto Application::input() -> input::InputManager& {
        return *m_pInputManager;
    }

    auto Application::textRenderer() -> renderer::TextRenderer& {
        return *m_pTextRenderer;
    }

    auto Application::resources() -> ResourceManager& {
        // singleton
        return ResourceManager::get();
    }

    auto Application::onResize(int width, int height) -> void {
        if (width <= 0 || height <= 0) return;

        //LOG_DEBUG(log::LogChannel::Engine, "Resizing window, new size: {0}x{1})", width, height);
        
        m_pTextRenderer->updateProjection();
        // todo: resize framebuffer class
    }
}