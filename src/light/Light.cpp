#include "Light.hpp"
#include "glm/gtc/type_ptr.hpp"

Light::Light(Program& program, const glm::vec3& kd, const glm::vec3& ks, float shininess, const glm::vec4& position, const glm::vec4 dirLPosition, const glm::vec3& intensity)
    : _program(program), _position(position), _dirLPosition(dirLPosition), _intensity(intensity), _kd(kd), _ks(ks), _shininess(shininess)
{}

Light::Light(Program& program, glm::vec4 const& position, const glm::vec4 _dirLPosition, glm::vec3 const& intensity)
    : _program(program), _position(position), _dirLPosition(_dirLPosition), _intensity(intensity)
{
    _kd        = glm::vec3(1, 0, 1);
    _ks        = glm::vec3(1, 0, 1);
    _shininess = 1.f;
}

void Light::sendUniform(const glm::mat4& ViewMatrixCamera, glm::vec3 arpPos)
{
    glUniform3fv(_program.uKd, 1, glm::value_ptr(_kd));
    glUniform3fv(_program.uKs, 1, glm::value_ptr(_ks));
    glUniform1f(_program.uShininess, _shininess);

    glUniform3fv(_program.uLightPos_vs, 1, glm::value_ptr(ViewMatrixCamera * glm::vec4(0.f, 0.f, 0.f, 1)));

    glUniform3fv(_program.uLightPosArp_vs, 1, glm::value_ptr(ViewMatrixCamera * glm::vec4(arpPos, 1)));
    glUniform3fv(_program.uLightDir_vs, 1, glm::value_ptr(ViewMatrixCamera * _dirLPosition));
    glUniform3fv(_program.uLightIntensity, 1, glm::value_ptr(_intensity));
}