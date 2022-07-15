#include <Engine/Input/Input.hpp>

#include <Engine/Util/Logger.hpp>
#include <GLFW/glfw3.h>

std::map<int, bool> Input::down_keys = std::map<int, bool>();
std::map<int, bool> Input::just_down_keys = std::map<int, bool>();

void Input::OnKeyEvent(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {
  switch (action) {
  case GLFW_PRESS:
    Input::down_keys[key] = true;
    Input::just_down_keys[key] = true;
    break;

  case GLFW_RELEASE:
    Input::down_keys[key] = false;
    Input::just_down_keys[key] = false;
    break;

  case GLFW_REPEAT:
    Input::down_keys[key] = true;
    break;

  default:
    break;
  }
}

void Input::Setup(GLFWwindow *window) {
  glfwSetKeyCallback(window, Input::OnKeyEvent);
}

bool Input::IsKeyJustDown(unsigned int key_code) {
  if (just_down_keys.find(key_code) == just_down_keys.end()) {
    return false;
  }
  bool value = just_down_keys.at(key_code);
  just_down_keys[key_code] = false;
  return value;
}

bool Input::IsKeyDown(unsigned int key_code) {
  if (down_keys.find(key_code) == down_keys.end()) {
    return false;
  }
  return down_keys.at(key_code);
}