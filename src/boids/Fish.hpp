#pragma once

#include <vector>
#include "Family.hpp"
#include "Speed.hpp"
#include "food/Food.hpp"
#include "imgui/Params.hpp"
#include "obstacles/Obstacle.hpp"
#include "p6/p6.h"

using ObstacleHandler = std::function<void(Obstacle const&)>;

class Fish {
private:
    glm::vec3 _pos;
    Speed     _s;
    Family    _family;

    void              updatePosition(glm::vec3 position);
    glm::vec3         separationForce(std::vector<Fish> const& boids, float& radius) const;
    glm::vec3         alignmentForce(std::vector<Fish> const& boids, float& radius) const;
    glm::vec3         cohesionForce(std::vector<Fish> const& boids, float& radius) const;
    glm::vec3         obstacleForces(std::function<void(ObstacleHandler)> const& for_each_obstacle);
    glm::vec3         foodForces(std::vector<Food> const& food, int boidsLength) const;
    std::vector<Fish> getNeighbors(const std::vector<Fish>& boids, float& radius) const;

public:
    Fish();
    Fish(const Fish& f) = default;
    Fish(glm::vec3 position, Speed s, Family fam);
    ~Fish() = default;

    glm::vec3 getPos() const;
    float     getDirectionY() const;
    void      move();
    void      resize(float newSize);
    void      drawFish2D(p6::Context& context) const;
    void      applyForces(std::vector<Fish> const& boids, Params& p);
    void      applyObstacleForces(std::function<void(ObstacleHandler)> const& for_each_obstacle);
    void      applyFoodForces(std::vector<Food> const& food, int boidsLength);
};
