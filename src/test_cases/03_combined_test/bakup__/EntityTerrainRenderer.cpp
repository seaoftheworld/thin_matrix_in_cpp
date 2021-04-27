#include "EntityTerrainRenderer.h"

void WrappingRenderer::specificSettingsOn() {
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE); // Usually enabled by default in driver

    // glEnable(GL_CULL_FACE);  // requried the model to be enclosed, otherwise
    // glCullFace(GL_BACK);     // will not be correctly displayed ???
    // glCullFace(GL_FRONT);
}

void WrappingRenderer::specificSettingsOff() {
}

bool VersatileRenderer::getEntityAndTerrainShaderStatus() {
    // return (entityShader && terrainShader && skyboxShader);
    // return (entityShader);
    return false;
}

// void VersatileRenderer::specificSettingsOn() {
//     glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
//     glEnable(GL_DEPTH_TEST);
//     // glDepthFunc(GL_LESS);

//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     glEnable(GL_MULTISAMPLE); // Usually enabled by default in driver

//     // glEnable(GL_CULL_FACE);  // requried the model to be enclosed, otherwise
//     // glCullFace(GL_BACK);     // will not be correctly displayed ???
//     // glCullFace(GL_FRONT);
// }

// void VersatileRenderer::specificSettingsOff() {
// }

void VersatileRenderer::allocShaders() {

    // Compile and Link the entity shader
    {
        // SpecularShader *shader = new SpecularShader;

        // if (!shader) {
        //     // shader is not allocated
        // }
        // else if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
        //     // shader failed to compile
        //     shader->cleanUp();
        //     shader = NULL;
        // }
        // else {
        //     entityShader = shader;

        //     // load projection matrix for the only once
        //     entityShader->start();
        //     entityShader->loadProjMatrix(getProjMatrix());
        //     entityShader->stop();
        // }
    }

    // Compile and Link the terrain shader
    {
        // TerrainShader *shader = new TerrainShader;

        // if (!shader) {
        //     // shader is not allocated
        // }
        // else if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
        //     // shader failed to compile
        //     shader->cleanUp();
        //     shader = NULL;
        // }
        // else {
        //     terrainShader = shader;

        //     // load projection matrix for the only once
        //     terrainShader->start();
        //     terrainShader->loadProjMatrix(getProjMatrix());
        //     terrainShader->bindTextureUnits();
        //     terrainShader->stop();
        // }
    }

    // Compile and Link the skybox shader
    {
        // SkyboxShader *shader = new SkyboxShader;

        // if (!shader) {
        //     // shader is not allocated
        // }
        // else if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
        //     // shader failed to compile
        //     shader->cleanUp();
        //     shader = NULL;
        // }
        // else {
        //     skyboxShader = shader;

        //     // load projection matrix for the only once
        //     skyboxShader->start();
        //     skyboxShader->loadProjMatrix(getProjMatrix());
        //     skyboxShader->stop();
        // }
    }
}

void VersatileRenderer::cleanUp() {
    // The specific shader's class-object is deleted here in derived-class
    // if (entityShader) {
    //     entityShader->stop();
    //     entityShader->cleanUp();

    //     delete entityShader;
    //     entityShader = NULL;
    // }

    // if (terrainShader) {
    //     terrainShader->stop();
    //     terrainShader->cleanUp();
    //     delete terrainShader;
    //     terrainShader = NULL;
    // }

    // if (skyboxShader) {
    //     skyboxShader->stop();
    //     skyboxShader->cleanUp();

    //     delete skyboxShader;
    //     skyboxShader = NULL;
    // }

    // guiRenderer.cleanUp();

    // The list of entities to be rendered are cleared in the destructor of base-class
    // clearEntities();
    // terrains.clear();
}
