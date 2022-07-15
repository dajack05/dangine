#include <Engine/Scripting/Register.hpp>

#include <Engine/Renderer/Texture.hpp>
#include <glm/glm.hpp>

#include <chaiscript.hpp>

void RegisterTexture(chaiscript::ChaiScript *context) {
  context->add(chaiscript::user_type<Texture>(), "Texture");
  context->add(chaiscript::fun(&Texture::load), "load");
  context->add(chaiscript::fun(&Texture::getSize), "getSize");
}