#pragma once

#include <vector>
#include "p6/p6.h"

class Speed {
private:
    glm::vec3 dir;
    float     vel;

public:
    Speed();
    Speed(const Speed& s) = default;
    Speed(glm::vec3 direction, float velocity);
    ~Speed() = default;
    void      setDir(glm::vec3 direction);
    void      setVel(float velocity);
    glm::vec3 getDir() const;
    float     getVel() const;
};