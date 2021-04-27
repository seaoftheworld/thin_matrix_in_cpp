#include "ModelRendererMultiLights.h"

void MultiLightsEntityRenderer::allocShadersData() {
    if (entityShader) {
        return;
    }

    MultiLightsShader *shader = new MultiLightsShader;

    if (!shader) {
        // shader is not allocated
        entityShader = NULL;
        return;
    }

    if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
        // shader failed to compile
        shader->cleanUp();
        shader = NULL;

        entityShader = NULL;
        return;
    }


    // load projection matrix for the only once
    shader->start();
    shader->loadProjMatrix(getProjMatrix());
    shader->stop();

    entityShader = shader;
}

void MultiLightsEntityRenderer::freeShadersData() {

    if (entityShader) {
        entityShader->stop();
        entityShader->cleanUp();

        delete entityShader;
        entityShader = NULL;
    }

    entities.clear();
    // lights.clear();
}

bool MultiLightsEntityRenderer::ready() {
    return (entityShader) ? (true) : (false);
}
