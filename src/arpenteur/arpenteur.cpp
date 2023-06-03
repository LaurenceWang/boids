#include "arpenteur.hpp"

/*Arpenteur::Arpenteur(glm::vec3 position)
    : _position(position) {}*/

void Arpenteur::updatePosition(FreeflyCamera ViewMatrixCamera)
{
    glm::vec3 cameraPos = ViewMatrixCamera.getPos() - 2.f * ViewMatrixCamera.getDir();
    /*if (_position.x > mapDimension.x || _position.x < -mapDimension.x || _position.y > mapDimension.y || _position.y < -mapDimension.y || _position.z > mapDimension.z || _position.z < -mapDimension.z)
    {
    }*/

    /*if (cameraPos.x > mapDimension.x || cameraPos.x < -mapDimension.x || cameraPos.y > mapDimension.y || cameraPos.y < -mapDimension.y || cameraPos.z > mapDimension.z || cameraPos.z < -mapDimension.z)
    {
    }
    else
    {
        _position = ViewMatrixCamera.getPos() - 2.f * ViewMatrixCamera.getDir();
    }*/
    glm::vec3 offset{0, -0.5, -4};
    _position = ViewMatrixCamera.getPos() + 2.f * ViewMatrixCamera.getDir();
    _position += offset;

    /*glm::vec3 cameraPosition  = ViewMatrixCamera.getPos();
    glm::vec3 cameraDirection = ViewMatrixCamera.getDir();

    glm::vec3 characterOffset      = glm::vec3(_position.x - cameraPosition.x, 0.0, _position.z - cameraPosition.z);
    glm::vec3 newCharacterPosition = cameraPosition + 2.0f * cameraDirection + characterOffset;

    _position = newCharacterPosition;*/
    //_position = ViewMatrixCamera.getPos() - 2.f * (ViewMatrixCamera.getDir() - glm::cross(ViewMatrixCamera.getUp(), ViewMatrixCamera.getLeft()));
}

void Arpenteur::updatePosition2(TrackballCamera ViewMatrixCamera)
{
    _direction      = ViewMatrixCamera.getAngleY();
    this->_position = glm::vec3(ViewMatrixCamera.getPosition().x, ViewMatrixCamera.getPosition().y, ViewMatrixCamera.getPosition().z);
}
