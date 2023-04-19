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

    void computeDirectionVectors()
    {
        m_FrontVector = glm::vec3(
            glm::cos(glm::radians(m_fTheta)) * glm::sin(glm::radians(m_fPhi)),
            glm::sin(glm::radians(m_fTheta)),
            glm::cos(glm::radians(m_fTheta)) * glm::cos(glm::radians(m_fPhi))
        );

        m_LeftVector = glm::vec3(glm::sin(glm::radians(m_fPhi + (M_PI / 2))), 0, glm::cos(glm::radians(m_fPhi + (M_PI / 2))));

        m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    }

public:
    FreeflyCamera()
        : m_Position(glm::vec3(0, 0, 0)), m_fPhi(M_PI), m_fTheta(0)
    {
        computeDirectionVectors();
    }

    void moveLeft(float t) { m_Position += t * m_LeftVector; }

    void moveFront(float t) { m_Position += t * m_FrontVector; }

    void rotateLeft(float degrees)
    {
        m_fPhi += glm::radians(degrees);
        computeDirectionVectors();
    }

    void rotateUp(float degrees)
    {
        m_fTheta += glm::radians(degrees);
        computeDirectionVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        // glm::mat4 MVMatrix = glm::mat4(1.f);
        // glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
        // return MVMatrix;
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }
};