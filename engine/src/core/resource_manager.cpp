#include <cineris/core/resource_manager.h>
#include <cineris/core/logger.h>

namespace cineris {
    auto ResourceManager::loadShader(const std::string& id, const std::string& path) -> renderer::Shader* {
        auto it = m_Shaders.find(id);
        if (it != m_Shaders.end()) return it->second.get();
        m_Shaders[id] = std::make_unique<renderer::Shader>(path);
        return m_Shaders[id].get();
    }

    auto ResourceManager::getShader(const std::string& id) -> renderer::Shader* {
        auto it = m_Shaders.find(id);
        if (it == m_Shaders.end()) {
            LOG_WARN(log::LogChannel::Renderer, "Shader not found: {}", id);
            return nullptr;
        }
        return it->second.get();
    }

    auto ResourceManager::loadTexture(const std::string& id, const std::string& path) -> renderer::Texture* {
        auto it = m_Textures.find(id);
        if (it != m_Textures.end()) return it->second.get();
        m_Textures[id] = std::make_unique<renderer::Texture>(path);
        return m_Textures[id].get();
    }

    auto ResourceManager::getTexture(const std::string& id) -> renderer::Texture* {
        auto it = m_Textures.find(id);
        if (it == m_Textures.end()) {
            LOG_WARN(log::LogChannel::Renderer, "Texture not found: {}", id);
            return nullptr;
        }
        return it->second.get();
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