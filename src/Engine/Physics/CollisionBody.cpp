#include <Engine/Physics/CollisionBody.hpp>

#include <Engine/Physics/Collision.hpp>
#include <Engine/Physics/CollisionShape.hpp>
#include <Engine/Physics/CollisionWorld.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Util/Logger.hpp>

#include <glm/glm.hpp>

void CollisionBody::checkAgainst(CollisionBody &other, CollisionWorld &world) {
  checkAgainst(&other, world);
}

void CollisionBody::checkAgainst(CollisionBody *other, CollisionWorld &world) {
  if (mShape && other->mShape) {
    auto point_on_other = other->mShape->getNearestPoint(origin);
    auto point_on_self = mShape->getNearestPoint(point_on_other);

    auto is_within = other->mShape->getDistance(point_on_self) < 0;

    if (is_within) {
      glm::vec2 normal = glm::normalize(point_on_other - point_on_self);
      Collision c = {normal, point_on_self, this, point_on_other, other};
      world.addCollision(c);
    }

    if (world.debug.drawNearestPointLines) {
      if (renderer) {
        if (!is_within) {
          renderer->drawLine(point_on_self, point_on_other, {0, 1, 0});
        }
      }
    }
  }
}

void CollisionBody::setShape(CollisionShape *shape) {
  mShape = shape;
  mShape->body = this;
}

CollisionShape *CollisionBody::getShape() { return mShape; }

void CollisionBody::setRenderer(Renderer *renderer) {
  this->renderer = renderer;
}

void CollisionBody::setWorld(CollisionWorld *world) { this->world = world; }

void CollisionBody::applyForce(const glm::vec2 &force) {
  // todo: take mass into consideration
  velocity += force;
}

CollisionBody::~CollisionBody() {
  renderer = nullptr;
  mShape = nullptr;
}

void CollisionBody::debugDraw() {
  if (world->debug.drawOutline) {
    if (renderer) {
      mShape->debugDraw(renderer);
    } else {
      Log::Error("No renderer attached to world");
    }
  }

  if (world->debug.drawVelocityVector) {
    if (renderer) {
      renderer->drawLine(origin, origin + velocity * 50.0F, {1, 0, 1});
    } else {
      Log::Error("No renderer attached to world");
    }
  }
}

bool CollisionBody::isColliding() {
  if (!world) {
    return false;
  }

  auto collisions = world->findCollisions(this);
  return collisions.size() > 0;
}