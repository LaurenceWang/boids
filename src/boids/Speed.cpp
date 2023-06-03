#include "Speed.hpp"
#include <math.h>
#include <iostream>

Speed::Speed(glm::vec3 direction, float velocity)
    : dir(direction), vel(velocity) {}
Speed::Speed()
    : dir({0, 0, 0}), vel(0.001f) {}

void Speed::setDir(glm::vec3 direction)
{
    this->dir = direction;
}
void Speed::setVel(float velocity)
{
    this->vel = velocity;
}
glm::vec3 Speed::getDir() const
{
    return this->dir;
}
float Speed::getVel() const
{
    return this->vel;
}
