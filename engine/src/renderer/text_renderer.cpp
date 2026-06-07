#include <cineris/renderer/text_renderer.h>
#include <cineris/renderer/texture.h>

namespace cineris::renderer {

    TextRenderer::TextRenderer(Window* window) : m_pWindow(window)
    {
        if (FT_Init_FreeType(&m_FreeType)) {
            std::cout << "Coulnd't initialize FreeType" << std::endl;
            return;
        }
    }

    TextRenderer::~TextRenderer() {
        for (auto& [character, glyph] : m_Characters) {
            glDeleteTextures(1, &glyph.textureID);
        }

        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        if (m_FreeType) {
            FT_Done_FreeType(m_FreeType);
        }
    };

    auto TextRenderer::loadFont(const std::string& filepath, unsigned int pixelSize, Shader* shader) -> void {
        if (FT_New_Face(m_FreeType, filepath.c_str(), 0, &m_Face)) {
            // i should really do a logging system..
            std::cout << "Couldn't load font" << std::endl;
            return;
        }

        FT_Set_Pixel_Sizes(m_Face, 0, pixelSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // todo: later support unicode/utf-8
        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER)) {
                std::cout << "Failed to load glyph: " << c << " (" << filepath << ")" << std::endl;
                continue;
            }

            // std::cout << "Glyph " << c << " bitmap: " << m_Face->glyph->bitmap.width << "x" << m_Face->glyph->bitmap.rows << "\n";

            // load up
            unsigned int textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                m_Face->glyph->bitmap.width,
                m_Face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                m_Face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
              textureID,
              glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
              glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
              static_cast<unsigned int>(m_Face->glyph->advance.x)
            };

            m_Characters.insert({ c, character });
        }

        FT_Done_Face(m_Face);

        // load mesh
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_pShader = shader;

        if (!m_pShader) {
            throw std::runtime_error("TextRenderer received null shader");
        }

        updateProjection();
    }

    auto TextRenderer::updateProjection() -> void {
        if (!m_pShader || !m_pWindow) {
            return;
        }

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_pWindow->m_iWidth), 0.0f, static_cast<float>(m_pWindow->m_iHeight));
        m_pShader->use();
        m_pShader->setMat4("projection", projection);
    }

    auto TextRenderer::renderText(const std::string& text, float x, float y, float scale, glm::vec4 color) -> void {
        if (!m_pShader || text.empty()) {
            return;
        }

        // todo: update on window resize
        updateProjection();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_pShader->use();
        m_pShader->setInt("text", 0);
        m_pShader->setVec4("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VAO);

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            auto it = m_Characters.find(*c);
            if (it == m_Characters.end())
                continue;

            const Character& ch = it->second;

            float xpos = x + ch.bearing.x * scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * scale;
            // glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            float vertices[6][4] = {
              { xpos,     ypos + h,   0.0f, 0.0f },
              { xpos,     ypos,       0.0f, 1.0f },
              { xpos + w, ypos,       1.0f, 1.0f },

              { xpos,     ypos + h,   0.0f, 0.0f },
              { xpos + w, ypos,       1.0f, 1.0f },
              { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (ch.advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_BLEND);
    }

    auto TextRenderer::setShader(Shader* shader) -> void {
        m_pShader = shader;

        if (!m_pShader)
            throw std::runtime_error("TextRenderer received null shader on reassigning shader");

        updateProjection();
    }
}