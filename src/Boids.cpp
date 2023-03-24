#include "Boids.hpp"

Boids::Boids(std::vector<Fish>& fish)
    : _fishpack(fish)
{}

Boids::Boids()
{
    std::vector<Fish> f;
    _fishpack = f;
}

void Boids::generateFish(int& nbFish)
{
    for (int i = 0; i < nbFish; ++i)
    {
        glm::vec2 coord = p6::random::point();
        glm::vec2 direc = p6::random::direction();
        float     velo  = p6::random::number(0, 0.001f);
        Speed     speed = Speed(direc, velo);
        Fish      b(coord, speed, 0.02f);
        _fishpack.push_back(b);
    }
}

void Boids::runBoids(Params p, p6::Context& context)
{
    for (auto& boid : _fishpack)
    {
        boid.applyForces(_fishpack, p);
        boid.move();
        boid.drawFish(context);
    }
}