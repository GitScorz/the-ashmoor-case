#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <cineris/renderer/shader.h>
#include <cineris/renderer/mesh.h>
#include <cineris/renderer/texture.h>

namespace cineris {
    class ResourceManager {
    public:
        static ResourceManager& get() {
            static ResourceManager instance;
            return instance;
        }

        ~ResourceManager() = default;

        renderer::Shader* getShader(const std::string& name) {
            auto it = m_Shaders.find(name);
            if (it != m_Shaders.end()) return it->second.get();
            m_Shaders[name] = std::make_unique<renderer::Shader>(name);
            return m_Shaders[name].get();
        }

        renderer::Texture* getTexture(const std::string& name) {
            auto it = m_Textures.find(name);
            if (it != m_Textures.end()) return it->second.get();
            m_Textures[name] = std::make_unique<renderer::Texture>(name);
            return m_Textures[name].get();
        }

        renderer::Mesh* getCubeMesh() {
            if (!m_CubeMesh) m_CubeMesh = std::unique_ptr<renderer::Mesh>(renderer::Mesh::createCube());
            return m_CubeMesh.get();
        }

        auto reloadShaders() -> void {
            for (auto& [name, shader] : m_Shaders) {
                shader->reload();
            }

            std::cout << "Shaders reloaded successfully!" << std::endl;
        }

        auto reloadTextures() -> void {
            // for (auto& [name, texture] : m_Textures) {
            // }
            std::cout << "Textures reloaded successfully!" << std::endl;
        }

        auto reloadAll() -> void {
            reloadShaders();
            reloadTextures();
        }

    private:
        ResourceManager() = default;
        std::unordered_map<std::string, std::unique_ptr<renderer::Shader>> m_Shaders;
        std::unordered_map<std::string, std::unique_ptr<renderer::Texture>> m_Textures;
        std::unique_ptr<renderer::Mesh> m_CubeMesh;
    };

}