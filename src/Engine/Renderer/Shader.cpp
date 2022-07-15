#include <Engine/Renderer/Shader.hpp>

#include <Engine/Config.hpp>
#include <Engine/Util/Logger.hpp>

#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>

void Shader::load(const std::string &name) {
  const auto fragment_path = SHADER_PATH + name + ".fs";
  const auto vertex_path = SHADER_PATH + name + ".vs";

  std::string vertex_src;
  std::string fragment_src;

  // Frag text
  std::ifstream file;
  file.open(fragment_path);

  if (!file) {
    Log::Error("Failed to open " + fragment_path);
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    fragment_src = fragment_src + line + "\n";
  }
  file.close();

  // Vert text
  file.open(vertex_path);
  if (!file) {
    Log::Error("Failed to open" + vertex_path);
    return;
  }

  while (std::getline(file, line)) {
    vertex_src = vertex_src + line + "\n";
  }
  file.close();

  // Ready to compile shader
  program = glCreateProgram();

  auto fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *real_fragment_src = fragment_src.c_str();
  glShaderSource(fragShader, 1, &real_fragment_src, nullptr);
  glCompileShader(fragShader);

  {
    GLint params;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE) {
      Log::Error("Failed to compile fragment shader");

      GLchar info[256];
      glGetShaderInfoLog(fragShader, 256, nullptr, info);
      Log::Error(info);
      // return;
    }
  }

  glAttachShader(program, fragShader);

  auto vertShader = glCreateShader(GL_VERTEX_SHADER);
  const char *real_vertex_src = vertex_src.c_str();
  glShaderSource(vertShader, 1, &real_vertex_src, nullptr);
  glCompileShader(vertShader);

  {
    GLint params;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE) {
      Log::Error("Failed to compile vertex shader");

      GLchar info[256];
      glGetShaderInfoLog(vertShader, 256, nullptr, info);
      Log::Error(info);
      // return;
    }
  }

  glAttachShader(program, vertShader);

  // Compile
  glLinkProgram(program);

  // Cleanup
  glDeleteShader(fragShader);
  glDeleteShader(vertShader);

  Log::Debug("Loaded Shader: " + name);
}

int Shader::setMat4(const std::string &name, glm::mat4 &matrix) {
  auto location = glGetUniformLocation(program, name.c_str());
  if (location >= 0) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
  }
  return location;
}

int Shader::setInt(const std::string &name, const int &value) {
  auto location = glGetUniformLocation(program, name.c_str());
  if (location >= 0) {
    glUniform1iv(location, 1, &value);
  }
  return location;
}

int Shader::setFloat(const std::string &name, const float &value) {
  auto location = glGetUniformLocation(program, name.c_str());
  if (location >= 0) {
    glUniform1fv(location, 1, &value);
  }
  return location;
}

int Shader::setVec2(const std::string &name, const glm::vec2 &vector) {
  auto location = glGetUniformLocation(program, name.c_str());
  if (location >= 0) {
    glUniform2fv(location, 1, &vector[0]);
  }
  return location;
}

int Shader::setVec3(const std::string &name, const glm::vec3 &vector) {
  auto location = glGetUniformLocation(program, name.c_str());
  if (location >= 0) {
    glUniform3fv(location, 1, &vector[0]);
  }
  return location;
}

int Shader::setVec4(const std::string &name, const glm::vec4 &vector) {
  auto location = glGetUniformLocation(program, name.c_str());
  if (location >= 0) {
    glUniform4fv(location, 1, &vector[0]);
  }
  return location;
}

int Shader::setVec3Attr(const std::string &name, unsigned int glBuffer) {
  auto attr_location = glGetAttribLocation(program, name.c_str());
  if (attr_location >= 0) {
    glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
    glEnableVertexAttribArray(attr_location);
    glVertexAttribPointer(attr_location, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(GLfloat), (void *)0);
  }
  return attr_location;
}

int Shader::setVec2Attr(const std::string &name, unsigned int glBuffer) {
  auto attr_location = glGetAttribLocation(program, name.c_str());
  if (attr_location >= 0) {
    glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
    glEnableVertexAttribArray(attr_location);
    glVertexAttribPointer(attr_location, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(GLfloat), (void *)0);
  }
  return attr_location;
}

void Shader::use() { glUseProgram(program); }

Shader::~Shader() {
  if (program > 0) {
    glDeleteProgram(program);
  }
}

void Shader::Unbind() { glUseProgram(0); }