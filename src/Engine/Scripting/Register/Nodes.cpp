#include <Engine/Scripting/Register.hpp>

#include <Engine/Scene/Animator.hpp>
#include <Engine/Scene/Collider.hpp>
#include <Engine/Scene/Node.hpp>
#include <Engine/Scene/Script.hpp>
#include <Engine/Scene/Sprite.hpp>
#include <Engine/Scene/TileMap.hpp>
#include <Engine/Scene/Transform.hpp>
#include <Engine/Renderer/Renderer.hpp>

#include <chaiscript.hpp>

#define REG_MEMBER(class, member)                                              \
  context->add(chaiscript::fun(&class::member), #member)

void RegisterNodes(chaiscript::ChaiScript *context) {
  context->add(chaiscript::user_type<Node>(), "Node");
  context->add(chaiscript::constructor<Node()>(), "Node");
  REG_MEMBER(Node, transform);
  REG_MEMBER(Node, globalTransform);
  REG_MEMBER(Node, getParent);
  REG_MEMBER(Node, addChild);
  
  context->add(chaiscript::fun(&Node::update), "update");
  context->add(chaiscript::fun(&Node::draw), "draw");

  context->add(chaiscript::user_type<Script>(), "Script");
  context->add(chaiscript::base_class<Node, Script>());

  context->add(chaiscript::user_type<Sprite>(), "Sprite");
  context->add(chaiscript::base_class<Node, Sprite>());
  context->add(chaiscript::constructor<Sprite()>(), "Sprite");
  REG_MEMBER(Sprite, imageScale);
  REG_MEMBER(Sprite, uv);
  REG_MEMBER(Sprite, mColumns);
  REG_MEMBER(Sprite, mRows);
  REG_MEMBER(Sprite, tint);
  REG_MEMBER(Sprite, visible);
  REG_MEMBER(Sprite, getTexture);

  context->add(chaiscript::user_type<Transform>(), "Transform");
  REG_MEMBER(Transform, position);
  REG_MEMBER(Transform, angle);
  REG_MEMBER(Transform, scale);
}