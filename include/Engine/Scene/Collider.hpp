#pragma once

#include <Engine/Scene/Node.hpp>
#include <Engine/Physics/CollisionBody.hpp>

class CollisionWorld;
class CollisionShape;

class Collider : public Node, public CollisionBody {
public:
  Collider(CollisionWorld &world);

  void update(double delta);
  void draw(Renderer &renderer);
};