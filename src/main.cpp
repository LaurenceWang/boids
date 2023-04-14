#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Boids.hpp"
#include "Food.hpp"
#include "ObstacleCollection.hpp"
#include "Params.hpp"
#include "doctest/doctest.h"
#include "imgui.hpp"

using ObstacleHandler = std::function<void(Obstacle const&)>;

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Swimming with boids"}};
    // ctx.maximize_window();
    int    fishNb = 100;
    Params p{.separation = 0.001f, .alignment = 0.02f, .steer = 1.5f, .neighRadius = 0.1f, .fishSize = 0.02f};
    bool   nbChanged   = false;
    bool   sizeChanged = false;

    Boids boids;
    boids.generateFish(fishNb, p.fishSize, 0);
    boids.generateFish(20, p.fishSize, 1);

    imGuiInit(&ctx, p, fishNb, boids);

    ObstacleCollection obstacle;

    obstacle.generateObstacles(3);
    obstacle.generateBorders(ctx);

    ObstacleCollection obstacle2;

    obstacle.generateObstacles(2);

    Food              seaweed;
    Food              seaweed2;
    std::vector<Food> meals;
    meals.push_back(seaweed);
    meals.push_back(seaweed2);

    // Declare your infinite update loop.

    ctx.update = [&]() {
        ctx.background({0.33, 0.8, 0.98});
        seaweed.draw(ctx);
        seaweed2.draw(ctx);

        const auto for_each_obstacle = [&](ObstacleHandler const& handler) {
            for (auto const& obs : obstacle.getObstacles())
            {
                handler(obs);
            }

            for (auto const& obs : obstacle2.getObstacles())
            {
                handler(obs);
            }
        };
        ;
        boids.runBoids(p, ctx, for_each_obstacle, meals);
        obstacle.runObstacles(ctx);
        obstacle2.runObstacles(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
