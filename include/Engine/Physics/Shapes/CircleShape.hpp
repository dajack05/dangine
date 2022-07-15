#pragma once

#include<Engine/Physics/CollisionShape.hpp>

class CollisionBody;

struct CircleShape : public CollisionShape
{
  float radius = 1.0F;

  float getDistance(const glm::vec2 &from);
  glm::vec2 getNearestPoint(const glm::vec2 &from);
  void debugDraw(Renderer *renderer);
};
