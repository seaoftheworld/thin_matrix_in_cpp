#include "ModelRendererSpecular.h"

void SpecularEntityRenderer::allocShadersData() {
    if (entityShader) {
        return;
    }

    // StaticShader *shader = new StaticShader;
    SpecularShader *shader = new SpecularShader;

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

void SpecularEntityRenderer::freeShadersData() {

    if (entityShader) {
        entityShader->stop();
        entityShader->cleanUp();

        delete entityShader;
        entityShader = NULL;
    }

    entities.clear();
}

bool SpecularEntityRenderer::ready() {
    return (entityShader) ? (true) : (false);
}
