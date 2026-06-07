#include "world_object.h"

namespace ashmoor {

    auto WorldObject::draw(const RenderContext& context) -> void {
        m_pShader->use();

        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_Position);
        model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, m_Scale);

        m_pShader->setMat4("model", model);
        m_pShader->setMat4("view", context.view);
        m_pShader->setMat4("projection", context.projection);
        m_pShader->setVec3("viewPos", context.viewPos);
        m_pShader->setVec3("lightColor", context.lightColor);
        m_pShader->setVec3("objectColor", m_ObjectColor);
        m_pShader->setBool("isLightSource", m_bIsLightSource);

        m_pShader->setInt("uFogEnabled", context.fog.bEnabled ? 1 : 0);
        m_pShader->setVec3("uFogColor", context.fog.v_Color);
        m_pShader->setFloat("uFogDensity", context.fog.fDensity);
        m_pShader->setFloat("uFogStart", context.fog.fStart);
        m_pShader->setFloat("uFogEnd", context.fog.fEnd);

        int lightCount = static_cast<int>(context.lightPositions.size());
        if (lightCount > RenderContext::MAX_LIGHTS) {
            lightCount = RenderContext::MAX_LIGHTS;
        }
        m_pShader->setInt("lightCount", lightCount);

        for (int i = 0; i < lightCount; ++i) {
            m_pShader->setVec3("lightPositions[" + std::to_string(i) + "]", context.lightPositions[i]);
        }

        if (m_pTexture) {
            m_pTexture->bind(0);
            m_pShader->setInt("uTexture", 0);
        }

        m_pMesh->draw();
    }

    auto WorldObject::getAABB() -> AABB {
        // TODO: calculate the AABB based on the actual vertices of the mesh lol
        glm::vec3 outMin = m_Position - m_Scale;
        glm::vec3 outMax = m_Position + m_Scale;
        return { outMin, outMax };
    }
}