#pragma once

#include <vector>
#include "p6/p6.h"

// rename m_attribut
class Fish {
private:
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 dir;
    float     size;

public:
    Fish();
    Fish(const Fish& f) = default;
    Fish(glm::vec2 position, glm::vec2 velocity, glm::vec2 direction, float size);
    ~Fish() = default;

    void      drawFish(p6::Context& context) const;
    void      updatePosition(glm::vec2 position);
    void      move();
    double    getDistance(Fish& f) const;
    glm::vec2 getAwayDir(Fish& f) const;
    void      steerAway(glm::vec2 awayDir);
};