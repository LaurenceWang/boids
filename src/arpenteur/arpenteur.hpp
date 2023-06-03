#pragma once
#include "camera_test/TrackballCamera.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "p6/p6.h"

class Arpenteur {
private:
    glm::vec3 _position;
    float     _direction;

public:
    Arpenteur() = default;
    // Arpenteur(glm::vec3 position);
    void updatePosition(FreeflyCamera ViewMatrixCamera);
    void updatePosition2(TrackballCamera ViewMatrixCamera);

    glm::vec3 getPos() { return _position; }
    float     getDir() { return _direction; }
};