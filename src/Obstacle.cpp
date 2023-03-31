#include "Obstacle.hpp"

Obstacle::Obstacle()
    : _pos(p6::random::point()), _radius(p6::random::number(0.1, 0.2f)) {}

Obstacle::Obstacle(glm::vec2 position, float rad)
    : _pos(position), _radius(rad) {}

float Obstacle::getRadius() const
{
    return _radius;
}

glm::vec2 Obstacle::getPos() const
{
    return _pos;
}

void Obstacle::draw(p6::Context& ctx) const
{
    // ctx.rectangle(p6::TopRightCorner{_pos}, p6::Radii{_pos.x * 0.5f, _pos.y * 0.5f});
    ctx.square(p6::Center{_pos}, p6::Radius{_radius});
}