#include "imgui.hpp"
#include <pthread.h>

void imGuiInit(p6::Context* ctx, Params& p, int& fishNb, Boids& boids, int& lodChoice)
{
    (*ctx).imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids sliders");
        if (ImGui::SliderInt("fish number", &fishNb, 1, 250))
        {
            boids.adjustBoids(fishNb);
        };
        if (ImGui::SliderFloat("fish size", &p.fishSize, 0.02f, 0.08f))
        {
            boids.resizeBoids(p.fishSize);
        };
        ImGui::SliderFloat("separation strength", &p.separation, 0.1f, 1.f);
        ImGui::SliderFloat("alignment strength", &p.alignment, 0.3f, 0.6f);
        ImGui::SliderFloat("cohesion strength", &p.steer, 1.f, 30.f);
        ImGui::SliderFloat("neighbour radius", &p.neighRadius, 0.5f, 6.f);
        if (ImGui::Button("Low"))
        {
            lodChoice = 1;
        };
        if (ImGui::Button("High"))
        {
            lodChoice = 0;
        };
        if (ImGui::Button("Adaptative"))
        {
            lodChoice = 2;
        };

        ImGui::End();
    };
}