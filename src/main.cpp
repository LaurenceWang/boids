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
    Params p{0.001f, 0.02f, 1.5f, 0.1f, 0.02f};
    bool   nbChanged   = false;
    bool   sizeChanged = false;
    imguiinit(&ctx, p, fishNb, nbChanged, sizeChanged);

    Boids boids;
    boids.generateFish(fishNb, p.fishSize, 0);
    boids.generateFish(20, p.fishSize, 1);

    ObstacleCollection obstacle;

    obstacle.generateObstacles(3);
    obstacle.generateBorders(ctx);

    Food              seaweed;
    std::vector<Food> meals;
    meals.push_back(seaweed);

    // Declare your infinite update loop.

    ctx.update = [&]() {
        if (nbChanged)
        {
            boids.adjustBoids(fishNb, p.fishSize);
        }
        if (sizeChanged)
        {
            boids.resizeBoids(p.fishSize);
        }

        ctx.background({0.33, 0.8, 0.98});
        seaweed.draw(ctx);
        boids.runBoids(p, ctx, obstacle.getObstacles(), meals);
        obstacle.runObstacles(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
