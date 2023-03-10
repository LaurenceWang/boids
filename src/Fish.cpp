#include "Fish.hpp"
#include <iostream>

Fish::Fish(glm::vec2 position, glm::vec2 velocity, glm::vec2 direction, float size)
    : pos(position)
    , vel(velocity)
    , dir(direction)
    , size(size)
{}

// Constructeur vide à modifier
Fish::Fish()
    : pos({0, 0}), vel({0.001f, 0.001f}), dir({1.f, -1.f}), size(0.02f) {}

void Fish::updatePosition(glm::vec2 position)
{
    this->pos.x += position.x;
    this->pos.y += position.y;
}

void Fish::move()
{
    glm::vec2 newPos = glm::vec2(this->vel.x * this->dir.x, this->vel.y * this->dir.y);
    updatePosition(newPos);
}

void Fish::drawFish(p6::Context& context)
{
    context.equilateral_triangle(
        p6::Center{this->pos.x, this->pos.y},
        p6::Radius{this->size},
        p6::Rotation{p6::Angle(p6::Radians{this->dir.y})}
    );
}