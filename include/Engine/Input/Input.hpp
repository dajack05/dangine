#pragma once

#include <map>

#include "Keys.hpp"

class GLFWwindow;

class Input {
public:
  static void Setup(GLFWwindow *window);
  static bool IsKeyDown(unsigned int key_code);
  static bool IsKeyJustDown(unsigned int key_code);

private:
  static void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
  static std::map<int, bool> down_keys;
  static std::map<int, bool> just_down_keys;
};