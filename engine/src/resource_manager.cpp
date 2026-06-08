#include <cineris/resource_manager.h>
#include <cineris/logger.h>

namespace cineris {
    auto ResourceManager::getShader(const std::string& name) -> renderer::Shader* {
        auto it = m_Shaders.find(name);
        if (it != m_Shaders.end()) return it->second.get();
        m_Shaders[name] = std::make_unique<renderer::Shader>(name);
        return m_Shaders[name].get();
    }

    auto ResourceManager::getTexture(const std::string& name) -> renderer::Texture* {
        auto it = m_Textures.find(name);
        if (it != m_Textures.end()) return it->second.get();
        m_Textures[name] = std::make_unique<renderer::Texture>(name);
        return m_Textures[name].get();
    }

    auto ResourceManager::getCubeMesh() -> renderer::Mesh* {
        if (!m_CubeMesh) m_CubeMesh = std::unique_ptr<renderer::Mesh>(renderer::Mesh::createCube());
        return m_CubeMesh.get();
    }

    auto ResourceManager::reloadShaders() -> void {
        for (auto& [name, shader] : m_Shaders) {
            shader->reload();
        }

        LOG_INFO(log::LogChannel::Renderer, "Shaders reloaded successfully!");
    }

    auto ResourceManager::reloadTextures() -> void {
        // for (auto& [name, texture] : m_Textures) {
        // }
        LOG_WARN(log::LogChannel::Renderer, "RELOAD TEXTURES NOT IMPLEMENTED!");
    }

    auto ResourceManager::reloadAll() -> void {
        reloadShaders();
        reloadTextures();
    }
}