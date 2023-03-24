#include <stdlib.h>
#include <cstddef>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Boids.hpp"
#include "Fish.hpp"
#include "Params.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    /*unsigned                              seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937                          generator(seed);
    std::uniform_real_distribution<float> uniformRealDistribution(-1, 1);
    std::uniform_real_distribution<float> uniformRealDistribution2(-1.9, 1.9);
    std::uniform_real_distribution<float> uniformRealDistribution3(-0.001, 0.001);*/
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
    auto        fishNb = 100;
    std::string text   = "Hello";

    Params p{0.00001f, 0.02f, 0.015f};
    // std::vector<Fish> boids;

    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids sliders");
        ImGui::SliderInt("fish number", &fishNb, 50, 150);
        ImGui::SliderFloat("separation strength", &p.sepStr, 0.00001f, 0.0001f);
        ImGui::SliderFloat("alignment strength", &p.aliStr, 0.005f, 0.05f);
        ImGui::SliderFloat("cohestion strength", &p.steerStr, 0.005, 0.03);
        ImGui::InputText("Text", &text);
        ImGui::End();

        // ImGui::ShowDemoWindow();
    };

    std::cout << fishNb << std::endl;

    /*for (int i = 0; i < fishNb; ++i)
    {
        glm::vec2 coord = p6::random::point();
        glm::vec2 direc = p6::random::direction();
        float     velo  = p6::random::number(0, 0.001f);
        Speed     speed = Speed(direc, velo);
        Fish      b(coord, speed, 0.02f);
        boids.push_back(b);
    }*/

    Boids boids;
    boids.generateFish(fishNb);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);

        /*for (auto& boid : boids)
        {
            boid.applyForces(boids, p);
            boid.move();
            boid.drawFish(ctx);
        }*/
        boids.runBoids(p, ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}