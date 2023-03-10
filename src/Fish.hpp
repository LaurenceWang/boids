#pragma once

#include <vector>
#include "p6/p6.h"

class Fish {
private:
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 dir;
    float     size;

public:
    Fish();
    Fish(const Fish& f);
    Fish(glm::vec2 position, glm::vec2 velocity, glm::vec2 direction, float size);
    ~Fish() = default;

    void drawFish(p6::Context& context);
    void updatePosition(glm::vec2 position);
    void move();
};