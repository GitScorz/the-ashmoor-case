#pragma once
#include <cineris/core/include.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace cineris::renderer {

    class Shader {
    public:
        Shader(const std::string& shaderName);
        ~Shader();

        auto use() const -> void;
        auto setMat2(const std::string& name, const glm::mat2& mat) const -> void;
        auto setMat3(const std::string& name, const glm::mat3& mat) const -> void;
        auto setMat4(const std::string& name, const glm::mat4& mat) const -> void;

        auto setFloat(const std::string& name, float value) const -> void;
        auto setInt(const std::string& name, int value) const -> void;
        auto setBool(const std::string& name, bool value) const -> void;

        auto setVec2(const std::string& name, const glm::vec2& value) const -> void;
        auto setVec2(const std::string& name, float x, float y) const -> void;

        auto setVec3(const std::string& name, const glm::vec3& value) const -> void;
        auto setVec3(const std::string& name, float x, float y, float z) const -> void;

        auto setVec4(const std::string& name, const glm::vec4& value) const -> void;
        auto setVec4(const std::string& name, float x, float y, float z, float w) const -> void;

        auto reload() -> void;

        auto getId() const -> unsigned int {
            return m_RendererID;
        }

    private:
        unsigned int m_RendererID = 0;
        std::string m_Name;
        mutable std::unordered_map<std::string, int> m_UniformCache;

        static unsigned int s_BoundID;

        auto deleteShaderModules(unsigned int vertModule, unsigned int fragModule) -> void;
        auto createShaderProgram(unsigned int vertModule, unsigned int fragModule) -> unsigned int;
        static auto loadShaderFile(const std::string& filepath, std::unordered_set<std::string>& includeStack) -> std::string;
        auto compile(const std::string& filepath, unsigned int type) -> unsigned int;
        auto getUniformLocation(const std::string& name) const -> int;
    };

}