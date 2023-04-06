#pragma once

#include <vector>
#include "Fish.hpp"
#include "p6/p6.h"

class Boids {
private:
    std::vector<Fish> _fishpack;

public:
    Boids();
    Boids(const Boids& b) = default;
    explicit Boids(std::vector<Fish>& fish);
    ~Boids() = default;

    int  sizeFishpack();
    void generateFish(int nbFish, float& fishSize);
    void runBoids(Params p, p6::Context& context, const std::vector<Obstacle>& obstacles);
    void adjustBoids(int nbFish, float fishSize);
    void resizeBoids(float fishSize);
};