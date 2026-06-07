#include "world_object.h"

namespace ashmoor {

    auto WorldObject::draw(const RenderContext& context) -> void {
        m_material.bind();

        auto* shader = m_material.shader;
        if (!shader || !m_pMesh) {
            return;
        }

        shader->setMat4("model", getModelMatrix());
        shader->setMat4("view", context.view);
        shader->setMat4("projection", context.projection);
        shader->setVec3("viewPos", context.viewPos);
        shader->setVec3("lightColor", context.lightColor);
        shader->setBool("isLightSource", isLightSource());

        shader->setInt("skybox", 0);

        shader->setInt("uFogEnabled", context.fog.bEnabled ? 1 : 0);
        shader->setVec3("uFogColor", context.fog.v_Color);
        shader->setFloat("uFogDensity", context.fog.fDensity);
        shader->setFloat("uFogStart", context.fog.fStart);
        shader->setFloat("uFogEnd", context.fog.fEnd);

        int lightCount = static_cast<int>(context.lightPositions.size());
        if (lightCount > RenderContext::MAX_LIGHTS) {
            lightCount = RenderContext::MAX_LIGHTS;
        }
        shader->setInt("lightCount", lightCount);

        for (int i = 0; i < lightCount; ++i) {
            shader->setVec3("lightPositions[" + std::to_string(i) + "]", context.lightPositions[i]);
        }

        m_pMesh->draw();
    }

    auto WorldObject::getModelMatrix() const -> glm::mat4 {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_Position);
        model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, m_Scale);
        return model;
    }

    auto WorldObject::getAABB() const -> AABB {
        // TODO: calculate the AABB based on the actual vertices of the mesh lol
        glm::vec3 outMin = m_Position - m_Scale;
        glm::vec3 outMax = m_Position + m_Scale;
        return { outMin, outMax };
    }
}