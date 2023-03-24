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

    void generateFish(int& nbFish);
    void runBoids(Params p, p6::Context& context);
};