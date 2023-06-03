#pragma once
#include "Program.hpp"
#include "p6/p6.h"

class Light {
private:
    Program&  _program;
    glm::vec4 _position;
    glm::vec4 _dirLPosition;
    glm::vec3 _intensity;
    glm::vec3 _kd;
    glm::vec3 _ks;
    float     _shininess;

public:
    Light(Program& program, const glm::vec3& kd, const glm::vec3& ks, float shininess, const glm::vec4& position, const glm::vec4 dirLPosition, const glm::vec3& intensity);
    Light(Program& program, glm::vec4 const& position, const glm::vec4 dirLPosition, glm::vec3 const& intensity);

    void sendUniform(const glm::mat4& ViewMatrixCamera, glm::vec3 arpPos);
};
