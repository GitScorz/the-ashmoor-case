#include "player_controller.h"
#include "world/world.h"

namespace ashmoor {

    PlayerController::PlayerController(cineris::Camera& camera, cineris::input::InputManager& input)
        : m_Camera(camera),
        m_Input(input),
        m_vPosition(0.0f, 0.0f, 0.0f),
        m_fSpeed(5.0f)
    {

    }

    auto PlayerController::getAABBAt(glm::vec3 vPosition) -> cineris::math::AABB {
        return {
            vPosition + glm::vec3(-0.3f, -0.3f, 0.0f),
            vPosition + glm::vec3(0.3f,  0.3f, 1.8f)
        };
    }

    auto PlayerController::update(float fDeltaTime, World* pWorld) -> void {
        glm::vec3 vFront = m_Camera.getFront();
        glm::vec3 vRight = glm::normalize(glm::cross(vFront, glm::vec3(0.0f, 0.0f, 1.0f)));

        vFront.z = 0.0f;
        vRight.z = 0.0f;

        m_vMovement = glm::vec3(0.0f);

        if (m_Input.isControlPressed(GLFW_KEY_W))
            m_vMovement += vFront * m_fSpeed * fDeltaTime;
        if (m_Input.isControlPressed(GLFW_KEY_S))
            m_vMovement -= vFront * m_fSpeed * fDeltaTime;
        if (m_Input.isControlPressed(GLFW_KEY_A))
            m_vMovement -= vRight * m_fSpeed * fDeltaTime;
        if (m_Input.isControlPressed(GLFW_KEY_D))
            m_vMovement += vRight * m_fSpeed * fDeltaTime;
        if (m_Input.isControlPressed(GLFW_KEY_E))
            m_vMovement += glm::vec3(0.0f, 0.0f, 1.0f) * m_fSpeed * fDeltaTime;
        if (m_Input.isControlPressed(GLFW_KEY_Q))
            m_vMovement -= glm::vec3(0.0f, 0.0f, 1.0f) * m_fSpeed * fDeltaTime;
        if (m_Input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
            m_Camera.zoomIn(fDeltaTime);
        else
            m_Camera.zoomOut(fDeltaTime);

        if (glm::length(m_vMovement) > 0.0f)
            m_vMovement = glm::normalize(m_vMovement) * m_fSpeed * fDeltaTime;

		// calculate each axis movement separately to avoid getting stuck on walls
        glm::vec3 candidate = m_vPosition;
        candidate.x += m_vMovement.x;

        if (!pWorld->collides(getAABBAt(candidate))) {
            m_vPosition.x = candidate.x;
        }

        candidate = m_vPosition;
        candidate.y += m_vMovement.y;

        if (!pWorld->collides(getAABBAt(candidate))) {
            m_vPosition.y = candidate.y;
        }

        candidate = m_vPosition;
        candidate.z += m_vMovement.z;

        if (!pWorld->collides(getAABBAt(candidate))) {
            m_vPosition.z = candidate.z;
        }

        m_vPosition += m_vVelocity * fDeltaTime;

        m_Camera.updateCameraPosition(m_vPosition);
    }


    PlayerController::~PlayerController() {

    }

}
