#pragma once

#include <Engine/Renderer/Shader.hpp>
#include <Engine/Renderer/Texture.hpp>
#include <Engine/Scene/Node.hpp>

#include <string>

class Renderer;

class Sprite : public Node {
public:
  glm::vec4 uv{0.0f, 0.0f, 1.0f, 1.0f};
  int mColumns = 1;
  int mRows = 1;
  float imageScale = 1.0F;
  glm::vec3 tint{1.0, 1.0, 1.0};
  bool visible = true;

  Sprite();

  void setShader(const std::string &filename);
  void setImage(const std::string &filename);

  void update(double delta);
  void draw(Renderer &renderer);

  inline Shader &getShader() { return this->shader; }
  inline Texture &getTexture() { return this->texture; }

private:
  Shader shader;
  Texture texture;
};