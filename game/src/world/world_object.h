#pragma once
#include <cineris/core/include.h>
#include <cineris/renderer/mesh.h>
#include <cineris/renderer/shader.h>
#include <cineris/renderer/render_context.h>
#include <cineris/math/aabb.h>
#include <cineris/renderer/material.h>

namespace ashmoor {

    class WorldObject {
    public:
        cineris::renderer::Mesh* m_pMesh;
        cineris::renderer::Material m_material;

        WorldObject(cineris::renderer::Mesh* mesh, cineris::renderer::Material material, glm::vec3 pos = glm::vec3(0.0f))
            : m_pMesh(mesh),
            m_material(material),
            m_Position(pos),
            m_Rotation(0.0f),
            m_Scale(1.0f)
        {
            static int nextId = 0;
            m_ObjectId = nextId++;
        }

        ~WorldObject() = default;

        auto draw(const RenderContext& context) -> void;

        auto setPosition(const glm::vec3& position) -> void { m_Position = position; }
        auto setRotation(const glm::vec3& rotation) -> void { m_Rotation = rotation; }
        auto setScale(const glm::vec3& scale) -> void { m_Scale = scale; }
        auto setIsLightSource(bool isLight) -> void { m_bIsLightSource = isLight; }
        auto setIsCollidable(bool collidable) -> void { m_bIsCollidable = collidable; }
        auto setLightColor(const glm::vec3& color) -> void { m_LightColor = color; }

        auto isLightSource() const -> bool { return m_bIsLightSource; }
        auto getLightColor() const -> glm::vec3 { return m_LightColor; }

        auto getModelMatrix() const -> glm::mat4;
        auto getAABB() const -> AABB;

        auto getId() const -> int { return m_ObjectId; }
        auto isCollidable() const -> bool { return m_bIsCollidable; }
        auto getPosition() const -> glm::vec3 { return m_Position; }
        auto getRotation() const -> glm::vec3 { return m_Rotation; }
        auto getScale() const -> glm::vec3 { return m_Scale; }

        auto applyCameraUniforms(cineris::renderer::Shader* shader, const RenderContext& content) -> void;
        auto applyLightingUniforms(cineris::renderer::Shader* shader, const RenderContext& content) -> void;
        auto applyFogUniforms(cineris::renderer::Shader* shader, const RenderContext& content) -> void;
    private:
        glm::vec3 m_Position, m_Rotation, m_Scale;
        int m_ObjectId;

        bool m_bIsLightSource = false;
        bool m_bIsCollidable = true;
        glm::vec3 m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    };

}