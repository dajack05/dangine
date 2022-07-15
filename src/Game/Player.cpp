#include <Game/Player.hpp>

#include <Engine/Input/Input.hpp>
#include <Engine/Physics/Shapes/CircleShape.hpp>

Player::Player(CollisionWorld &world) : Collider(world) {
  shape.radius = 10.0F;
  setShape(&shape);

  sprite.imageScale = 16.0F;
  sprite.setImage("player");
  sprite.mRows = 5;
  sprite.mColumns = 2;
  addChild(&sprite);

  animator.addAnimation("idle", 0, 0, 8);
  animator.addAnimation("north", 4, 5, 8);
  animator.addAnimation("south", 0, 1, 8);
  animator.addAnimation("east", 6, 7, 8);
  animator.addAnimation("west", 2, 3, 8);
  animator.playForward("idle");

  sprite.addChild(&animator);

  script.loadScript("player");
  addChild(&script);
}

void Player::update(double delta) {
  Collider::update(delta);

  if (Input::IsKeyDown(KEY_SPACE)) {
    transform.angle += 0.5;
  }

  glm::vec2 move_vec{0.0F, 0.0F};

  if (Input::IsKeyDown(KEY_W)) {
    move_vec.y += 1;
  }
  if (Input::IsKeyDown(KEY_S)) {
    move_vec.y -= 1;
  }
  if (Input::IsKeyDown(KEY_A)) {
    move_vec.x -= 1;
  }
  if (Input::IsKeyDown(KEY_D)) {
    move_vec.x += 1;
  }

  if (glm::length(move_vec) > 0.0F) {
    move_vec = glm::normalize(move_vec) * move_speed;
  }

  if (std::abs(move_vec.x) < std::abs(move_vec.y)) {
    if (move_vec.y > 0) {
      animator.playForward("north");
    } else {
      animator.playForward("south");
    }
  } else if (glm::length(move_vec) < 0.1F) {
    animator.playForward("idle");
  } else {
    if (move_vec.x > 0) {
      animator.playForward("east");
    } else {
      animator.playForward("west");
    }
  }

  applyForce(move_vec * (float)delta);
}