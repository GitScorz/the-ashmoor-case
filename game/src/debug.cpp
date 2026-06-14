#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <windows.h>
#endif

#include "debug.h"
#include <cstdio>
#include <cineris/core/logger.h>

namespace ashmoor::debug {
    bool bInWireframeMode = false;

    auto setupDebugConsole() -> void {
        #if defined(_WIN32)
            if (!AttachConsole(ATTACH_PARENT_PROCESS) && !AllocConsole()) {
                return;
            }

            std::freopen("CONIN$", "r", stdin);
            std::freopen("CONOUT$", "w", stdout);
            std::freopen("CONOUT$", "w", stderr);
        #endif

        LOG_DEBUG(cineris::log::LogChannel::Game, "Debug output ready!");
    }

    auto registerDebugBindings(cineris::input::InputManager& pInput) -> void {
        pInput.registerKeyBinding(GLFW_KEY_F1, []() {
            bInWireframeMode = !bInWireframeMode;
            glPolygonMode(GL_FRONT_AND_BACK, bInWireframeMode ? GL_LINE : GL_FILL);
        });
    
        pInput.registerKeyBinding(GLFW_KEY_ESCAPE, [](){ 
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        });
    }
}

