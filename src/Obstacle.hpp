#pragma once

#include <vector>
#include "p6/p6.h"

class Obstacle {
private:
    glm::vec2 _pos;
    float     _radius;

public:
    Obstacle();
    Obstacle(glm::vec2 position, float rad);
    void      draw(p6::Context& ctx) const;
    float     getRadius() const;
    void      updateBorderPosition(p6::Context& ctx, int i);
    glm::vec2 getPos() const;
};