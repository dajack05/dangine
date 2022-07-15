#include <Engine/Scripting/Register.hpp>

#include <Engine/Renderer/Renderer.hpp>

#include <chaiscript.hpp>

#define REG_MEMBER(class, member)                                              \
  context->add(chaiscript::fun(&class ::member), #member)

void RegisterRenderer(chaiscript::ChaiScript *context) {
  context->add(chaiscript::user_type<Renderer>(), "Renderer");
  REG_MEMBER(Renderer, drawLine);
  REG_MEMBER(Renderer, drawRect);
  REG_MEMBER(Renderer, getDelta);
  context->add(chaiscript::var(&Renderer::mCameraScale), "cameraScale");
  context->add(chaiscript::var(&Renderer::mCameraLocation), "cameraLocation");
}