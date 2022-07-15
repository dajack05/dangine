#pragma once

#include <glm/vec2.hpp>
#include <vector>

class Renderer;
struct CollisionShape;
class CollisionWorld;

class CollisionBody {
public:
  ~CollisionBody();
  glm::vec2 origin{0.0F, 0.0F};
  glm::vec2 velocity{0.0F, 0.0F};

  void applyForce(const glm::vec2 &force);

  void checkAgainst(CollisionBody &other, CollisionWorld &world);
  void checkAgainst(CollisionBody *other, CollisionWorld &world);

  void setShape(CollisionShape *shape);
  CollisionShape *getShape();
  bool isColliding();

  void setRenderer(Renderer *renderer);
  void setWorld(CollisionWorld *world);
  void debugDraw();

  bool isStatic = false;

private:
  Renderer *renderer = nullptr;
  CollisionShape *mShape = nullptr;
  CollisionWorld *world = nullptr;
};