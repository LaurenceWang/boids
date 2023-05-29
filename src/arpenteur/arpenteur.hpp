#pragma once
#include "glimac/FreeflyCamera.hpp"
#include "p6/p6.h"

class Arpenteur {
private:
    glm::vec3 _position;

public:
    Arpenteur() = default;
    // Arpenteur(glm::vec3 position);
    void      updatePosition(FreeflyCamera ViewMatrixCamera, glm::vec3 mapDimension);
    glm::vec3 getPos() { return _position; }
};