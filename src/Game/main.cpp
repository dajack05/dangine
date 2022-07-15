#include <Engine/Input/Input.hpp>
#include <Engine/Physics/CollisionBody.hpp>
#include <Engine/Physics/CollisionWorld.hpp>
#include <Engine/Physics/Shapes/CircleShape.hpp>
#include <Engine/Physics/Shapes/RectShape.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Scene/Collider.hpp>
#include <Engine/Scene/Node.hpp>
#include <Engine/Scene/Script.hpp>
#include <Engine/Scene/Sprite.hpp>
#include <Engine/Scene/TileMap.hpp>
#include <Engine/Util/Logger.hpp>
#include <Engine/Util/Math.hpp>

#include <Game/Player.hpp>

#include <memory>

#define GB_WIDTH 160
#define GB_HEIGHT 144

bool should_run = true;

int main(int argc, char **args) {
  Renderer renderer(GB_WIDTH * 4, GB_HEIGHT * 4);
  renderer.setFramebufferSize(GB_WIDTH, GB_HEIGHT);

  CollisionWorld world;
  world.setRenderer(&renderer);
  world.debug = {
      true,  // drawOutline
      true,  // drawVelocityVector
      false, // drawNearestPointLines
  };

  if (!renderer.ready()) {
    return 0;
  }

  Node root;

  TileMap map;
  root.addChild(&map);
  map.load("testmap2", world);

  Player player(world);
  root.addChild(&player);

  glm::vec2 grav{0.0F, -0.1F};
  world.linear_damping = 0.2F;

  while (!renderer.didWindowClose() && !Input::IsKeyDown(KEY_ESCAPE)) {
    renderer.clear();

    root.update(renderer.getDelta());

    glm::vec2 cam_player_delta =
        renderer.mCameraLocation - player.globalTransform.position;
    float dist_to_player =
        fmax(abs(cam_player_delta.x), abs(cam_player_delta.y));
    if (dist_to_player > 20.0F) {
      renderer.mCameraLocation = glm::round(
          vlerp(renderer.mCameraLocation, player.globalTransform.position,
                renderer.getDelta() * 2));
    }

    renderer.update();

    world.tick(renderer.getDelta());

    // Draw stuff
    root.draw(renderer);

    world.draw();

    renderer.present();
  }

  return 0;
}