#include "shader.h"
#include "paths.h"

unsigned int Shader::s_BoundID = 0;

Shader::Shader(const std::string& shaderName) : m_Name(shaderName)
{
  std::string vertexFilePath = Paths::Shaders + shaderName + ".vert";
  std::string fragmentFilePath = Paths::Shaders + shaderName + ".frag";

  std::vector<unsigned int> modules = {
    compile(vertexFilePath, GL_VERTEX_SHADER),
    compile(fragmentFilePath, GL_FRAGMENT_SHADER)
  };

  m_RendererID = createShaderProgram(modules[0], modules[1]);
}

Shader::~Shader() {
  glDeleteProgram(m_RendererID);
}

auto Shader::deleteShaderModules(unsigned int vertModule, unsigned int fragModule) -> void {
  if (vertModule != 0) glDeleteShader(vertModule);
  if (fragModule != 0) glDeleteShader(fragModule);
}

auto Shader::createShaderProgram(unsigned int vertModule, unsigned int fragModule) -> unsigned int {
  if (vertModule == 0 || fragModule == 0) {
    deleteShaderModules(vertModule, fragModule);
    return 0;
  }
  
  unsigned int program = glCreateProgram();
  glAttachShader(program, vertModule);
  glAttachShader(program, fragModule);
  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char errorLog[512];
    glGetProgramInfoLog(program, 512, NULL, errorLog);
    std::cout << "Shader linking failed:\n" << errorLog << std::endl;
    glDeleteProgram(program);
    deleteShaderModules(vertModule, fragModule);
    return 0;
  }

  deleteShaderModules(vertModule, fragModule);

  return program;
}

auto Shader::compile(const std::string &filepath, unsigned int type) -> unsigned int {
  std::ifstream file(filepath);

  if (!file.is_open()) {
    std::cerr << "Failed to open shader file: " << filepath << std::endl;
    return 0;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string shaderSource = buffer.str();
  const char* shaderSrc = shaderSource.c_str();

  unsigned int shaderModule = glCreateShader(type);
  glShaderSource(shaderModule, 1, &shaderSrc, nullptr);
  glCompileShader(shaderModule);

  int success;
  glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
  if (!success) {
    char errorLog[512];
    glGetShaderInfoLog(shaderModule, 512, NULL, errorLog);
    std::cout << "Shader module compilation failed (" << filepath << "):\n" << errorLog << std::endl;
    glDeleteShader(shaderModule);
    return 0;
  }

  return shaderModule;
}

auto Shader::use() const -> void
{
  if (s_BoundID != m_RendererID) {
    glUseProgram(m_RendererID);
    s_BoundID = m_RendererID;
  }
}

auto Shader::setMat2(const std::string &name, const glm::mat2 &mat) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
}

auto Shader::setMat3(const std::string &name, const glm::mat3 &mat) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}

auto Shader::setMat4(const std::string &name, const glm::mat4 &mat) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

auto Shader::setFloat(const std::string &name, float value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform1f(location, value);
}

auto Shader::setInt(const std::string &name, int value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform1i(location, value);
}

auto Shader::setBool(const std::string &name, bool value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform1i(location, static_cast<int>(value));
}

auto Shader::setVec2(const std::string &name, const glm::vec2 &value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform2fv(location, 1, &value[0]);
}

auto Shader::setVec2(const std::string &name, float x, float y) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform2f(location, x, y);
}

auto Shader::setVec3(const std::string &name, const glm::vec3 &value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform3fv(location, 1, &value[0]);
}

auto Shader::setVec3(const std::string &name, float x, float y, float z) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform3f(location, x, y, z);
}

auto Shader::setVec4(const std::string &name, const glm::vec4 &value) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform4fv(location, 1, &value[0]);
}

auto Shader::setVec4(const std::string &name, float x, float y, float z, float w) const -> void
{
  use();
  int location = Shader::getUniformLocation(name);
  glUniform4f(location, x, y, z, w);
}

auto Shader::getUniformLocation(const std::string& name) const -> int {
  auto it = m_UniformCache.find(name);
  if (it != m_UniformCache.end()) return it->second;
  int loc = glGetUniformLocation(m_RendererID, name.c_str());
  m_UniformCache[name] = loc;
  return loc;
}

auto Shader::reload() -> void {
  std::string vertexFilePath = Paths::Shaders + m_Name + ".vert";
  std::string fragmentFilePath = Paths::Shaders + m_Name + ".frag";

  std::vector<unsigned int> modules = {
    compile(vertexFilePath, GL_VERTEX_SHADER),
    compile(fragmentFilePath, GL_FRAGMENT_SHADER)
  };

  int newProgram = createShaderProgram(modules[0], modules[1]);
  if (newProgram == 0) return;

  if (s_BoundID == m_RendererID) {
    s_BoundID = 0;
  }

  glDeleteProgram(m_RendererID);

  m_RendererID = newProgram;
  m_UniformCache.clear();
}