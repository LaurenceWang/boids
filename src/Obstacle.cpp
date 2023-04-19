#include "Obstacle.hpp"

Obstacle::Obstacle()
    : _pos(p6::random::point(), 0), _radius(p6::random::number(0.03, 0.18f)), _isBorderX(false), _isBorderY(false) {}

Obstacle::Obstacle(glm::vec3 position, float rad)
    : _pos(position), _radius(rad), _isBorderX(false), _isBorderY(false) {}

Obstacle::Obstacle(glm::vec3 position, float rad, bool borderX, bool borderY)
    : _pos(position), _radius(rad), _isBorderX(borderX), _isBorderY(borderY) {}

float Obstacle::getRadius() const
{
    return _radius;
}

glm::vec3 Obstacle::getPos() const
{
    return _pos;
}

void Obstacle::updateBorderX(p6::Context& ctx)
{
    if (_isBorderX)
    {
        if (_pos.x < 0)
        {
            _pos.x += -ctx.aspect_ratio() - _pos.x;
        }
        else
        {
            _pos.x += ctx.aspect_ratio() - _pos.x;
        }
        _radius = ctx.aspect_ratio() / 70;
    }
}

void Obstacle::updateBorderY(p6::Context& ctx, int nb)
{
    if (_isBorderY)
    {
        if (nb > 71)
        {
            nb = nb % 71;
        }
        _pos.x  = -ctx.aspect_ratio() + nb * (2 * ctx.aspect_ratio() / 70);
        _radius = ctx.aspect_ratio() / 70;
    }
}

void Obstacle::updateBorderPosition(p6::Context& ctx, int i)
{
    updateBorderX(ctx);
    updateBorderY(ctx, i);
}

void Obstacle::draw(p6::Context& ctx) const
{
    ctx.fill   = {0.16, 0.32, 0.29};
    ctx.stroke = {0.16, 0.32, 0.29};

    ctx.use_fill   = true;
    ctx.use_stroke = true;
    // ctx.rectangle(p6::TopRightCorner{_pos}, p6::Radii{_pos.x * 0.5f, _pos.y * 0.5f});
    if (!_isBorderX && !_isBorderY)
    {
        ctx.circle(p6::Center{_pos}, p6::Radius{_radius});
    }
}
