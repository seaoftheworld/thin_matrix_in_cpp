#include "EntityTerrainRenderer.h"

bool EntityTerrainRenderer::getEntityAndTerrainShaderStatus() {
    return (entityShaderLinked && terrainShaderLinked);
}

void EntityTerrainRenderer::specificSettingsOn() {
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

void EntityTerrainRenderer::specificSettingsOff() {
}

void EntityTerrainRenderer::allocEntityShader() {

    // Compile and Link the entity shader
    {
        SpecularShader *shader = new SpecularShader;

        // shader is not allocated
        if (!shader) {
            entityShaderLinked = false;
        }

        // shader failed to compile
        if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
            shader->cleanUp();
            shader = NULL;
            entityShaderLinked = false;
        }

        entityShaderLinked = true;
        entityShader = shader;

        // load projection matrix for the only once
        entityShader->start();
        entityShader->loadProjMatrix(getProjMatrix());
        entityShader->stop();
    }

    // Compile and Link the terrain shader
    {
        TerrainShader *shader = new TerrainShader;

        // shader is not allocated
        if (!shader) {
            terrainShaderLinked = false;
        }

        // shader failed to compile
        if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
            shader->cleanUp();
            shader = NULL;
            terrainShaderLinked = false;
        }

        terrainShaderLinked = true;
        terrainShader = shader;

        // load projection matrix for the only once
        terrainShader->start();
        terrainShader->loadProjMatrix(getProjMatrix());
        terrainShader->stop();
    }
}

void EntityTerrainRenderer::cleanUp() {
    // The specific shader's class-object is deleted here in derived-class
    if (entityShader) {
        entityShader->stop();
        entityShader->cleanUp();

        delete entityShader;
        entityShader = NULL;
    }

    if (terrainShader) {
        terrainShader->stop();
        terrainShader->cleanUp();

        delete terrainShader;
        terrainShader = NULL;
    }

    // The list of entities to be rendered are cleared in the destructor of base-class
    // clearEntities();
    terrains.clear();
}