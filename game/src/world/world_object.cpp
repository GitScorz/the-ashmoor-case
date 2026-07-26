#include "world_object.h"
#include <cineris/math/aabb.h>

namespace ashmoor {

    auto WorldObject::draw(const RenderContext& context) -> void {
        auto* shader = m_material.shader;
        if (!shader || !m_pMesh) {
            return;
        }

        shader->use();
        shader->setMat4("model", getModelMatrix());

        applyCameraUniforms(shader, context);

        if (!isLightSource()) {
            //shader->setInt("skybox", 0);
            applyLightingUniforms(shader, context);
            applyFogUniforms(shader, context);
        }

        m_material.bind();
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

    auto WorldObject::getAABB() const -> cineris::math::AABB {
        if (!m_pMesh) return {};

        return cineris::math::transformAABB(
            m_pMesh->getLocalBounds(),
            getModelMatrix()
        );
    }

    auto WorldObject::applyCameraUniforms(const cineris::renderer::Shader* shader, const RenderContext& context) -> void {
        shader->setMat4("view", context.view);
        shader->setMat4("projection", context.projection);
        shader->setVec3("viewPos", context.viewPos);
    }

    auto WorldObject::applyLightingUniforms(const cineris::renderer::Shader* shader, const RenderContext& context) -> void {
        shader->setVec3("uAmbientColor", glm::vec3(0.20f, 0.24f, 0.30f));
        shader->setFloat("uAmbientStrength", 0.18f);

        shader->setBool("isLightSource", isLightSource());
        shader->setVec3("lightColor", context.lightColor);

        int lightCount = static_cast<int>(context.lightPositions.size());
        if (lightCount > RenderContext::MAX_LIGHTS) {
            lightCount = RenderContext::MAX_LIGHTS;
        }
        shader->setInt("lightCount", lightCount);

        for (int i = 0; i < lightCount; ++i) {
            shader->setVec3("lightPositions[" + std::to_string(i) + "]", context.lightPositions[i]);

            if (i < static_cast<int>(context.lightColors.size())) {
                shader->setVec3("lightColors[" + std::to_string(i) + "]", context.lightColors[i]);
            } else {
                shader->setVec3("lightColors[" + std::to_string(i) + "]", context.lightColor);
            }
        }
    }

    auto WorldObject::applyFogUniforms(const cineris::renderer::Shader* shader, const RenderContext& context) -> void {
        shader->setInt("uFogEnabled", context.fog.enabled ? 1 : 0);
        if (context.fog.enabled) {
            shader->setVec3("uFogColor", context.fog.color);
            shader->setFloat("uFogDensity", context.fog.density);
            shader->setFloat("uFogStart", context.fog.start);
            shader->setFloat("uFogEnd", context.fog.end);
        }
    }
}