#include <Engine/Scene/Script.hpp>

#include <Engine/Config.hpp>
#include <Engine/Scripting/ScriptManager.hpp>
#include <Engine/Util/Logger.hpp>
#include <Engine/Renderer/Renderer.hpp>

#include <chaiscript.hpp>

Script::Script() : Node() {
  auto id = ScriptManager::addScript(*this);
  mContext = ScriptManager::getContext(id);
}

void Script::loadScript(const std::string &script_name) {
  if (!mContext)
    return;

  try {
    mContext->eval_file(SCRIPT_PATH + script_name + SCRIPT_EXT);
  } catch (const chaiscript::exception::eval_error &e) {
    std::string err = "Script Eval File Error: " + e.reason + "\n" + e.pretty_print();
    if(last_error.length() == 0){
      Log::Error(err);
    }
    last_error = err;
  }
}

void Script::exec(const std::string &command) {
  if (!mContext)
    return;
  try {
    mContext->eval(command);
    // mCon
  } catch (const chaiscript::exception::eval_error &e) {
    std::string err = "Script Eval File Error: " + e.reason + "\n" + e.pretty_print();
    if(last_error.length() == 0){
      Log::Error(err);
    }
    last_error = err;
  }
}

void Script::update(double delta) {
  Node::update(delta);

  if (!mContext)
    return;

  try {
    auto v = mContext->eval<std::function<void(double)>>("OnUpdate");
    v(delta);
  } catch (const chaiscript::exception::eval_error &e) {
    std::string err = "Script Eval File Error: " + e.reason + "\n" + e.pretty_print();
    if(last_error.length() == 0){
      Log::Error(err);
    }
    last_error = err;
  }
}

void Script::draw(Renderer &renderer) {
  Node::draw(renderer);

  if (!mContext)
    return;

  try {
    auto v = mContext->eval<std::function<void(Renderer & renderer)>>("OnDraw");
    v(renderer);
  } catch (const chaiscript::exception::eval_error &e) {
    std::string err = "Script Eval File Error: " + e.reason + "\n" + e.pretty_print();
    if(last_error.length() == 0){
      Log::Error(err);
    }
    last_error = err;
  }
}