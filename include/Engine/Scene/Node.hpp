#pragma once

#include <glm/vec2.hpp>

#include <Engine/Scene/Transform.hpp>

#include <string>
#include <vector>

class Renderer;

class Node {
public:
  Transform transform;
  Transform globalTransform;

  virtual void update(double delta);
  virtual void draw(Renderer &renderer);

  void addChild(Node *child);

  template <class T> std::vector<T *> getChildrenOfType() {
    std::vector<T *> data;
    for (auto child : children) {
      auto asT = dynamic_cast<T *>(child);
      if (asT) {
        data.push_back(asT);
      }
    }
    return data;
  }

  inline Node *getParent() { return parent; }

protected:
  Node *parent = nullptr;
  std::vector<Node *> children;
};