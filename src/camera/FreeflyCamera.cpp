#include "FreeflyCamera.hpp"

FreeflyCamera::FreeflyCamera()
{
    m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_fPhi     = glm::pi<float>();
    m_fTheta   = 0.0f;

    computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors()
{
    {
        m_FrontVector = glm::vec3(glm::cos(m_fTheta) * glm::sin(m_fPhi), glm::sin(m_fTheta), glm::cos(m_fTheta) * glm::cos(m_fPhi));

        m_LeftVector = glm::vec3(glm::sin(m_fPhi + (glm::pi<float>() / 2.f)), 0, glm::cos(m_fPhi + (glm::pi<float>() / 2.f)));

        m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    }
}

void FreeflyCamera::moveLeft(float t)
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
}

void FreeflyCamera::moveFront(float t)
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

void FreeflyCamera::moveUp(float t)
{
    m_Position += t * glm::vec3(0.f, 1.f, 0.f);
}

void FreeflyCamera::moveDown(float t)
{
    m_Position -= t * glm::vec3(0.f, 1.f, 0.f);
}

void FreeflyCamera::rotateLeft(float degrees)
{
    m_fPhi += glm::radians(degrees);
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    m_fTheta += glm::radians(degrees);
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

glm::vec3 FreeflyCamera::getPos() const
{
    return m_Position;
}

glm::vec3 FreeflyCamera::getDir() const
{
    return -m_FrontVector;
}

glm::vec3 FreeflyCamera::getFront() const
{
    return m_FrontVector;
}

glm::vec3 FreeflyCamera::getUp() const
{
    return m_UpVector;
}

glm::vec3 FreeflyCamera::getLeft() const
{
    return m_LeftVector;
}