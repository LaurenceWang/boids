#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "App.hpp"
#include "Light.hpp"
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
    auto                                   ctx      = p6::Context{{.title = "Swimming with Nemo"}};
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(0.2f, 32, 16);

    const p6::Shader shader = p6::load_shader(
        "Shaders/skybox.vs.glsl",
        "Shaders/skybox.fs.glsl"
    );

    lightProgram shadow{};
    lightProgram debugSh("Shaders/depth.fs.glsl");
    Program      lightAndText{};
    Program      light("Shaders/pointLight.fs.glsl");
    Program      Objects("Shaders/multiTex3D.fs.glsl");

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    Texture defaulTex("Assets/textures/default_tex.png", 5);
    Texture moonTexture("Assets/textures/MoonMap.jpg", 3);
    Texture cubeTexture("Assets/models/portal-companion-cube/textures/Portal_Companion_Cube.png", 7);
    Texture crysTex("Assets/models/crystal/Crystal_Albedo.png", 0);

    //  Model cubeMod("Assets/models/portal-companion-cube/Portal_Companion_Cube.obj");
    Model cubeMod("Assets/models/cube-test.obj");
    Model room("Assets/models/room.obj");
    Model crystal("Assets/models/crystal/crystal.obj");

    Object crys(Objects, crystal, crysTex);

    Object lightSphere(light, vertices);
    Object lightAndTex(lightAndText, vertices, moonTexture);

    //  Object cube(Objects, cubeMod, cubeTexture);

    Object cube2(Objects, cubeMod, cubeTexture);

    Light test(shadow, vertices);
    Light test2(debugSh, vertices, moonTexture);

    Light cube(shadow, room);

    Light cubeSh(debugSh, room, defaulTex);

    Object roomT(Objects, room, cubeTexture);

    App scene(ctx);

    //////////////

    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    std::array<float, 4> borderColor = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;

    glm::vec3 lightInvDir = glm::vec3(2, 2, -5);

    ctx.update = [&]() {
        // ctx.maximize_window();

        ////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
        // ctx.background({0, 0, 0});
        ctx.background({0.33, 0.8, 0.98});
        glViewport(0, 0, 1920, 1080);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glClear(GL_DEPTH_BUFFER_BIT);

        cube.createDrawEnvironment(ctx);

        cube.draw(lightInvDir, glm::vec3(0, 0, 0), 0.F, 1);
        cube.debindVAO();

        test.createDrawEnvironment(ctx);

        test.draw(lightInvDir, glm::vec3(0, 0, 0), 0.F, 1);
        test.debindVAO();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 ViewMatrix = scene.getCamera().getViewMatrix();

        cubeSh.createDrawEnvironment(ctx);

        cubeSh.draw2(ViewMatrix, glm::vec3(0, 0, 0), 0., 1., lightInvDir, depthTexture);
        cubeSh.debindVAO();

        test2.createDrawEnvironment(ctx);

        test2.draw2(ViewMatrix, glm::vec3(2, 2, 2), 0., 2., lightInvDir, depthTexture);
        test2.debindVAO();

        glm::mat4 ViewMatrix2 = glm::mat4(1);
        ViewMatrix2           = glm::translate(ViewMatrix2, glm::vec3(0, 0, -5));

        lightSphere.createDrawEnvironment(ctx);
        lightSphere.draw(ViewMatrix, glm::vec3(4, 4, -5), 20, 3);
        lightSphere.debindVAO();

        crys.createDrawEnvironment(ctx);
        crys.draw(ViewMatrix, glm::vec3(2, 4, -5), 0, 1);
        crys.debindVAO();

        scene.cameraMovement(ctx);
        scene.sceneRender(ctx);

        /**********/

        /*cube2.createDrawEnvironment(ctx);
        cube2.draw(scene.getCamera().getViewMatrix(), glm::vec3(0, 5, 5), 0.f, 15);
        // cube.draw(scene.getCamera().getViewMatrix(), glm::vec3(0, 0, -5), 0.f, 1);
        cube2.debindVAO();*/

        /*lightSphere.createDrawEnvironment(ctx);
        lightSphere.draw(scene.getCamera().getViewMatrix(), glm::vec3(0, 0, -5), 0.f, 2);
        lightSphere.debindVAO();

        lightAndTex.createDrawEnvironment(ctx);
        lightAndTex.draw(scene.getCamera().getViewMatrix(), glm::vec3(-4, -2, -5), 0.f, 2);
        lightAndTex.debindVAO();*/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    scene.sceneClean();
    lightSphere.deleteVBO_VAO();
    lightAndTex.deleteVBO_VAO();
    cube.deleteVBO_VAO();
}