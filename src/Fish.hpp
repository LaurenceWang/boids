#pragma once

#include <vector>
#include "p6/p6.h"

// rename m_attribut
class Fish {
private:
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 dir;
    float     size;

public:
    Fish();
    Fish(const Fish& f) = default;
    Fish(glm::vec2 position, glm::vec2 velocity, glm::vec2 direction, float size);
    ~Fish() = default;

    void              drawFish(p6::Context& context) const;
    void              updatePosition(glm::vec2 position);
    void              move();
    double            getDistance(Fish& f) const;
    glm::vec2         getAwayDir(Fish& f) const;
    void              steerAway(glm::vec2 awayDir);
    std::vector<Fish> getNeighbors(const std::vector<Fish>& boids) const;
    glm::vec2         getPos() const;
    glm::vec2         seperationForce(std::vector<Fish> const& boids) const;
    glm::vec2         alignmentForce(std::vector<Fish> const& boids) const;
    glm::vec2         cohesionForce(std::vector<Fish> const& boids) const;
    void              applyForces(std::vector<Fish> const& boids);
};