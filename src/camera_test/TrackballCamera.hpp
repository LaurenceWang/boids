#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"

class TrackballCamera {
private:
    glm::vec3 _position;
    float     _angleX;
    float     _angleY;

public:
    explicit TrackballCamera(glm::vec3 position = glm::vec3(0.f), float AngleX = 0, float AngleY = 0);

public:
    void  moveFront(float delta);
    void  moveLeft(float delta);
    void  moveUp(float delta);
    void  rotateLeft(float degrees);
    void  rotateUp(float degrees);
    float _distance;

public:
    glm::vec3 getPosition() const;
    // glm::mat4 getViewMatrix(const glm::vec3& arpenteurPosition) const;
    // void      updatePosition(const glm::vec3& arpenteurPosition, float orientation, const float border);
    glm::mat4 getViewMatrix() const;
    float     getAngleY() const;
    float     getAngleX() const;

    glm::vec3 setPos(glm::vec3 position);
};
