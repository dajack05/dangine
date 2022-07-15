#pragma once

#include <glm/vec2.hpp>

inline float flerp(float from, float to, float t) { return from + t * (to - from); }
inline int ilerp(int from, int to, float t) { return from + t * (to - from); }
inline glm::vec2 vlerp(const glm::vec2 &from, const glm::vec2 &to, const float &t) {
  return {
      flerp(from.x, to.x, t),
      flerp(from.y, to.y, t),
  };
}