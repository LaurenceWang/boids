#pragma once
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_fPhi;
    float     m_fTheta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors();

public:
    FreeflyCamera();
    void moveLeft(float t);

    void moveFront(float t);

    void moveUp(float t);

    void moveDown(float t);

    void rotateLeft(float degrees);

    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;

    glm::vec3 getPos() const;

    glm::vec3 getDir() const;

    glm::vec3 getFront() const;

    glm::vec3 getUp() const;

    glm::vec3 getLeft() const;
};
