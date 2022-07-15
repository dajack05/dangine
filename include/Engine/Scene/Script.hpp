#pragma once

#include <Engine/Scene/Node.hpp>
#include <Engine/Scripting/ScriptManager.hpp>
#include <string>

class Script : public Node {
public:
  Script();

  void loadScript(const std::string &script_name);
  void exec(const std::string &command);

  void update(double delta);
  void draw(Renderer &renderer);

private:
  chaiscript::ChaiScript *mContext = nullptr;
  std::string last_error;
};