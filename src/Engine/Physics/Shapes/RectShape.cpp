#include <Engine/Physics/Shapes/RectShape.hpp>

#include <Engine/Physics/CollisionBody.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Util/Logger.hpp>

#include <glm/glm.hpp>

float clamp(float input, float min, float max) {
  if (input > max) {
    return max;
  }

  if (input < min) {
    return min;
  }

  return input;
}

float RectShape::getDistance(const glm::vec2 &from) {
  auto p = from - body->origin;
  auto b = extents;

  auto d = glm::abs(p) - b;
  auto outside_len = glm::length(glm::max(d, glm::vec2{0.0, 0.0}));
  auto inside_len = fmin(fmax(d.x, d.y), 0);

  return outside_len + inside_len;
}

glm::vec2 RectShape::getNearestPoint(const glm::vec2 &from) {

  glm::vec2 closestPoint =
      glm::max(body->origin - extents, glm::min(from, body->origin + extents));

  return closestPoint;
}

void RectShape::debugDraw(Renderer *renderer) {
  glm::vec3 color{1, 0, 0};

  if (body->isColliding()) {
    color = {0, 1, 0};
  }

  auto TL = body->origin + (extents * glm::vec2{+1, +1});
  auto TR = body->origin + (extents * glm::vec2{-1, +1});
  auto BL = body->origin + (extents * glm::vec2{+1, -1});
  auto BR = body->origin + (extents * glm::vec2{-1, -1});

  renderer->drawLine(TL, TR, color);
  renderer->drawLine(TR, BR, color);
  renderer->drawLine(BR, BL, color);
  renderer->drawLine(BL, TL, color);
}
