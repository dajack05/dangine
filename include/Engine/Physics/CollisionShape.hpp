#pragma once

#include<glm/vec2.hpp>
class Renderer;
class CollisionBody;

struct CollisionShape {
  CollisionBody *body = nullptr;
  virtual glm::vec2 getNearestPoint(const glm::vec2 &from) = 0;
  virtual float getDistance(const glm::vec2 &from) = 0;
  virtual void debugDraw(Renderer *renderer) = 0;
};