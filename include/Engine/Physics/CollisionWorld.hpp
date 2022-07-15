#pragma once

#include <Engine/Physics/Collision.hpp>
#include <memory>
#include <vector>

struct CollisionBody;
class Renderer;

struct DebugConfig {
  bool drawOutline = false;
  bool drawVelocityVector = false;
  bool drawNearestPointLines = false;
};

class CollisionWorld {
public:
  void addBody(CollisionBody &body);
  void addBody(std::shared_ptr<CollisionBody> &body);
  void addBody(CollisionBody *body);
  void removeBody(CollisionBody &body);
  void clearBodies();

  void tick(double delta);
  void draw();

  void setRenderer(Renderer *renderer);

  void addCollision(const Collision &collision);
  std::vector<Collision> findCollisions(CollisionBody *body);

  DebugConfig debug;

  float linear_damping = 0.0F;

private:
  std::vector<CollisionBody *> bodies;
  std::vector<Collision> collisions;

  Renderer *renderer = nullptr;
  double timer = 0.0F;

  void resolveCollision(const Collision &collision);
};