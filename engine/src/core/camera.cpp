#include <cineris/core/camera.h>
#include <iostream>

namespace cineris {

    Camera::Camera(glm::vec3 vTargetPos)
        : m_vFront(glm::vec3(0.0f, 0.0f, -1.0f)),
        m_vUp(glm::vec3(0.0f, 0.0f, 1.0f)),
        m_fYaw(90.0f),
        m_fPitch(0.0f),
        m_vTargetOffset(glm::vec3(0.0f, 2.0f, 4.0f)),
        m_fDefaultDistance(glm::length(m_vTargetOffset)),
        m_fZoomDistance(m_fDefaultDistance * 0.5f)
    {
        updateCameraPosition(vTargetPos);
    }

    Camera::~Camera() {}

    auto Camera::getViewMatrix() const -> glm::mat4 {
        return glm::lookAt(m_vPosition, m_vPosition + m_vFront, m_vUp);
    }

    auto Camera::updateCameraPosition(glm::vec3 vTarget) -> void {
        m_vTarget = vTarget;
        m_vPosition = m_vTarget + m_vTargetOffset;
        m_vFront = glm::normalize(m_vTarget - m_vPosition);
    }

    auto Camera::zoomIn(float fDeltaTime) -> void {
        zoomTo(m_fZoomDistance, fDeltaTime);
    }

    auto Camera::zoomOut(float fDeltaTime) -> void {
        zoomTo(m_fDefaultDistance, fDeltaTime);
    }

    auto Camera::zoomTo(float fTargetDistance, float fDeltaTime) -> void {
        constexpr float fZoomSpeed = 10.0f;

        float fCurrentDistance = glm::length(m_vTargetOffset);
        if (fCurrentDistance <= 0.0f)
            return;

        float fBlend = glm::clamp(fZoomSpeed * fDeltaTime, 0.0f, 1.0f);
        float fNewDistance = glm::mix(fCurrentDistance, fTargetDistance, fBlend);
        m_vTargetOffset *= fNewDistance / fCurrentDistance;
    }

    auto Camera::rotate(float fDeltaYaw, float fDeltaPitch) -> void
    {
        // TODO: invert camera settings
        m_fYaw -= fDeltaYaw;
        m_fPitch = glm::clamp(m_fPitch + fDeltaPitch, -89.0f, 89.0f);

        float fRadYaw = glm::radians(m_fYaw);
        float fRadPitch = glm::radians(m_fPitch);

        float fRadius = glm::length(m_vTargetOffset);
        m_vTargetOffset.x = fRadius * cos(fRadPitch) * cos(fRadYaw);
        m_vTargetOffset.y = fRadius * cos(fRadPitch) * sin(fRadYaw);
        m_vTargetOffset.z = fRadius * sin(fRadPitch);

        m_vPosition = m_vTarget + m_vTargetOffset;

        m_vFront = glm::normalize(m_vTarget - m_vPosition);
    }

}
