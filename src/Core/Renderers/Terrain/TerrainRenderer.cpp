#include "TerrainRenderer.h"

void TerrainRenderer::allocShadersData() {

    if (terrainShader) {
        return;
    }

    TerrainShader *shader = new TerrainShader;

    if (!shader) {
        // shader is not allocated
        terrainShader = NULL;
        return;
    }

    if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
        // shader failed to compile
        shader->cleanUp();
        shader = NULL;

        terrainShader = NULL;
        return;
    }

    // load projection matrix for the only once
    shader->start();
        shader->loadProjMatrix(getProjMatrix());
        shader->bindTextureUnits();
    shader->stop();

    terrainShader = shader;
}

void TerrainRenderer::freeShadersData() {

    if (terrainShader) {
        terrainShader->stop();
        terrainShader->cleanUp();

        delete terrainShader;
        terrainShader = NULL;
    }

    terrains.clear();
}

bool TerrainRenderer::ready() {
    return (terrainShader) ? (true) : (false);
}
