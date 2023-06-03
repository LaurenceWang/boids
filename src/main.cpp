#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "app/App.hpp"
// #include "Light.hpp"
#include "doctest/doctest.h"
#include "glimac/sphere_vertices.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "rendering/Model.hpp"
#include "rendering/Object.hpp"
#include "rendering/Program.hpp"
#include "rendering/Texture.hpp"
#include "skybox/skybox.hpp"

using ObstacleHandler = std::function<void(Obstacle const&)>;

int main()
{
    auto ctx = p6::Context{{.title = "Finding Dory"}};

    glEnable(GL_DEPTH_TEST);

    App scene(ctx);

    ctx.update = [&]() {
        scene.cameraMovement(ctx);
        scene.sceneRender(ctx);
    };

    ctx.start();
    scene.sceneClean();
}