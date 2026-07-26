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

    auto World::clearObjects() -> void {
        for (WorldObject* obj : m_Objects) {
            delete obj;
        }

        m_Objects.clear();
    }

    auto World::getObjectFromId(int id) -> WorldObject* {
        for (WorldObject* obj : m_Objects) {
            if (obj->getId() == id) {
                return obj;
            }
        }
        return nullptr;
    }

    auto World::draw(const RenderContext& context) -> void {
        for (WorldObject* obj : m_Objects) {
             /*std::cout << "Drawing object at position: (" << obj->getPosition().x << ", " << obj->getPosition().y << ", " << obj->getPosition().z << ")" << std::endl;
             std::cout << "drawing object with id: " << obj->getId() << std::endl;*/
            obj->draw(context);
        }
    }

    auto World::loadScene(PlayerController* pController, const cineris::Scene& scene) -> void {
        clearObjects();

        for (const auto& obj : scene.getObjects()) {
            cineris::renderer::Mesh* mesh = nullptr;

            if (obj.type == cineris::GameObjectType::Cube) {
                mesh = cineris::ResourceManager::get().getCubeMesh();
            }
            else if (obj.type == cineris::GameObjectType::Plane) {
                mesh = cineris::renderer::Mesh::createGrid(obj.transform.size.x, obj.transform.size.y, 100, 100);
            }
            else if (obj.type == cineris::GameObjectType::Model) {
                LOG_ERROR(cineris::log::LogChannel::Game, "Model loading not implemented yet");
                continue;
            }
            else if (obj.type == cineris::GameObjectType::Player) {
                pController->setPosition(obj.transform.position);
                continue;
            }

            cineris::renderer::Material material;
            // maybe set the shader on level file?
            material.shader = cineris::ResourceManager::get().loadShader("lit_textured", Paths::Shaders + "lit_textured");

            if (!obj.textureId.empty()) {
                // remove texture format
                std::string textureId = obj.textureId;
                size_t pos = textureId.find_first_of('.');
                if (pos != std::string::npos)
                    textureId = textureId.substr(0, pos);

                material.albedo = cineris::ResourceManager::get().loadTexture(textureId, Paths::Textures + obj.textureId);
                material.color = glm::vec3(1.0f);

                WorldObject* worldObj = new WorldObject(mesh, material, obj.transform.position);
                worldObj->setRotation(obj.transform.rotation);
                worldObj->setScale(obj.transform.scale);

                addObject(worldObj);
            }
        }

        for (const auto& light : scene.getLights()) {
            cineris::renderer::Material lightMaterial;
            lightMaterial.shader = cineris::ResourceManager::get().loadShader("unlit", Paths::Shaders + "unlit");
            lightMaterial.color = light.color;

            cineris::renderer::Mesh* lightMesh = cineris::ResourceManager::get().getCubeMesh();
            WorldObject* lightObj = new WorldObject(lightMesh, lightMaterial, light.position);

            lightObj->setScale(glm::vec3(0.3f));
            lightObj->setIsLightSource(true);
            lightObj->setLightColor(light.color);

            addObject(lightObj);
        }

        LOG_DEBUG(
            cineris::log::LogChannel::Game,
            "Loaded scene into world: {} objects, {} lights",
            scene.getObjects().size(),
            scene.getLights().size()
        );
    }

    auto World::getLightPositions(int maxLights) const -> std::vector<glm::vec3> {
        std::vector<glm::vec3> lights;
        lights.reserve(maxLights);

        for (WorldObject* obj : m_Objects) {
            if (obj->isLightSource()) {
                lights.push_back(obj->getPosition());
                if (static_cast<int>(lights.size()) >= maxLights) {
                    break;
                }
            }
        }

        return lights;
    }

    auto World::getLightColors(int maxLights) const -> std::vector<glm::vec3> {
        std::vector<glm::vec3> colors;
        colors.reserve(maxLights);

        for (WorldObject* obj : m_Objects) {
            if (obj->isLightSource()) {
                colors.push_back(obj->getLightColor());
                if (static_cast<int>(colors.size()) >= maxLights) {
                    break;
                }
            }
        }

        return colors;
    }

    auto World::collides(const cineris::math::AABB& box) -> bool {
        for (const WorldObject* obj : m_Objects) {
            if (!obj || !obj->isCollidable()) {
                continue;
            }

            if (box.intersects(obj->getAABB())) {
                return true;
            }
        }
        return false;
    }

}