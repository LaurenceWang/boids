#include "Fish.hpp"
#include <iostream>

Fish::Fish(std::vector<float> position, std::vector<float> velocity, std::vector<float> direction)
    : pos(position), vel(velocity), dir(direction) {}

void Fish::updatePosition(std::vector<float> position)
{
    this->pos[0] += position[0];
    this->pos[1] += position[1];
}

void Fish::move()
{
    std::vector<float> newPos = {this->vel[0] * this->dir[0], this->vel[1] * this->dir[1]};
    updatePosition(newPos);
}

void Fish::drawFish(p6::Context& context)
{
    context.square(
        p6::Center{this->pos[0], this->pos[1]},
        p6::Radius{
            0.02f}
    );
}