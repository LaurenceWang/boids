#include "Fish.hpp"
#include <math.h>
#include <iostream>
#include "Speed.hpp"

// using ObstacleHandler = std::function<void(Obstacle const&)>;

Fish::Fish(glm::vec3 position, Speed s, Family fam)
    : _pos(position), _s(s), _family(fam)
{}

Fish::Fish()
{
    this->_pos = {0, 0, 0};
    this->_s   = Speed(glm::vec3(1.f, 1.f, 1.f), 0.1f);
    Family f{
        1, 0.02f, p6::Color(255., 0., 0.)};
    this->_family = f;
}

glm::vec3 Fish::getPos() const
{
    return _pos;
}

void Fish::updatePosition(glm::vec3 position)
{
    this->_pos.x += position.x;
    this->_pos.y += position.y;
    this->_pos.z += position.z;
}

void Fish::move()
{
    glm::vec3 newPos = glm::vec3(this->_s.getDir() * this->_s.getVel());
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
        p6::Angle{p6::Radians{_s.getDir().y}}
    );
}

glm::vec3 Fish::separationForce(std::vector<Fish> const& boids, float& radius) const // réfléchir à prendre en compte la fishSize pour éviter des collisions dues à la taille
{
    glm::vec3 sForce = {};

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

glm::vec3 Fish::alignmentForce(std::vector<Fish> const& boids, float& radius) const
{
    glm::vec3               aForce(0, 0, 0);
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

glm::vec3 Fish::cohesionForce(std::vector<Fish> const& boids, float& radius) const
{
    glm::vec3               cForce(0, 0, 0);
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
    glm::vec3 speed         = _s.getDir() * _s.getVel();
    glm::vec3 steeringForce = {};
    steeringForce += separationForce(boids, p.neighRadius) * p.separation;
    steeringForce += alignmentForce(boids, p.neighRadius) * p.alignment;
    steeringForce += cohesionForce(boids, p.neighRadius) * p.steer;

    speed += steeringForce;
    speed *= 0.05;

    _s.setVel(speed.x);
    _s.setDir({1, speed.y / _s.getVel(), 0});
}

void Fish::applyObstacleForces(std::function<void(ObstacleHandler)> const& for_each_obstacle)
{
    glm::vec3 speed = _s.getDir() * _s.getVel();
    speed += obstacleForces(for_each_obstacle);

    _s.setVel(speed.x);
    _s.setDir({1, speed.y / _s.getVel(), 0});
}

glm::vec3 Fish::obstacleForces(std::function<void(ObstacleHandler)> const& for_each_obstacle)
{
    glm::vec3 oForce(0, 0, 0);
    /*for (const auto& obstacle : obstacles)
    {
        if (glm::distance(_pos, obstacle.getPos()) < obstacle.getRadius() * 1.5)
        {
            oForce += glm::normalize(_pos - obstacle.getPos());
        }
    }*/
    for_each_obstacle([&](const auto& obstacle) {
        if (glm::distance(_pos, obstacle.getPos()) < obstacle.getRadius() * 1.5)
        {
            oForce += glm::normalize(_pos - obstacle.getPos());
        }
    });

    oForce *= 0.01;

    return oForce;
}

glm::vec3 Fish::foodForces(std::vector<Food> const& food, int boidsLength) const
{
    glm::vec3 fForce(0, 0, 0);

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
    glm::vec3 speed = _s.getDir() * _s.getVel();
    speed += foodForces(food, boidsLength);
    _s.setVel(speed.x);
    _s.setDir({1, speed.y / _s.getVel(), 0});
}

std::vector<Fish> Fish::getNeighbors(const std::vector<Fish>& boids, float& radius) const
{
    std::vector<Fish> neighbors{};
    for (const auto& fish : boids)
    {
        if ((glm::distance(this->_pos, fish._pos) < radius) && (this->_family._id == fish._family._id))
        {
            neighbors.push_back(fish);
        }
    }

    return neighbors;
}
