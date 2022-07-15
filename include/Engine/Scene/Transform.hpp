#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/glm.hpp>

struct Transform {
  glm::vec2 position{0.0F, 0.0F};
  glm::vec2 scale{1.0F, 1.0F};
  float angle = 0.0F;

  inline glm::mat4 getMatrix() {
    glm::mat4 mat = glm::mat4(1.0F);
    mat = glm::translate(mat, glm::round(glm::vec3(position, 0.0f)));
    mat = glm::rotate(mat, glm::radians((float)(round(angle / 10.0F)) * 10.0F), glm::vec3{0, 0, 1});
    mat = glm::scale(mat, glm::round(glm::vec3(scale, 1.0F)));
    return mat;
  }

  inline Transform operator*(Transform &other) {
    auto mat1 = getMatrix();
    auto mat2 = other.getMatrix();
    auto sum = mat1 * mat2;
    return Transform::FromMatrix(sum);
  }

  static Transform FromMatrix(const glm::mat4 matrix) {
    Transform t;

    glm::vec3 scale;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, scale, rotation, position, skew, perspective);

    glm::vec3 angles = glm::eulerAngles(rotation);

    t.scale = {scale.x, scale.y};
    t.position = {position.x, position.y};
    t.angle = glm::degrees(angles.z);

    return t;
  }
};