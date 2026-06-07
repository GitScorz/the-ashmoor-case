#pragma once
#include <cineris/include.h>
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

        auto isLightSource() const -> bool { return m_bIsLightSource; }

        auto getModelMatrix() const -> glm::mat4;
        auto getAABB() const -> AABB;

        auto getId() const -> int { return m_ObjectId; }
        auto isCollidable() const -> bool { return m_bIsCollidable; }
        auto getPosition() const -> glm::vec3 { return m_Position; }
        auto getRotation() const -> glm::vec3 { return m_Rotation; }
        auto getScale() const -> glm::vec3 { return m_Scale; }
    private:
        glm::vec3 m_Position, m_Rotation, m_Scale;
        int m_ObjectId;

        bool m_bIsLightSource = false;
        bool m_bIsCollidable = true;
    };

}