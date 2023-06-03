#include "TrackballCamera.hpp"
#include <iostream>
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"

TrackballCamera::TrackballCamera(glm::vec3 position, float AngleX, float AngleY)
    : _position(-position), _angleX(AngleX), _angleY(AngleY) {}

void TrackballCamera::moveFront(float delta)
{
    float x = 0.f;
    float z = 0.f;

    int angle = static_cast<int>(_angleY);
    if (angle < 0)
    {
        angle = 360 - (-angle) % 360;
    }
    else
    {
        angle = angle % 360;
    }

    if (angle < 90)
    {
        z = ((90 - angle) / 90.f) * delta;
        x = -((angle) / 90.f) * delta;
    }
    if (angle >= 90 && angle < 180)
    {
        z = -((angle - 90) / 90.f) * delta;
        x = -((180 - angle) / 90.f) * delta;
    }
    if (angle >= 180 && angle < 270)
    {
        z = -((270 - angle) / 90.f) * delta;
        x = ((angle - 180) / 90.f) * delta;
    }
    if (angle >= 270)
    {
        z = ((angle - 270) / 90.f) * delta;
        x = ((360 - angle) / 90.f) * delta;
    }

    glm::vec3 newPos = glm::vec3(_position.x + x, _position.y, _position.z + z);
    int       border = 15;
    if (newPos.x > border || newPos.x < -border || newPos.y > border || newPos.y < -border || newPos.z > border + 5 || newPos.z < -border - +5)
    {
    }
    else
    {
        _position = glm::vec3(_position.x + x, _position.y, _position.z + z);
    }
}

void TrackballCamera::moveLeft(float delta)
{
    float x = 0.f;
    float z = 0.f;

    int angle = static_cast<int>(_angleY - 90);
    if (angle < 0)
    {
        angle = 360 - (-angle) % 360;
    }
    else
    {
        angle = angle % 360;
    }

    if (angle < 90)
    {
        z = ((90 - angle) / 90.f) * delta;
        x = -((angle) / 90.f) * delta;
    }
    if (angle >= 90 && angle < 180)
    {
        z = -((angle - 90) / 90.f) * delta;
        x = -((180 - angle) / 90.f) * delta;
    }
    if (angle >= 180 && angle < 270)
    {
        z = -((270 - angle) / 90.f) * delta;
        x = ((angle - 180) / 90.f) * delta;
    }
    if (angle >= 270)
    {
        z = ((angle - 270) / 90.f) * delta;
        x = ((360 - angle) / 90.f) * delta;
    }

    glm::vec3 newPos = glm::vec3(_position.x + x, _position.y, _position.z + z);
    int       border = 15;
    if (newPos.x > border || newPos.x < -border || newPos.y > border || newPos.y < -border || newPos.z > border + 5 || newPos.z < -border + 5)
    {
    }
    else
    {
        _position = glm::vec3(_position.x + x, _position.y, _position.z + z);
    }
}

void TrackballCamera::moveUp(float delta)
{
    _position = glm::vec3(_position.x, _position.y + delta, _position.z);
}

void TrackballCamera::rotateLeft(float degrees)
{
    _angleY += degrees;
}

void TrackballCamera::rotateUp(float degrees)
{
    _angleX += degrees;
}

glm::vec3 TrackballCamera::setPos(glm::vec3 position)
{
    _position = position;
}

glm::vec3 TrackballCamera::getPosition() const
{
    return -_position;
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::mat4 viewMatrix(1);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(_angleX), glm::vec3(1, 0, 0));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(_angleY), glm::vec3(0, 1, 0));
    viewMatrix = glm::translate(viewMatrix, _position);

    return viewMatrix;
}

float TrackballCamera::getAngleY() const
{
    return _angleY;
}

float TrackballCamera::getAngleX() const
{
    return _angleX;
}
