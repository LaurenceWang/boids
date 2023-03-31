#pragma once

#include <vector>
#include "Obstacle.hpp"
#include "Params.hpp"
#include "Speed.hpp"
#include "p6/p6.h"

class Fish {
private:
    glm::vec2 _pos;
    Speed     _s;
    float     _size;

public:
    Fish();
    Fish(const Fish& f) = default;
    Fish(glm::vec2 position, Speed s, float size);
    ~Fish() = default;

    void              drawFish(p6::Context& context) const;
    void              updatePosition(glm::vec2 position);
    void              move();
    std::vector<Fish> getNeighbors(const std::vector<Fish>& boids, float& radius) const;
    glm::vec2         getPos() const;
    glm::vec2         separationForce(std::vector<Fish> const& boids, float& radius) const;
    glm::vec2         alignmentForce(std::vector<Fish> const& boids, float& radius) const;
    glm::vec2         cohesionForce(std::vector<Fish> const& boids, float& radius) const;
    void              applyForces(std::vector<Fish> const& boids, Params& p);
    void              applyObstacleForces(std::vector<Obstacle> const& obstacles);
    glm::vec2         obstacleForces(std::vector<Obstacle> const& obstacle);
};
