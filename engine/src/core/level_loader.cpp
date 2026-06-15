#include <cineris/core/level_loader.h>
#include <iostream>
#include <cineris/core/logger.h>
#include <nlohmann/json.hpp>
#include <fstream>

struct PlayerSpawnData {
    glm::vec3 position{};
    glm::vec3 rotation{};
};

namespace cineris {
    auto LevelLoader::load(const std::string& path, const std::string& levelName, Scene& scene) -> bool {
        std::ifstream file(path + levelName);

        if (!file.is_open()) {
            LOG_ERROR(log::LogChannel::Engine, "Failed to open level file: {}", levelName);
            return {};
        }

        scene.clear();

        try {
            nlohmann::json j;
            file >> j;

            if (j.contains("objects")) {
                for (const auto& objJson : j["objects"]) {
                    GameObject obj;

                    obj.name = objJson.value("name", "");
                    obj.textureId = objJson.value("texture", "");
                    obj.meshId = objJson.value("mesh", "");

                    std::string type = objJson.value("type", "cube");

                    if (type == "cube") {
                        obj.type = GameObjectType::Cube;
                        if (obj.meshId.empty())
                            obj.meshId = "cube";
                    }
                    else if (type == "plane") {
                        obj.type = GameObjectType::Plane;
                        if (obj.meshId.empty())
                            obj.meshId = "plane";
                    }
                    else if (type == "model") {
                        obj.type = GameObjectType::Model;
                        obj.modelPath = objJson.value("model", "");
                    }
                    else if (type == "player") {
                        obj.type = GameObjectType::Player;
                    }

                    if (objJson.contains("position")) {
                        const auto& p = objJson["position"];
                        obj.transform.position = {
                            p[0].get<float>(),
                            p[1].get<float>(),
                            p[2].get<float>(),
                        };
                    }
                    

                    if (objJson.contains("rotation")) {
                        const auto& r = objJson["rotation"];
                        obj.transform.rotation = {
                            r[0].get<float>(),
                            r[1].get<float>(),
                            r[2].get<float>(),
                        };
                    }

                    if (objJson.contains("scale")) {
                        const auto& s = objJson["scale"];
                        obj.transform.scale = {
                            s[0].get<float>(),
                            s[1].get<float>(),
                            s[2].get<float>(),
                        };
                    }

                    obj.visible = objJson.value("visible", true);

                    scene.addObject(obj);
                }

            }

            if (j.contains("lights")) {
                for (const auto& lightJson : j["lights"]) {
                    Light light;

                    light.name = lightJson.value("name", "");

                    std::string type = lightJson.value("type", "directional");
                    if (type == "directional") {
                        light.type = LightType::Directional;
                    }
                    else if (type == "point") {
                        light.type = LightType::Point;
                    }
                    else if (type == "spot") {
                        light.type = LightType::Spot;
                    }

                    if (lightJson.contains("position")) {
                        const auto& p = lightJson["position"];
                        light.position = {
                            p[0].get<float>(),
                            p[1].get<float>(),
                            p[2].get<float>()
                        };
                    }

                    if (lightJson.contains("direction")) {
                        const auto& d = lightJson["direction"];
                        light.direction = {
                            d[0].get<float>(),
                            d[1].get<float>(),
                            d[2].get<float>()
                        };
                    }

                    if (lightJson.contains("color")) {
                        const auto& c = lightJson["color"];
                        light.color = {
                            c[0].get<float>(),
                            c[1].get<float>(),
                            c[2].get<float>()
                        };
                    }

                    light.intensity = lightJson.value("intensity", 1.0f);
                    light.radius = lightJson.value("radius", 10.0f);

                    scene.addLight(light);
                }
            }
        }
        catch (const std::exception& e) {
            LOG_ERROR(log::LogChannel::Engine, "Failed to load level file {}: {}", levelName, e.what());
            return false;
        }

        LOG_DEBUG(log::LogChannel::Engine, "Loaded level {} successfully!", levelName);

        return true;
    }

}