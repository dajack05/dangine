#pragma once

#include <map>
#include <memory>
#include <string>

class Node;

namespace chaiscript{
  class ChaiScript;
}

#define null_script 0
typedef unsigned int ScriptID;

class Script;

class ScriptManager {
public:
  static ScriptID addScript(Script &node);
  static chaiscript::ChaiScript *getContext(const ScriptID &id);
  static void setRootNode(Node &node);

private:
  static Node* root;
  static ScriptID nextID;
  static std::map<ScriptID, std::shared_ptr<chaiscript::ChaiScript>> contexts;

  static ScriptID addScript();
  
  static void setupScript(const ScriptID &id, Script &node);
};