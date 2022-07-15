#include <Engine/Physics/Shapes/CircleShape.hpp>

#include <Engine/Physics/CollisionBody.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Util/Logger.hpp>

#include <glm/glm.hpp>

#define DEBUG_SEGMENTS 8

float CircleShape::getDistance(const glm::vec2 &from) {
  return glm::length(from - body->origin) - radius;
}

glm::vec2 CircleShape::getNearestPoint(const glm::vec2 &from) {
  glm::vec2 dir = glm::normalize(from - body->origin);
  glm::vec2 dir_radius = dir * radius;
  glm::vec2 point = body->origin + dir_radius;
  return point;
}

void CircleShape::debugDraw(Renderer *renderer) {
  glm::vec3 color{1, 0, 0};

  if (body->isColliding()) {
    color = {0, 1, 0};
  }

  glm::vec2 last{sin(0) * radius, cos(0) * radius};
  for (auto i = 0; i < DEBUG_SEGMENTS; i++) {
    glm::vec2 loc{sin(glm::radians(360.0F / DEBUG_SEGMENTS) * (i + 1)) * radius,
                  cos(glm::radians(360.0F / DEBUG_SEGMENTS) * (i + 1)) *
                      radius};
    renderer->drawLine(last + body->origin, loc + body->origin, color);
    last = loc;
  }
}