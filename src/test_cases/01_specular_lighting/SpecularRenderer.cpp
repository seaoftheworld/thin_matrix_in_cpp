#include "SpecularRenderer.h"

bool SpecularRenderer::getEntityShaderStatus() {
    return entityShaderLinked;
}

void SpecularRenderer::allocEntityShader() {

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

    entityShader = dynamic_cast<BaseShader *>(shader);
    entityShaderLinked = true;

    // load projection matrix for the only once
    entityShader->start();
    entityShader->loadProjMatrix(getProjMatrix());
    entityShader->stop();
}

void SpecularRenderer::cleanUp() {
    // The specific shader's class-object is deleted here in derived-class
    if (entityShader) {
        entityShader->stop();
        entityShader->cleanUp();

        SpecularShader *shader = dynamic_cast<SpecularShader *>(entityShader);
        delete shader;

        entityShader = NULL;
    }

    // The list of entities to be rendered are cleared in the destructor of base-class
    // clearEntities();
}
