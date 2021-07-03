#include "WaterShader.h"

#define ATTR_IDX0_POS      (0)


// This is optional, attr idx could be set explicitly within shader
// in latter versions of gl
void WaterShader::bindAllAttributeLocations() {
    printf("    override for setting attr-idx called.\n");

    // This binding takes effect when the program is linked the next time—it
    // does not change the bindings used by the currently linked program.
    bindAttributeLocation(ATTR_IDX0_POS,    "position");
}

void WaterShader::getAllUniformLocations() {
    printf("    override for getting uniform-loc called.\n");
    setTransformMatLoc(getUniformLocation("modelMatrix"));
    setViewMatLoc(getUniformLocation("viewMatrix"));
    setProjMatLoc(getUniformLocation("projMatrix"));

    printf("    water-shader's mvp unif_locs: %d, %d, %d \n\n", 
        getTransformMatLoc(), getViewMatLoc(), getProjMatLoc() );

    reflectionTexture_loc = getUniformLocation("reflectionTexture");
    refractionTexture_loc = getUniformLocation("refractionTexture");
    dudvTexture_loc = getUniformLocation("dudvTexture");
    normalTexture_loc = getUniformLocation("normalTexture");
    printf("    \
        water-shader's reflection/refraction, dudv, normal texutures unif_locs: \
        %d, %d, %d, %d\n",
        reflectionTexture_loc, refractionTexture_loc, dudvTexture_loc, normalTexture_loc);

    moveSpeed_loc = getUniformLocation("move_speed");
    printf("moveFactor_loc: %d\n\n", moveSpeed_loc);

    lightPosition_loc = getUniformLocation("lightPosition");
}
