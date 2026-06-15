#pragma once
#include <vector>
#include "world_object.h"
#include <cineris/renderer/render_context.h>
#include <cineris/core/scene.h>
#include <cineris/math/aabb.h>

namespace ashmoor {
    class PlayerController;

    class World {
    public:
        std::vector<WorldObject*> m_Objects;
        World();
        ~World();

        auto addObject(WorldObject* object) -> void { 
            m_Objects.push_back(object); 
        }

        auto clearObjects() -> void;

        auto draw(const RenderContext& context) -> void;

        //auto loadLevel(PlayerController* playerController, const std::vector<std::string>& levelData) -> void;
        auto loadScene(PlayerController* pController, const cineris::Scene& scene) -> void;

        auto getObjectFromId(int id) -> WorldObject*;
        auto getLightPositions(int maxLights = RenderContext::MAX_LIGHTS) const -> std::vector<glm::vec3>;
        auto getLightColors(int maxLights = RenderContext::MAX_LIGHTS) const -> std::vector<glm::vec3>;

        auto collides(const AABB& box) -> bool;
    };

}