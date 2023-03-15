#include <stdlib.h>
#include <cstddef>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Fish.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    unsigned                              seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937                          generator(seed);
    std::uniform_real_distribution<float> uniformRealDistribution(-1, 1);
    std::uniform_real_distribution<float> uniformRealDistribution2(-1.9, 1.9);
    std::uniform_real_distribution<float> uniformRealDistribution3(-0.001, 0.001);
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
    ctx.maximize_window();

    std::vector<Fish> boids;

    for (int i = 0; i < 100; ++i)
    {
        glm::vec2 coord = p6::random::point();
        glm::vec2 direc = p6::random::direction();
        float     velo  = p6::random::number(0, 0.001f);
        Speed     speed = Speed(direc, velo);
        Fish      b(coord, speed, 0.02f);
        boids.push_back(b);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);

        for (auto& boid : boids)
        {
            boid.applyForces(boids);
            boid.move();
            boid.drawFish(ctx);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}