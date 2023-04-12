#pragma once

#include <vector>
#include "Obstacle.hpp"
#include "p6/p6.h"

class ObstacleCollection {
private:
    std::vector<Obstacle> _obstacles;

public:
    ObstacleCollection();
    ObstacleCollection(const ObstacleCollection& o) = default;
    explicit ObstacleCollection(std::vector<Obstacle> const& o);

    void                  generateObstacles(int nbObstacles);
    void                  generateBorders(p6::Context& ctx);
    void                  runObstacles(p6::Context& ctx);
    std::vector<Obstacle> getObstacles() { return _obstacles; };
};