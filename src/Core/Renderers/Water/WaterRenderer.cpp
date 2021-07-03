#include "WaterRenderer.h"

void WaterRenderer::allocShadersData() {

    if (waterShader) {
        return;
    }

    // Compile and Link the water-shader
    WaterShader *shader = new WaterShader;
    
    if (!shader) {
        // shader is not allocated
        waterShader = NULL;
        return;
    }

    if (shader->getStatus() != BaseShader::link_prog_passed || shader->getProgId() <= 0) {
        // shader failed to compile
        printf("waterShader status: %d\n", shader->getStatus());
        shader->cleanUp();
        shader = NULL;

        waterShader = NULL;
        return;
    }

    // load projection matrix for the only once
    shader->start();
        shader->loadProjMatrix(getProjMatrix());
        shader->bindTextureUnits();
        shader->setDefaultLightPosition();
    shader->stop();

    waterShader = shader;
}

void WaterRenderer::freeShadersData() {

    // The specific shader's class-object is deleted here in derived-class
    if (waterShader) {
        waterShader->stop();
        waterShader->cleanUp();

        delete waterShader;
        waterShader = NULL;
    }
    waters.clear();
}

bool WaterRenderer::ready() {
    return (waterShader) ? (true) : (false);
}
