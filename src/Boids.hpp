#pragma once

#include <vector>
#include "Family.hpp"
#include "Fish.hpp"
#include "p6/p6.h"

using ObstacleHandler = std::function<void(Obstacle const&)>;

class Boids {
private:
    std::vector<Fish> _fishpack;
    Family            _family;

public:
    Boids();
    Boids(const Boids& b) = default;
    explicit Boids(std::vector<Fish>& fish);

    int               sizeFishpack();
    void              generateFish(int nbFish, int fam);
    void              runBoids(Params p, p6::Context& context, std::function<void(ObstacleHandler)> const& for_each_obstacle, std::vector<Food>& food);
    void              adjustBoids(int nbFish);
    void              resizeBoids(float fishSize);
    std::vector<Fish> getFishPack() { return _fishpack; }
};