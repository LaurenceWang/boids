#include "Fish.hpp"
#include <math.h>
#include <iostream>
#include "Speed.hpp"

Fish::Fish(glm::vec2 position, Speed speed, float size)
    : _pos(position), _s(speed), _size(size)
{}

Fish::Fish()
    : _pos({0, 0}), _s(glm::vec2(1.f, 1.f), 0.1f), _size(0.02f) {}

void Fish::updatePosition(glm::vec2 position)
{
    this->_pos.x += position.x;
    this->_pos.y += position.y;
}

void Fish::move()
{
    glm::vec2 newPos = glm::vec2(this->_s.getDir() * this->_s.getVel());
    updatePosition(newPos);
}

void Fish::resize(float newSize)
{
    _size = newSize;
}

void Fish::drawFish(p6::Context& context) const
{
    context.equilateral_triangle(
        p6::Center{this->_pos.x, this->_pos.y},
        p6::Radius{this->_size},
        p6::Angle{p6::Radians{_s.getDir().y}}
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
        aForce += glm::normalize(neighbor._s.getDir() * neighbor._s.getVel());
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
    glm::vec2 speed         = _s.getDir() * _s.getVel();
    glm::vec2 steeringForce = {};
    steeringForce += separationForce(boids, p.neighRadius) * p.sepStr;
    steeringForce += alignmentForce(boids, p.neighRadius) * p.aliStr;
    steeringForce += cohesionForce(boids, p.neighRadius) * p.steerStr;

    speed += steeringForce;
    speed *= 0.05;

    _s.setVel(speed.x);
    _s.setDir({1, speed.y / _s.getVel()});
}

void Fish::applyObstacleForces(std::vector<Obstacle> const& obstacles)
{
    glm::vec2 speed = _s.getDir() * _s.getVel();
    speed += obstacleForces(obstacles);

    // speed *= 0.01;
    _s.setVel(speed.x);
    _s.setDir({1, speed.y / _s.getVel()});
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

std::vector<Fish> Fish::getNeighbors(const std::vector<Fish>& boids, float& radius) const
{
    std::vector<Fish> neighbors{};
    for (const auto& fish : boids)
    {
        // if (*this != fish)
        //{
        if (glm::distance(this->_pos, fish._pos) < radius)
        {
            neighbors.push_back(fish);
        }
        //}
    }

    return neighbors;
}
