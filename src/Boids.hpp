#pragma once

#include <vector>
#include "Family.hpp"
#include "Fish.hpp"
#include "p6/p6.h"

class Boids {
private:
    std::vector<Fish> _fishpack;
    Family            _family;

public:
    Boids();
    Boids(const Boids& b) = default;
    explicit Boids(std::vector<Fish>& fish);

    int  sizeFishpack();
    void generateFish(int nbFish, float& fishSize, int fam);
    void runBoids(Params p, p6::Context& context, const std::vector<Obstacle>& obstacles, std::vector<Food>& food);
    void adjustBoids(int nbFish, float fishSize);
    void resizeBoids(float fishSize);
};