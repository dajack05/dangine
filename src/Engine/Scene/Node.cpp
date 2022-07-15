#include <Engine/Scene/Node.hpp>

#include <Engine/Util/Logger.hpp>
#include <Engine/Renderer/Renderer.hpp>

void Node::addChild(Node *child) {
  for(auto c : children){
    if(c == child){
        Log::Debug("Child already exists");
        return;
    }
  }
  child->parent = this;
  children.push_back(child);
}

void Node::update(double delta){
    // Global transform
    if(parent){
        globalTransform = parent->globalTransform * transform;
    }

    // Propogate
    for(auto child : children){
        if(child){
            child->update(delta);
        }
    }
}

void Node::draw(Renderer &renderer){
    for(auto child : children){
        if(child){
            child->draw(renderer);
        }
    }
}