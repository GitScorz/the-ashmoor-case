#include <cineris/input/input_manager.h>

namespace cineris::input {
    std::unordered_map<int, bool> InputManager::s_mKeyStates;
    std::unordered_map<int, std::function<void()>> InputManager::s_mKeyBindings;

    InputManager::InputManager(GLFWwindow* window)
        : m_pWindow(window)
    {
        if (m_bFirstMouse) {
            glfwGetCursorPos(m_pWindow, &m_dLastMouseX, &m_dLastMouseY);
            m_bFirstMouse = false;
        }
    }

    InputManager::~InputManager() {}

    auto InputManager::processInput() -> void {
        for (auto& [key, func] : s_mKeyBindings) {
            if (glfwGetKey(m_pWindow, key) == GLFW_PRESS) {
                if (!s_mKeyStates[key]) {
                    func();
                    s_mKeyStates[key] = true;
                }
            }
            else {
                s_mKeyStates[key] = false;
            }
        }
    }

    auto InputManager::registerKeyBinding(int key, std::function<void()> func) -> void {
        s_mKeyBindings[key] = func;
        s_mKeyStates[key] = false; // Initialize key state
    }

}