#pragma once

#include <glm/fwd.hpp>
#include <string>

class Shader {
public:
  void load(const std::string &name);
  void use();

  int setMat4(const std::string &name, glm::mat4 &matrix);

  int setInt(const std::string &name, const int &value);
  int setFloat(const std::string &name, const float &value);

  int setVec2(const std::string &name, const glm::vec2 &vector);
  int setVec3(const std::string &name, const glm::vec3 &vector);
  int setVec4(const std::string &name, const glm::vec4 &vector);

  int setVec3Attr(const std::string &name, unsigned int glBuffer);
  int setVec2Attr(const std::string &name, unsigned int glBuffer);

  inline unsigned int getProgram() { return program; }

  static void Unbind();

  ~Shader();

  bool isFirstRun = true;

private:
  unsigned int program = 0;
};