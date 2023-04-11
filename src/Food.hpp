#pragma once

#include <vector>
#include "p6/p6.h"

class Food {
private:
    glm::vec2 _pos;
    float     _radius;

public:
    Food();
    Food(glm::vec2 position, float rad);
    ~Food() = default;
    void      draw(p6::Context& ctx) const;
    glm::vec2 getPos() const;
    float     getRadius() const;
};