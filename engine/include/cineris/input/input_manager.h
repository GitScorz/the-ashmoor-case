#pragma once
#include <cineris/core/include.h>
#include <unordered_map>
#include <functional>

namespace cineris::input {
    class InputManager {
    public:
        bool m_bFirstMouse = true;
        double m_dLastMouseX = 0.0, m_dLastMouseY = 0.0;

        InputManager(GLFWwindow* window);
        ~InputManager();

        auto processInput() -> void;
        auto registerKeyBinding(int key, std::function<void()> func) -> void;

        auto isControlPressed(int key) const -> bool {
            return glfwGetKey(m_pWindow, key) == GLFW_PRESS;
        }

        auto isMouseButtonPressed(int button) const -> bool {
            return glfwGetMouseButton(m_pWindow, button) == GLFW_PRESS;
        }

        auto getMouseDeltaX() -> float {
            double xpos, ypos;
            glfwGetCursorPos(m_pWindow, &xpos, &ypos);
            float deltaX = xpos - m_dLastMouseX;
            m_dLastMouseX = xpos;
            return deltaX;
        }

        auto getMouseDeltaY() -> float {
            double xpos, ypos;
            glfwGetCursorPos(m_pWindow, &xpos, &ypos);
            float deltaY = ypos - m_dLastMouseY;
            m_dLastMouseY = ypos;
            return deltaY;
        }

        auto setMouseSensitivity(float sensitivity) -> void {
            m_fMouseSensitivity = sensitivity;
        }

        auto getMouseSensitivity() const -> float {
            return m_fMouseSensitivity;
        }
    private:
      GLFWwindow* m_pWindow;
      static std::unordered_map<int, bool> s_mKeyStates;
      static std::unordered_map<int, std::function<void()>> s_mKeyBindings;
      float m_fMouseSensitivity = 0.1f;
    };
}
