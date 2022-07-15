#include <Engine/Renderer/Renderer.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <Engine/Renderer/Quad.hpp>
#include <Engine/Renderer/Shader.hpp>
#include <Engine/Renderer/Texture.hpp>
#include <Engine/Util/Logger.hpp>

const float LINE_DEPTH = -1.0F;

void Renderer::setupLines() {
  // Setup the line drawing stuff
  glGenVertexArrays(1, &lineVAO);
  glGenBuffers(1, &lineVBO);

  lineShader.load("line");
}

void Renderer::drawLine(const glm::vec2 &from, const glm::vec2 &to,
                        const glm::vec3 &color) {

  float data[6] = {
      from.x, from.y, LINE_DEPTH, to.x, to.y, LINE_DEPTH,
  };

  glBindVertexArray(lineVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

  lineShader.use();
  int apos_pos = lineShader.setVec3Attr("aPos", lineVBO);

  lineShader.setVec3("color", color);

  auto MVP = projection * view;
  lineShader.setMat4("MVP", MVP);

  glEnableVertexAttribArray(apos_pos);
  glVertexAttribPointer(apos_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void *)0);

  glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::drawRect(const glm::vec2 &from, const glm::vec2 &to,
                        const glm::vec3 &color) {
  drawLine({from.x, from.y}, {to.x, from.y}, color);
  drawLine({from.x, to.y}, {to.x, to.y}, color);
  drawLine({from.x, from.y}, {from.x, to.y}, color);
  drawLine({to.x, from.y}, {to.x, to.y}, color);
}