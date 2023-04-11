#pragma once

#include <vector>
#include "p6/p6.h"

class Obstacle {
private:
    glm::vec2 _pos;
    float     _radius;
    bool      _isBorderX;
    bool      _isBorderY;

public:
    Obstacle();
    Obstacle(glm::vec2 position, float rad);
    Obstacle(glm::vec2 position, float rad, bool borderX, bool borderY);
    ~Obstacle() = default;
    void      draw(p6::Context& ctx) const;
    float     getRadius() const;
    void      updateBorderY(p6::Context& ctx, int nb);
    void      updateBorderX(p6::Context& ctx);
    void      updateBorderPosition(p6::Context& ctx, int i);
    glm::vec2 getPos() const;
};