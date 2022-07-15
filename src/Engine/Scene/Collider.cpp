#include <Engine/Scene/Collider.hpp>

#include <Engine/Physics/CollisionShape.hpp>
#include <Engine/Physics/CollisionWorld.hpp>

Collider::Collider(CollisionWorld &world) {
    world.addBody(*this);
}

void Collider::update(double delta) {
    Node::update(delta);
    transform.position = origin;
}

void Collider::draw(Renderer &renderer) {
    Node::draw(renderer);
}