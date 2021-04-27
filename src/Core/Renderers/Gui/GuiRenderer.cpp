#include "GuiRenderer.h"

void GuiRenderer::allocShadersData() {

    if (guiShader) {
        return;
    }

    // Compile and Link the gui-shader
    GuiType00Shader *shader = new GuiType00Shader;
    
    if (!shader) {
        // shader is not allocated
        guiShader = NULL;
        return;
    }

    if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
        // shader failed to compile
        printf("guiShader status: %d\n", guiShader->getStatus());
        shader->cleanUp();
        shader = NULL;

        guiShader = NULL;
        return;
    }

    // printf("guiShader ok\n");
    // load projection matrix for the only once
    shader->start();
        // // guiShader doesn't support projection/view_matrix now
        // guiShader->loadProjMatrix(getProjMatrix());
    shader->stop();

    guiShader = shader;
}

void GuiRenderer::freeShadersData() {

    // The specific shader's class-object is deleted here in derived-class
    if (guiShader) {
        guiShader->stop();
        guiShader->cleanUp();

        delete guiShader;
        guiShader = NULL;
    }
    
    guis.clear();
}

bool GuiRenderer::ready() {
    return (guiShader) ? (true) : (false);
}
