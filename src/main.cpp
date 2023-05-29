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

    const p6::Shader objshader = p6::load_shader(
        "Shaders/3D.vs.glsl",
        "Shaders/multiTex3D.fs.glsl"
    );

    const p6::Shader shader = p6::load_shader(
        "Shaders/skybox.vs.glsl",
        "Shaders/skybox.fs.glsl"
    );

    lightProgram shadow{};
    lightProgram debugSh("Shaders/depth.fs.glsl");
    Program      lightAndText{};
    Program      light("Shaders/pointLight.fs.glsl");
    Program      Objects("Shaders/multiTex3D.fs.glsl");
    Program      lum("Shaders/normals.fs.glsl");

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
    Model crystal("Assets/models/crystals/crystals.obj");

    Object lumiere(lum, vertices);

    Object crys(Objects, crystal, crysTex);

    Object lightSphere(light, vertices);
    Object lightAndTex(lightAndText, vertices, moonTexture);

    //  Object cube(Objects, cubeMod, cubeTexture);

    Object cube2(Objects, cubeMod, cubeTexture);

    Light test(shadow, vertices);
    Light test2(debugSh, vertices, moonTexture);

    Light cube(shadow, room);

    Light cubeSh(debugSh, room, defaulTex);

    Object roomT(Objects, cubeMod, defaulTex);

    App scene(ctx);

    /// objs

    /*Model   modPortal2("Assets/models/archPortal/portal1.obj");
    Texture texPortal2("Assets/models/archPortal/portal1.png", 0);
    Object  portal2(Objects, modPortal2, texPortal2);*/

    Model   modPortal("Assets/models/portal/portal.obj");
    Texture texPortal("Assets/models/portal/portal1.png", 0);
    Object  portal(Objects, modPortal, texPortal);

    /*Model   modIsl("Assets/models/island_1/island-1-mid.obj");
    Texture texIsl("Assets/models/island_1/textures/material_0_diffuse.png", 0);
    Object  isl(Objects, modIsl, texIsl);*/

    Model   modIsl("Assets/models/island/island.obj");
    Texture texIsl("Assets/models/island/island.png", 0);
    Object  isl(Objects, modIsl, texIsl);

    /*Model   modChest("Assets/models/pink-chest/chest-low.obj");
    Texture texChest("Assets/models/pink-chest/textures/lambert6_Base_Color.png", 0);
    Object  chest(Objects, modChest, texChest);*/

    /*Model   modChest("Assets/models/pink-chest/chest-low.obj");
    Texture texChest("Assets/models/pink-chest/textures/lambert6_Base_Color.png", 0);
    Object  chest(Objects, modChest, texChest);*/

    Model   modChest("Assets/models/chest/chest-test.obj");
    Texture texChest("Assets/models/chest/chest.png", 0);
    Object  chest(Objects, modChest, texChest);

    /*Model   modCloud("Assets/models/stylized_clouds/cloud-low.obj");
    Texture texCloud("Assets/models/stylized_clouds/textures/base1_baseColor_3.jpg", 0);
    Object  cloud(Objects, modCloud, texCloud);*/

    Model   modCloud("Assets/models/cloud/cloud-low.obj");
    Texture texCloud("Assets/models/cloud/cloud.jpg", 0);
    Object  cloud(Objects, modCloud, texCloud);

    /*Model   modRock("Assets/models/stylized_rocks/rocks.obj");
    Texture texRock("Assets/models/stylized_rocks/textures/TCE_Rocks_v02_Jungle_Mat_baseColor.jpeg", 0);
    Object  rock(Objects, modRock, texRock);*/

    /*Model   modPinkTr("Assets/models/organics_small_tree/tree.obj");
    Texture texPinkTr("Assets/models/organics_small_tree/textures/pinkTree.jpg", 0);
    Object  pinkTr(Objects, modPinkTr, texPinkTr);*/

    Model   modPinkTr("Assets/models/tree/tree.obj");
    Texture texPinkTr("Assets/models/tree/tree.jpg", 0);
    Object  pinkTr(Objects, modPinkTr, texPinkTr);

    /*Model   modCastle("Assets/models/demonologists-castle/castle-cry.obj");
    Texture texCastle("Assets/models/demonologists-castle/textures/castle-main-1024.png", 0);
    Object  castle(Objects, modCastle, texCastle);*/

    Model   modCastle("Assets/models/castle/castle.obj");
    Texture texCastle("Assets/models/castle/castle.png", 0);
    Object  castle(Objects, modCastle, texCastle);

    /*Model   modFishIsl("Assets/models/fish-isl/island-fish.obj");
    Texture texFishIsl("Assets/models/fish-isl/textures/Image_13.png", 0);
    Object  fishIsl(Objects, modFishIsl, texFishIsl);*/

    Model   modHorPlant("Assets/models/horrorific_plant/horrific-plant-low.obj");
    Texture texHorPlant("Assets/models/horrorific_plant/textures/lambert3SG1_baseColor.png", 0);
    Object  horPlant(Objects, modHorPlant, texHorPlant);

    /*Model   modPlant("Assets/models/set-plant-exotic-fantasy/exotic-plant-low.obj");
    Texture texPlant("Assets/models/set-plant-exotic-fantasy/textures/Plant-5_Plant-5_BaseColor.png", 0);
    Object  plant(Objects, modPlant, texPlant);*/

    /*Model   modR("Assets/models/stylized-rock/floating-rock.obj");
    Texture texR("Assets/models/stylized-rock/textures/Default_2D_View.png", 0);
    Object  floatinRock(Objects, modR, texR);*/

    Model   modR("Assets/models/rock/rock.obj");
    Texture texR("Assets/models/rock/rock.png", 0);
    Object  floatinRock(Objects, modR, texR);

    /*Model   modTr("Assets/models/tree-speed-sculpt/tree-1.obj");
    Texture texTr("Assets/models/tree-speed-sculpt/textures/Leaves.png", 0);
    Object  tr(Objects, modTr, texTr);*/

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

    // glm::vec3 lightInvDir = glm::vec3(5, 1, -5);

    glm::vec3 lightInvDir = glm::vec3(3, 1, -3);

    glm::vec3 lightPos(0.f, 0.f, 0.0f);
    glm::vec3 lightPos2(9.f, 2.f, -13.0f);

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

        ////////////////////
        /*objectParameters portalParameters{glm::vec3(0, -1.5, 0), 0.f, 1};
        portal.createDrawEnvironment(ctx);
        portal.draw(scene.getCamera().getViewMatrix(), glm::vec3(0, -1.5, 0), 0.f, 1);
        portal.debindVAO();*/

        /*isl.createDrawEnvironment(ctx);
        isl.draw(scene.getCamera().getViewMatrix(), glm::vec3(0, -1.5, 0), 45.f, 1);
        isl.draw(scene.getCamera().getViewMatrix(), glm::vec3(-9, 3, -9), 45.f, 1);
        isl.debindVAO();

        pinkTr.createDrawEnvironment(ctx);
        pinkTr.draw(scene.getCamera().getViewMatrix(), glm::vec3(-8.3, 3.3, -9), 0.f, 1);
        pinkTr.draw(scene.getCamera().getViewMatrix(), glm::vec3(-10, 3.3, -10), 0.f, 0.7);
        pinkTr.debindVAO();

        chest.createDrawEnvironment(ctx);
        chest.draw(scene.getCamera().getViewMatrix(), glm::vec3(-9.2, 3.3, -8.7), 180.f, 1);
        chest.debindVAO();*/

        /*cloud.createDrawEnvironment(ctx);
        cloud.draw(scene.getCamera().getViewMatrix(), glm::vec3(5, 0, 0), 0.f, 1);
        cloud.debindVAO();*/

        /*rock.createDrawEnvironment(ctx);
        rock.draw(scene.getCamera().getViewMatrix(), glm::vec3(-3, 0, 0), 0.f, 1);
        rock.debindVAO();*/

        /*castle.createDrawEnvironment(ctx);
        castle.draw(scene.getCamera().getViewMatrix(), glm::vec3(12, 2, -13), 90.f, 0.6);
        castle.debindVAO();*/

        /*fishIsl.createDrawEnvironment(ctx);
        fishIsl.draw(scene.getCamera().getViewMatrix(), glm::vec3(-5, 0, 0), 0.f, 1);
        fishIsl.debindVAO();*/

        /*horPlant.createDrawEnvironment(ctx);
        horPlant.draw(scene.getCamera().getViewMatrix(), glm::vec3(-5, 0, 0), 0.f, 0.05);
        horPlant.debindVAO();*/

        /*plant.createDrawEnvironment(ctx);
        plant.draw(scene.getCamera().getViewMatrix(), glm::vec3(-5, 0, 0), 0.f, 0.05);
        plant.debindVAO();*/

        /*floatinRock.createDrawEnvironment(ctx);
        floatinRock.draw(scene.getCamera().getViewMatrix(), glm::vec3(-1.7, -7, 2.5), 0.f, 0.7);
        floatinRock.draw(scene.getCamera().getViewMatrix(), glm::vec3(3.2, -7, -2), 0.f, 0.7);

        floatinRock.draw(scene.getCamera().getViewMatrix(), glm::vec3(-2, -3, -5), 0.f, 1);
        floatinRock.draw(scene.getCamera().getViewMatrix(), glm::vec3(-4, -5.5, -0.5), 0.f, 0.8);
        floatinRock.draw(scene.getCamera().getViewMatrix(), glm::vec3(2, -4.7, -8), 0.f, 0.8);
        floatinRock.debindVAO();*/

        /*tr.createDrawEnvironment(ctx);
        tr.draw(scene.getCamera().getViewMatrix(), glm::vec3(-5, 0, 0), 0.f, 10);
        tr.debindVAO();*/

        ////////lumiere
        /*roomT.createDrawEnvironment(ctx);
        roomT.draw(scene.getCamera().getViewMatrix(), glm::vec3(0, 0, 0), 0.f, 1);
        roomT.debindVAO();*/

        lumiere.createDrawEnvironment(ctx);
        lumiere.draw(scene.getCamera().getViewMatrix(), lightparameters);
        lumiere.debindVAO();
        //////////////

        /*lightSphere.createDrawEnvironment(ctx);
        lightSphere.draw(scene.getCamera().getViewMatrix(), glm::vec3(4, 0, -5), 20, 3);
        lightSphere.debindVAO();*/

        /*crys.createDrawEnvironment(ctx);
        crys.draw(scene.getCamera().getViewMatrix(), glm::vec3(1.5, -1.15, 0), 0, 1);
        crys.draw(scene.getCamera().getViewMatrix(), glm::vec3(-1.5, -1.15, 0), 0, 1);
        crys.draw(scene.getCamera().getViewMatrix(), glm::vec3(0, -1.15, -1), 0, 0.9);
        crys.draw(scene.getCamera().getViewMatrix(), glm::vec3(0.7, -1.15, 0.5), 0, 0.9);
        crys.draw(scene.getCamera().getViewMatrix(), glm::vec3(-0.2, -1.15, 0.6), 45, 0.8);
        crys.debindVAO();
        // ctx.maximize_window()*/
        ;

        ////////////////////////
        /*glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
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
        scene.sceneRender(ctx);*/

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
    isl.deleteVBO_VAO();
}