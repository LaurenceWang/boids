#include "Boids.hpp"

Family carpe{0, Speed(glm::vec2(p6::random::direction()), 0.1f), 0.02f, p6::Color(0.58, 0.09, 0.13)};
Family saumon{1, Speed(glm::vec2(p6::random::direction()), 0.1f), 0.02f, p6::Color(0.98, 0.44, 0.41)};

std::vector<Family> species = {carpe, saumon};

Boids::Boids(std::vector<Fish>& fish)
    : _fishpack(fish)
{}

Boids::Boids()
{
    std::vector<Fish> f;
    _fishpack = f;
}

void Boids::generateFish(int nbFish, float& fishSize, int fam)
{
    for (int i = 0; i < nbFish; ++i)
    {
        glm::vec2 coord = p6::random::point();
        glm::vec2 direc = p6::random::direction();
        float     velo  = 0.1f;
        // float velo  = p6::random::number(0, 0.1f);
        Speed speed = Speed(direc, velo);
        Fish  b(coord, species[fam]);
        _fishpack.push_back(b);
    }
}

void Boids::runBoids(Params p, p6::Context& context, std::function<void(ObstacleHandler)> const& for_each_obstacle, std::vector<Food>& food)
{
    for (auto& boid : _fishpack)
    {
        boid.applyForces(_fishpack, p);
        boid.applyObstacleForces(for_each_obstacle);
        boid.applyFoodForces(food, _fishpack.size());
        boid.move();
        // context.fill = {1.f, 0.7f, 0.2f};
        boid.drawFish(context);
    }
}

int Boids::sizeFishpack()
{
    return (this->_fishpack.size());
}

void Boids::adjustBoids(int nbFish, float fishSize)
{
    if (nbFish > this->sizeFishpack())
    {
        this->generateFish(nbFish - this->sizeFishpack(), fishSize, 1);
    }

    else
    {
        for (int i; this->sizeFishpack() - nbFish; i++)
            this->_fishpack.pop_back();
    }
}

void Boids::resizeBoids(float fishSize)
{
    for (auto& boid : _fishpack)
    {
        boid.resize(fishSize);
    }
}
