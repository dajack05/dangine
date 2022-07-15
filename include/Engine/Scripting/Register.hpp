#pragma once

namespace chaiscript {
class ChaiScript;
}

void RegisterGLM(chaiscript::ChaiScript *context);
void RegisterKeyCodes(chaiscript::ChaiScript *context);
void RegisterMath(chaiscript::ChaiScript *context);
void RegisterNodes(chaiscript::ChaiScript *context);
void RegisterRenderer(chaiscript::ChaiScript *context);
void RegisterTexture(chaiscript::ChaiScript *context);

inline void RegisterTypes(chaiscript::ChaiScript *context) {
  RegisterGLM(context);
  RegisterKeyCodes(context);
  RegisterMath(context);
  RegisterRenderer(context);
  RegisterTexture(context);
  RegisterNodes(context);
}