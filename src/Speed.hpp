#pragma once

#include <vector>
#include "p6/p6.h"

class Speed {
private:
    glm::vec2 dir;
    float     vel;

public:
    Speed();
    Speed(const Speed& s) = default;
    Speed(glm::vec2 direction, float velocity);
    ~Speed() = default;
    void      setDir(glm::vec2 direction);
    void      setVel(float velocity);
    glm::vec2 getDir() const;
    float     getVel() const;
};