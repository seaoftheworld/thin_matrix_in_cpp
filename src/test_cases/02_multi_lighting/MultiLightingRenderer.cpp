#include "MultiLightingRenderer.h"

bool MultiLightsRenderer::getEntityShaderStatus() {
    return entityShaderLinked;
}

void MultiLightsRenderer::allocEntityShader() {

    MultiLightsShader *shader = new MultiLightsShader;

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

    entityShader = dynamic_cast<BaseShader *>(shader);
    entityShaderLinked = true;

    // load projection matrix for the only once
    entityShader->start();
    entityShader->loadProjMatrix(getProjMatrix());
    entityShader->stop();
}

void MultiLightsRenderer::cleanUp() {
    // The specific shader's class-object is deleted here in derived-class
    if (entityShader) {
        entityShader->stop();
        entityShader->cleanUp();

        MultiLightsShader *shader = dynamic_cast<MultiLightsShader *>(entityShader);
        delete shader;

        entityShader = NULL;
    }

    // The list of entities to be rendered are cleared in the destructor of base-class
    // clearEntities();
}
