#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "App.hpp"
// #include "Light.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "doctest/doctest.h"
#include "glimac/sphere_vertices.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "skybox/skybox.hpp"

using ObstacleHandler = std::function<void(Obstacle const&)>;

int main()
{
    auto                                   ctx      = p6::Context{{.title = "Finding Dory"}};
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(0.2f, 32, 16);

    Program Objects("Shaders/multiTex3D.fs.glsl");
    Program lum("Shaders/normals.fs.glsl");

    glEnable(GL_DEPTH_TEST);

    // Object lumiere(lum, vertices);

    App scene(ctx);

    glm::vec3        lightPos(0.f, 0.f, 0.0f);
    objectParameters lightparameters{lightPos, 0, 1};

    ctx.update = [&]() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080);
        ctx.background({0.2, 0.2, 0.2});

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.cameraMovement(ctx);
        scene.sceneRender(ctx);

        /*lumiere.createDrawEnvironment(ctx);
        lumiere.draw(scene.getCamera().getViewMatrix(), lightparameters);
        lumiere.debindVAO();*/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    scene.sceneClean();
}