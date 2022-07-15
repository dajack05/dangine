#include <Engine/Scripting/ScriptManager.hpp>

#include <chaiscript.hpp>

#include <Engine/Input/Input.hpp>
#include <Engine/Scene/Script.hpp>
#include <Engine/Scripting/Register.hpp>
#include <Engine/Util/Logger.hpp>

Node* ScriptManager::root = nullptr;

std::map<ScriptID, std::shared_ptr<chaiscript::ChaiScript>>
    ScriptManager::contexts;
ScriptID ScriptManager::nextID = 1;

ScriptID ScriptManager::addScript() {
  const ScriptID id = nextID++;
  Log::Debug("Adding Script with ID: ", id);
  contexts.insert(
      std::make_pair(id, std::make_shared<chaiscript::ChaiScript>()));
  return id;
}

ScriptID ScriptManager::addScript(Script &node) {
  const ScriptID id = addScript();

  setupScript(id, node);

  return id;
}

chaiscript::ChaiScript *ScriptManager::getContext(const ScriptID &id) {
  if (id == null_script) {
    Log::Error("Attempted to get context with null id");
    return nullptr;
  }

  auto result = contexts.find(id);
  if (result == contexts.end()) {
    Log::Error("Attempted to get context with id that does not exist: ", id);
    return nullptr;
  }

  return result->second.get();
}

void ScriptManager::setupScript(const ScriptID &id, Script &node) {
  auto context = getContext(id);
  if (!context)
    return;

  context->add(chaiscript::user_type<Input>(), "Input");
  context->register_namespace(
      [](chaiscript::Namespace &input) {
        input["IsKeyDown"] = chaiscript::var(chaiscript::fun(
            [](const int key) { return Input::IsKeyDown(key); }));
        input["IsKeyJustDown"] = chaiscript::var(chaiscript::fun(
            [](const int key) { return Input::IsKeyJustDown(key); }));
      },
      "Input");

  context->register_namespace(
      [](chaiscript::Namespace &system) {
        system["rand"] =
            chaiscript::var(chaiscript::fun([]() { return std::rand(); }));
        system["randSeed"] = chaiscript::var(
            chaiscript::fun([]() { return std::srand(std::time(nullptr)); }));
      },
      "System");

  RegisterTypes(context);

  if(root){
    Log::Debug("Adding `Root`");
    context->add_global(chaiscript::var(root), "root");
  }
}

void ScriptManager::setRootNode(Node &node) {
  root = &node;
  // for (auto &pair : contexts) {
  //   pair.second->set_global(chaiscript::var(root), "root");
  // }
}