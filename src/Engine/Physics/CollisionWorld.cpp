#include <Engine/Physics/CollisionWorld.hpp>

#include <Engine/Physics/CollisionBody.hpp>
#include <Engine/Physics/CollisionShape.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Util/Logger.hpp>
#include <Engine/Util/Math.hpp>

void CollisionWorld::addBody(CollisionBody *body){
  body->setWorld(this);
  if(renderer){
    body->setRenderer(renderer);
  }
  for (auto b : bodies) {
    if (b == body) {
      return;
    }
  }
  bodies.push_back(body);
}

void CollisionWorld::addBody(std::shared_ptr<CollisionBody> &body){
  addBody(body.get());
}

void CollisionWorld::addBody(CollisionBody &body) {
  addBody(&body);
}

void CollisionWorld::removeBody(CollisionBody &body) {
  for (int i = 0; i < bodies.size(); i++) {
    if (bodies[i] == &body) {
      body.setWorld(nullptr);
      bodies.erase(bodies.begin() + i);
    }
  }
}

std::vector<Collision> CollisionWorld::findCollisions(CollisionBody *body) {
  std::vector<Collision> _collisions;

  for (auto c : collisions) {
    if (c.body1 == body || c.body2 == body) {
      _collisions.push_back(c);
    }
  }

  return _collisions;
}

void CollisionWorld::clearBodies() {
  for (auto b : bodies) {
    b->setWorld(nullptr);
  }
  bodies.clear();
}

void CollisionWorld::draw(){
    for (auto body : bodies) {
      body->debugDraw();
    }
}

void CollisionWorld::tick(double delta) {
  timer += delta;
  if (timer > 1.0/100) {
    timer = 0.0;
    collisions.clear();
    for (auto body : bodies) {
      if (!body->isStatic && body->getShape() != nullptr) {
        // Apply velocity
        body->origin += body->velocity;
        body->velocity *= 1.0f - linear_damping;

        // Detection
        for (auto other : bodies) {
          if (other != body) {
            body->checkAgainst(other, *this);
          }
        }
      }
    }

    // Restitution
    for (int i = 0; i < collisions.size(); i++) {
      resolveCollision(collisions[i]);
    }
  }
}

void CollisionWorld::resolveCollision(const Collision &collision) {
  auto rel_velocity = collision.body2->velocity - collision.body1->velocity;
  auto vel_along_normal = glm::dot(collision.normal, rel_velocity);

  if (vel_along_normal <= 0.0F) {
    return;
  }

  auto dir = collision.location2 - collision.location1;
  auto dir_len = glm::length(dir);

  float e = 1.0F; // Update if I add restitution
  float impulse_scalar = -(1 / e) * vel_along_normal;

  float body1_mass_v = collision.body1->isStatic ? 0.0F : 1.0F;
  float body2_mass_v = collision.body2->isStatic ? 0.0F : 1.0F;
  impulse_scalar /= body1_mass_v + body2_mass_v;

  glm::vec2 impulse = impulse_scalar * collision.normal;

  if (!collision.body1->isStatic) {
    collision.body1->velocity -= impulse;
    collision.body1->origin += collision.normal * (dir_len / 4);
  }

  if (!collision.body2->isStatic) {
    collision.body2->velocity += impulse;
    collision.body2->origin -= collision.normal * (dir_len / 4);
  }
}

void CollisionWorld::setRenderer(Renderer *renderer) {
  this->renderer = renderer;
  for (auto body : bodies) {
    body->setRenderer(renderer);
  }
}

void CollisionWorld::addCollision(const Collision &collision) {
  collisions.push_back(collision);
}