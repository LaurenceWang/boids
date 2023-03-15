#include "Fish.hpp"
#include <math.h>
#include <iostream>

Fish::Fish(glm::vec2 position, glm::vec2 sp, float size)
    : pos(position)
    , speed(sp)
    , size(size)
{}

Fish::Fish()
    : pos({0, 0}), speed({0.1f, 0.1f}), size(0.02f) {}

void Fish::updatePosition(glm::vec2 position)
{
    this->pos.x += position.x;
    this->pos.y += position.y;
}

void Fish::move()
{
    glm::vec2 newPos = glm::vec2(this->speed);
    updatePosition(newPos);
}

void Fish::drawFish(p6::Context& context) const
{
    context.equilateral_triangle(
        p6::Center{this->pos.x, this->pos.y},
        p6::Radius{this->size}

    );
}

double Fish::getDistance(Fish& f) const
{
    return std::sqrt(std::pow(f.pos.x - this->pos.x, 2) + std::pow(f.pos.y - this->pos.y, 2));
}

glm::vec2 Fish::getAwayDir(Fish& f) const
{
    glm::vec2 newDir = glm::vec2(this->pos.x - f.pos.x, this->pos.y - f.pos.y);
    return newDir;
}

glm::vec2 Fish::getPos() const
{
    return this->pos;
}

glm::vec2 Fish::separationForce(std::vector<Fish> const& boids) const
{
    glm::vec2 sForce = {};

    std::vector<Fish> const neighbors = getNeighbors(boids);
    for (auto const& neighbor : neighbors)
    {
        if (glm::distance(pos, neighbor.pos) > 0.001f)
        {
            sForce += glm::normalize((pos - neighbor.pos)) / glm::distance(pos, neighbor.pos);
        }
    }
    // sForce += (pos - neighbor.pos) / glm::distance(pos, neighbor.pos);

    return sForce;
}

glm::vec2 Fish::alignmentForce(std::vector<Fish> const& boids) const
{
    glm::vec2               aForce(0, 0);
    std::vector<Fish> const neighbors = getNeighbors(boids);
    if (neighbors.empty())
    {
        return aForce;
    }
    for (auto const& neighbor : neighbors)
    {
        aForce += glm::normalize(neighbor.speed);
    }

    aForce /= neighbors.size();
    return aForce;
}

glm::vec2 Fish::cohesionForce(std::vector<Fish> const& boids) const
{
    glm::vec2               cForce(0, 0);
    std::vector<Fish> const neighbors = getNeighbors(boids);
    if (neighbors.empty())
    {
        return cForce;
    }

    for (auto const& neighbor : neighbors)
    {
        cForce += neighbor.pos;
    }

    cForce /= neighbors.size();
    cForce -= pos;
    return cForce;
}

void Fish::applyForces(std::vector<Fish> const& boids)
{
    glm::vec2 steeringForce = {};
    steeringForce += separationForce(boids) * 0.00001f;
    steeringForce += alignmentForce(boids) * 0.02f;
    steeringForce += cohesionForce(boids) * 0.015f;

    // std::cout << "x : " << steeringForce.x << " y : " << steeringForce.y << std::endl;
    // std::cout << "sForce : " << sForce.x << " sForce : " << sForce.y << std::endl;
    speed += steeringForce;
    speed *= 0.1;

    // std::cout << "vel : " << speed.x << " vel " << speed.y << std::endl;
    //  std::cout << "vel : " << vel.x << " vel " << vel.y << std::endl;
}

std::vector<Fish> Fish::getNeighbors(const std::vector<Fish>& boids) const
{
    std::vector<Fish> neighbors{};
    for (const auto& fish : boids)
    {
        if (glm::distance(this->pos, fish.pos) < 0.08)
        {
            neighbors.push_back(fish);
        }
    }

    return neighbors;
}
