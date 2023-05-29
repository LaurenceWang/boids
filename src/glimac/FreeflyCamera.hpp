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
        m_FrontVector = glm::vec3(glm::cos(m_fTheta) * glm::sin(m_fPhi), glm::sin(m_fTheta), glm::cos(m_fTheta) * glm::cos(m_fPhi));

        m_LeftVector = glm::vec3(glm::sin(m_fPhi + (glm::pi<float>() / 2.f)), 0, glm::cos(m_fPhi + (glm::pi<float>() / 2.f)));

        m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    }

public:
    FreeflyCamera()
    {
        m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_fPhi     = glm::pi<float>();
        m_fTheta   = 0.0f;

        computeDirectionVectors();
    }

    void moveLeft(float t)
    {
        glm::vec3 newPos = m_Position + t * m_LeftVector;
        int       border = 15;

        if (newPos.x > border || newPos.x < -border || newPos.y > border || newPos.y < -border || newPos.z > border + 5 || newPos.z < -border + 5)
        {
        }
        else
        {
            m_Position += t * m_LeftVector;
        }
        // if(newPos)
    }

    void moveFront(float t)
    {
        glm::vec3 newPos = m_Position + t * m_FrontVector;
        int       border = 15;

        if (newPos.x > border || newPos.x < -border || newPos.y > border || newPos.y < -border || newPos.z > border + 5 || newPos.z < -border - +5)
        {
        }
        else
        {
            m_Position += t * m_FrontVector;
        }
    }

    void moveUp(float t)
    {
        m_Position += t * glm::vec3(0.f, 1.f, 0.f);
    }

    void moveDown(float t)
    {
        m_Position -= t * glm::vec3(0.f, 1.f, 0.f);
    }

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
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }

    glm::vec3 getPos() const
    {
        return m_Position;
    }

    glm::vec3 getDir() const
    {
        return -m_FrontVector;
    }

    glm::vec3 getFront() const
    {
        return m_FrontVector;
    }

    glm::vec3 getUp() const
    {
        return m_UpVector;
    }

    glm::vec3 getLeft() const
    {
        return m_LeftVector;
    }
};
