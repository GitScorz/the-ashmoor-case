#include <cineris/renderer/skybox.h>

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

namespace cineris::renderer {
    Skybox::~Skybox() {
        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
            m_textureID = 0;
        }

        if (m_VBO != 0) {
            glDeleteBuffers(1, &m_VBO);
            m_VBO = 0;
        }

        if (m_VAO != 0) {
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }

        m_pShader = nullptr;
    }

    auto Skybox::createCube() -> void {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    auto Skybox::load(const std::array<std::string, 6>& faces, Shader* shader) -> void {
        if (!shader || faces.empty()) return;

        createCube();

        m_pShader = shader;
        m_pShader->setInt("skybox", 0);

        m_textureID = loadCubemap(faces);

        std::cout << "Loaded skybox!" << std::endl;
    }

    auto Skybox::draw(const glm::mat4& view, const glm::mat4& projection) ->void {
        if (!m_pShader || m_textureID == 0) {
            return;
        }

        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        m_pShader->use();

        // Remove translation from the view matrix
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

        // rotate skybox to match Z-up coordinate system (world uses Z as up, skybox uses Y as up)
        skyboxView = glm::rotate(skyboxView, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        m_pShader->setMat4("view", skyboxView);
        m_pShader->setMat4("projection", projection);

        glBindVertexArray(m_VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    auto Skybox::loadCubemap(const std::array<std::string, 6>& faces) -> unsigned int {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;

        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                GLenum format = GL_RGB;
                if (nrChannels == 1) format = GL_RED;
                else if (nrChannels == 3) format = GL_RGB;
                else if (nrChannels == 4) format = GL_RGBA;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
}