#pragma once
#include "camera/FreeflyCamera.hpp"
#include "p6/p6.h"

class Arpenteur {
private:
    glm::vec3 _position;
    float     _direction;

public:
    Arpenteur() = default;
    void      updatePosition(FreeflyCamera ViewMatrixCamera);
    glm::vec3 getPos() { return _position; }
    float     getDir() { return _direction; }
};