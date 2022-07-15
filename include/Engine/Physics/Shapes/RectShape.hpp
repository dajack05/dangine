#pragma once

#include <Engine/Physics/CollisionShape.hpp>

class CollisionBody;

struct RectShape : public CollisionShape {
  glm::vec2 extents{1.0f, 1.0f};

  float getDistance(const glm::vec2 &from);
  glm::vec2 getNearestPoint(const glm::vec2 &from);
  void debugDraw(Renderer *renderer);
};
