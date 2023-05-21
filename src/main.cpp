#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "App.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "doctest/doctest.h"
#include "glimac/sphere_vertices.hpp"

using ObstacleHandler = std::function<void(Obstacle const&)>;

int main()
{
    auto                                   ctx      = p6::Context{{.title = "Swimming with Nemo"}};
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(0.2f, 32, 16);

    Program lightAndText{};
    Program light("Shaders/pointLight.fs.glsl");

    glEnable(GL_DEPTH_TEST);
    Texture moonTexture("Assets/textures/MoonMap.jpg", 3);

    Object lightSphere(light, vertices);
    Object lightAndTex(lightAndText, vertices, moonTexture);

    App scene(ctx);

    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ctx.background({0.33, 0.8, 0.98});
        scene.cameraMovement(ctx);
        scene.sceneRender(ctx);

        /*************LIGHT TESTS*****************/

        lightSphere.createDrawEnvironment(ctx);
        lightSphere.draw(scene.getCamera(), glm::vec3(0, 0, -5), 0.f, 2);
        lightSphere.debindVAO();

        lightAndTex.createDrawEnvironment(ctx);
        lightAndTex.draw(scene.getCamera(), glm::vec3(-4, -2, -5), 0.f, 2);
        lightAndTex.debindVAO();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    scene.sceneClean();
    lightSphere.deleteVBO_VAO();
    lightAndTex.deleteVBO_VAO();
}