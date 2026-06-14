#include "world.h"
#include <cineris/core/resource_manager.h>
#include "ashmoor_case.h"
#include "paths.h"
#include "controllers/player_controller.h"
#include <cineris/core/logger.h>

namespace ashmoor {

    World::World() {}

    World::~World() {
        for (WorldObject* obj : m_Objects) {
            delete obj;
        }
        m_Objects.clear();
    }

    auto World::loadLevel(PlayerController* playerController, const std::vector<std::string>& levelData) -> void {
        clearObjects();

        for (size_t row = 0; row < levelData.size(); ++row) {
            const std::string& levelRow = levelData[row];
            for (size_t col = 0; col < levelRow.size(); ++col) {
                char tile = levelRow[col];
                if (tile == '#') {
                    // Mesh* wallMesh = ResourceManager::get().getCubeMesh();
                    // Shader* wallShader = ResourceManager::get().getShader(Paths::Shaders + "lightning");
                    // WorldObject* wallObject = new WorldObject(wallMesh, wallShader, nullptr, glm::vec3(col, row, 0.0f));
                    // wallObject->setObjectColor(glm::vec3(0.6f, 0.6f, 0.65f));
                    // addObject(wallObject);
                }
                else if (tile == 'P') {
                    // start position
                    playerController->setPosition(glm::vec3(col, row, 0.0f));
                }
                else if (tile == 'L') {
                    // light source
                    cineris::renderer::Material lightMaterial;
                    lightMaterial.shader = cineris::ResourceManager::get().loadShader("cube", Paths::Shaders + "cube");
                    cineris::renderer::Mesh* lightMesh = cineris::renderer::Mesh::createCube();
                    WorldObject* lightObject = new WorldObject(lightMesh, lightMaterial, glm::vec3(col, row, 0.0f));
                    lightObject->setScale(glm::vec3(0.2f));
                    lightObject->setIsLightSource(true);
                    addObject(lightObject);
                }
            }
        }

        float floorAreaSize = 0.f;

        for (const std::string& row : levelData) {
            floorAreaSize = std::max(floorAreaSize, static_cast<float>(row.size()));
        }

        LOG_DEBUG(cineris::log::LogChannel::Game, "Floor area size: {}", floorAreaSize);

        renderer::Mesh* floorMesh = renderer::Mesh::createGrid(20.f, 20.f, 100, 100);
        renderer::Material floorMaterial;

        floorMaterial.shader = cineris::ResourceManager::get().loadShader("lightning", Paths::Shaders + "lightning");
        floorMaterial.albedo = cineris::ResourceManager::get().loadTexture("basic_rock", Paths::Textures + "Rocks024L_1K/Rocks024L_1K-PNG_Color.png");
        floorMaterial.color = glm::vec3(0.25f, 0.35f, 0.25f);

        WorldObject* floorObject = new WorldObject(floorMesh, floorMaterial, glm::vec3(floorAreaSize / 2.0f - 0.5f, levelData.size() / 2.0f - 0.5f, -0.5f));
        floorObject->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
        addObject(floorObject);
    }

}