#include "Fish.hpp"
#include <math.h>
#include <iostream>
#include "Speed.hpp"

Fish::Fish(glm::vec2 position, Family fam)
    : _pos(position), _family(fam)
{}

Fish::Fish()
{
    this->_pos = {0, 0};
    Family f{
        1, Speed(glm::vec2(1.f, 1.f), 0.1f), 0.02f, p6::Color(255., 0., 0.)};
    this->_family = f;
}

void Fish::updatePosition(glm::vec2 position)
{
    this->_pos.x += position.x;
    this->_pos.y += position.y;
}

void Fish::move()
{
    glm::vec2 newPos = glm::vec2(this->_family._s.getDir() * this->_family._s.getVel());
    updatePosition(newPos);
}

void Fish::resize(float newSize)
{
    _family._size = newSize;
}

void Fish::drawFish(p6::Context& context) const
{
    context.fill       = {this->_family._color};
    context.stroke     = {this->_family._color};
    context.use_fill   = true;
    context.use_stroke = true;
    context.equilateral_triangle(
        p6::Center{this->_pos.x, this->_pos.y},
        p6::Radius{this->_family._size},
        p6::Angle{p6::Radians{_family._s.getDir().y}}
    );
}

glm::vec2 Fish::separationForce(std::vector<Fish> const& boids, float& radius) const
{
    glm::vec2 sForce = {};

    std::vector<Fish> const neighbors = getNeighbors(boids, radius);
    for (auto const& neighbor : neighbors)
    {
        if (glm::distance(_pos, neighbor._pos) > 0.001f)
        {
            sForce += glm::normalize((_pos - neighbor._pos)) / glm::distance(_pos, neighbor._pos);
        }
    }
    // sForce += (pos - neighbor.pos) / glm::distance(pos, neighbor.pos);

    return sForce;
}

glm::vec2 Fish::alignmentForce(std::vector<Fish> const& boids, float& radius) const
{
    glm::vec2               aForce(0, 0);
    std::vector<Fish> const neighbors = getNeighbors(boids, radius);
    if (neighbors.empty())
    {
        return aForce;
    }
    for (auto const& neighbor : neighbors)
    {
        aForce += glm::normalize(neighbor._family._s.getDir() * neighbor._family._s.getVel());
        // aForce += glm::normalize(neighbor.speed);
    }

    aForce /= neighbors.size();
    return aForce;
}

glm::vec2 Fish::cohesionForce(std::vector<Fish> const& boids, float& radius) const
{
    glm::vec2               cForce(0, 0);
    std::vector<Fish> const neighbors = getNeighbors(boids, radius);
    if (neighbors.empty())
    {
        return cForce;
    }

    for (auto const& neighbor : neighbors)
    {
        cForce += neighbor._pos;
    }

    cForce /= neighbors.size();
    cForce -= _pos;
    return cForce;
}

void Fish::applyForces(std::vector<Fish> const& boids, Params& p)
{
    glm::vec2 speed         = _family._s.getDir() * _family._s.getVel();
    glm::vec2 steeringForce = {};
    steeringForce += separationForce(boids, p.neighRadius) * p.separation;
    steeringForce += alignmentForce(boids, p.neighRadius) * p.alignment;
    steeringForce += cohesionForce(boids, p.neighRadius) * p.steer;

    speed += steeringForce;
    speed *= 0.05;

    _family._s.setVel(speed.x);
    _family._s.setDir({1, speed.y / _family._s.getVel()});
}

void Fish::applyObstacleForces(std::vector<Obstacle> const& obstacles)
{
    glm::vec2 speed = _family._s.getDir() * _family._s.getVel();
    speed += obstacleForces(obstacles);

    // speed *= 0.01;
    _family._s.setVel(speed.x);
    _family._s.setDir({1, speed.y / _family._s.getVel()});
}

glm::vec2 Fish::obstacleForces(std::vector<Obstacle> const& obstacles)
{
    glm::vec2 oForce(0, 0);
    for (const auto& obstacle : obstacles)
    {
        if (glm::distance(_pos, obstacle.getPos()) < obstacle.getRadius() * 1.5)
        {
            oForce += glm::normalize(_pos - obstacle.getPos());
        }
    }

    oForce *= 0.01;

    return oForce;
}

glm::vec2 Fish::foodForces(std::vector<Food> const& food, int boidsLength) const
{
    glm::vec2 fForce(0, 0);

    for (const auto& f : food)
    {
        fForce += glm::normalize(f.getPos() - _pos)
                  * 0.001f / glm::distance(f.getPos(), _pos);
    }

    fForce /= boidsLength;

    return fForce;
}

void Fish::applyFoodForces(std::vector<Food> const& food, int boidsLength)
{
    glm::vec2 speed = _family._s.getDir() * _family._s.getVel();
    speed += foodForces(food, boidsLength);
    _family._s.setVel(speed.x);
    _family._s.setDir({1, speed.y / _family._s.getVel()});
}

std::vector<Fish> Fish::getNeighbors(const std::vector<Fish>& boids, float& radius) const
{
    std::vector<Fish> neighbors{};
    for (const auto& fish : boids)
    {
        // if (*this != fish)
        //{

        // if (this->_family._id != fish._family._id)
        // {
        //     throw 0;
        // }

        if ((glm::distance(this->_pos, fish._pos) < radius) && (this->_family._id == fish._family._id))
        {
            neighbors.push_back(fish);
        }
        //}
    }

    return neighbors;
}
