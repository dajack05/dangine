#pragma once

#include<glm/vec2.hpp>

class CollisionBody;

struct Collision{
    glm::vec2 normal;

    glm::vec2 location1;
    CollisionBody *body1;

    glm::vec2 location2;
    CollisionBody *body2;
};