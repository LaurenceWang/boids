#include "ObstacleCollection.hpp"

ObstacleCollection::ObstacleCollection(std::vector<Obstacle>& o)
    : _obstacles(o)
{}

ObstacleCollection::ObstacleCollection()
{
    std::vector<Obstacle> o;
    _obstacles = o;
}

ObstacleCollection::ObstacleCollection(int nbObstacles)
{
    generateObstacles(nbObstacles);
}

void ObstacleCollection::generateObstacles(int nbObstacles)
{
    for (int i = 0; i < nbObstacles; ++i)
    {
        Obstacle o;
        _obstacles.push_back(o);
    }
}

void ObstacleCollection::generateBorders(p6::Context& ctx)
{
    const int borderLength = 8;
    const int border       = 4;

    for (int i = 0; i < borderLength; ++i)
    {
        for (int j = 0; j < borderLength; ++j)
        {
            for (int k = 0; k < borderLength; ++k)
            {
                // Vérifier si le point est à l'extérieur du cube
                if (i == 0 || i == borderLength - 1 || j == 0 || j == borderLength - 1 || k == 0 || k == borderLength - 1)
                {
                    float x = -borderLength + i * border; // Coordonnée x
                    float y = -borderLength + j * border; // Coordonnée y
                    float z = -borderLength + k * border; // Coordonnée z

                    Obstacle cubePoint(glm::vec3(x - border, y, z), 4, false, false);
                    _obstacles.push_back(cubePoint);
                }
            }
        }
    }
}

void ObstacleCollection::runObstacles(p6::Context& ctx)
{
    int i = 0;
    for (auto& obs : _obstacles)
    {
        obs.updateBorderPosition(ctx, i);
        ++i;
        // obs.draw(ctx);
    }
}
