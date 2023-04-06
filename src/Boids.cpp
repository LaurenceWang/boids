#include "Boids.hpp"

Boids::Boids(std::vector<Fish>& fish)
    : _fishpack(fish)
{}

Boids::Boids()
{
    std::vector<Fish> f;
    _fishpack = f;
}

void Boids::generateFish(int& nbFish, float& fishSize)
{
    for (int i = 0; i < nbFish; ++i)
    {
        glm::vec2 coord = p6::random::point();
        glm::vec2 direc = p6::random::direction();
        float     velo  = 0.1f;
        // float velo  = p6::random::number(0, 0.1f);
        Speed speed = Speed(direc, velo);
        Fish  b(coord, speed, fishSize);
        _fishpack.push_back(b);
    }
}

void Boids::runBoids(Params p, p6::Context& context, const std::vector<Obstacle>& obstacles)
{
    for (auto& boid : _fishpack)
    {
        boid.applyForces(_fishpack, p);
        boid.applyObstacleForces(obstacles);
        boid.move();
        boid.drawFish(context);
    }
}

int Boids::sizeFishpack()
{
    return (this->_fishpack.size());
}

void adjustBoids(int& nbFish, Boids school)
{
    if (nbFish <= school.sizeFishpack())
    {
    }
    else
    {
    }
}