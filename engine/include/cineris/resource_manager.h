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

        auto getShader(const std::string& name) -> renderer::Shader*;
        auto getTexture(const std::string& name) -> renderer::Texture*;
        auto getCubeMesh() -> renderer::Mesh*;

        auto reloadShaders() -> void;
        auto reloadTextures() -> void;

        auto reloadAll() -> void;

    private:
        ResourceManager() = default;
        std::unordered_map<std::string, std::unique_ptr<renderer::Shader>> m_Shaders;
        std::unordered_map<std::string, std::unique_ptr<renderer::Texture>> m_Textures;
        std::unique_ptr<renderer::Mesh> m_CubeMesh;
    };

}