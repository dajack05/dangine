#include <Engine/Input/Input.hpp>
#include <Engine/Input/Keys.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Scene/Node.hpp>
#include <Engine/Renderer/Shader.hpp>
#include <Engine/Scene/Script.hpp>
#include <Engine/Scene/Sprite.hpp>
#include <Engine/Util/Logger.hpp>

#define WIDTH 8 * 10
#define HEIGHT 8 * 20
#define SCALE 4

#define DEBUG

int main(int argc, char **argv) {

  Renderer renderer("Tetrus", WIDTH * SCALE, HEIGHT * SCALE);
  renderer.setFramebufferSize(WIDTH, HEIGHT);
  renderer.mCameraLocation = {36, -76};

  Node root;

  ScriptManager::setRootNode(root);

  Script main_script;
  main_script.loadScript("Tetris/Main");
  root.addChild(&main_script);

  while (!renderer.didWindowClose() && !Input::IsKeyDown(KEY_ESCAPE)) {

    renderer.update();

    root.update(renderer.getDelta());

    renderer.clear();

    root.draw(renderer);

    renderer.present();
  }

  return 0;
}