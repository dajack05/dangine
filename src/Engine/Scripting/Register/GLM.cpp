#include <Engine/Scripting/Register.hpp>

#include <chaiscript.hpp>
#include <glm/glm.hpp>

#define OPERATOR(type, operator)                                               \
  context->add(chaiscript::fun([](type &a, type &b) { return a operator b; }), \
               #operator);

void RegisterVec2(chaiscript::ChaiScript *context) {
  context->add(chaiscript::user_type<glm::vec2>(), "Vec2");

  context->add(chaiscript::constructor<glm::vec2()>(), "Vec2");
  context->add(chaiscript::constructor<glm::vec2(glm::vec2 &)>(), "Vec2");
  context->add(chaiscript::constructor<glm::vec2(float scalar)>(), "Vec2");
  context->add(chaiscript::constructor<glm::vec2(float x, float y)>(), "Vec2");

  context->add(chaiscript::fun(&glm::vec2::x), "x");
  context->add(chaiscript::fun(&glm::vec2::y), "y");

  OPERATOR(glm::vec2, +);
  OPERATOR(glm::vec2, -);
  OPERATOR(glm::vec2, *);
  OPERATOR(glm::vec2, /);
  context->add(chaiscript::fun([](glm::vec2 &a, glm::vec2 &b) { a = b; }), "=");
}

void RegisterVec3(chaiscript::ChaiScript *context) {
  context->add(chaiscript::user_type<glm::vec3>(), "Vec3");

  context->add(chaiscript::constructor<glm::vec3()>(), "Vec3");
  context->add(chaiscript::constructor<glm::vec3(glm::vec3 &)>(), "Vec3");
  context->add(chaiscript::constructor<glm::vec3(float scalar)>(), "Vec3");
  context->add(chaiscript::constructor<glm::vec3(float x, float y, float z)>(),
               "Vec3");

  context->add(chaiscript::fun(&glm::vec3::x), "x");
  context->add(chaiscript::fun(&glm::vec3::y), "y");
  context->add(chaiscript::fun(&glm::vec3::z), "z");

  OPERATOR(glm::vec3, +);
  OPERATOR(glm::vec3, -);
  OPERATOR(glm::vec3, *);
  OPERATOR(glm::vec3, /);
  context->add(chaiscript::fun([](glm::vec3 &a, glm::vec3 &b) { a = b; }), "=");
}

void RegisterVec4(chaiscript::ChaiScript *context) {
  context->add(chaiscript::user_type<glm::vec4>(), "Vec4");

  context->add(chaiscript::constructor<glm::vec4(glm::vec4 &)>(), "Vec4");
  context->add(chaiscript::constructor<glm::vec4()>(), "Vec4");
  context->add(chaiscript::constructor<glm::vec4(float scalar)>(), "Vec4");
  context->add(
      chaiscript::constructor<glm::vec4(float x, float y, float z, float w)>(),
      "Vec4");

  OPERATOR(glm::vec4, +);
  OPERATOR(glm::vec4, -);
  OPERATOR(glm::vec4, *);
  OPERATOR(glm::vec4, /);
  context->add(chaiscript::fun([](glm::vec4 &a, glm::vec4 &b) { a = b; }), "=");

  context->add(chaiscript::fun(&glm::vec4::x), "x");
  context->add(chaiscript::fun(&glm::vec4::y), "y");
  context->add(chaiscript::fun(&glm::vec4::z), "z");
  context->add(chaiscript::fun(&glm::vec4::w), "w");
}

void RegisterColor(chaiscript::ChaiScript *context) {
  context->add(chaiscript::user_type<glm::vec3>(), "Color");

  context->add(chaiscript::constructor<glm::vec3()>(), "Color");
  context->add(chaiscript::constructor<glm::vec3(glm::vec3 &)>(), "Color");
  context->add(chaiscript::constructor<glm::vec3(float r, float g, float b)>(),
               "Color");

  context->add(chaiscript::fun(&glm::vec3::r), "r");
  context->add(chaiscript::fun(&glm::vec3::g), "g");
  context->add(chaiscript::fun(&glm::vec3::b), "b");

  OPERATOR(glm::vec3, +);
  OPERATOR(glm::vec3, -);
  OPERATOR(glm::vec3, *);
  OPERATOR(glm::vec3, /);
  context->add(chaiscript::fun([](glm::vec3 &a, glm::vec3 &b) { a = b; }), "=");
}

void RegisterGLM(chaiscript::ChaiScript *context) {
  RegisterVec2(context);
  RegisterVec3(context);
  RegisterVec4(context);
  RegisterColor(context);
}