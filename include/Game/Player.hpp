#pragma once

#include <Engine/Scene/Collider.hpp>
#include <Engine/Scene/Sprite.hpp>
#include <Engine/Scene/Animator.hpp>
#include <Engine/Scene/Script.hpp>
#include <Engine/Physics/Shapes/CircleShape.hpp>
#include <memory>

class Player : public Collider {
public:
  Player(CollisionWorld &world);

  void update(double delta);

  float move_speed = 10.0F;

private:
  CircleShape shape;
  Sprite sprite;
  Animator animator;
  Script script;
};