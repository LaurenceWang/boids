#include "Food.hpp"

Food::Food()
    : _pos(p6::random::point()), _radius(p6::random::number(0.03, 0.1f)) {}

Food::Food(glm::vec2 position, float rad)
    : _pos(position), _radius(rad) {}

void Food::draw(p6::Context& ctx) const
{
    ctx.square(p6::Center{_pos}, p6::Radius{_radius});
}

float Food::getRadius() const
{
    return _radius;
}

glm::vec2 Food::getPos() const
{
    return _pos;
}