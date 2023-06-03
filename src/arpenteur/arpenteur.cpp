#include "arpenteur.hpp"

void Arpenteur::updatePosition(FreeflyCamera ViewMatrixCamera)
{
    glm::vec3 cameraPos = ViewMatrixCamera.getPos() + 2.f * ViewMatrixCamera.getDir();

    glm::vec3 offset{0, -0.5, -4};
    _position = cameraPos;
    _position += offset;
}
