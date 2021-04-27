#include "SpecularShader.h"

#define ATTR_IDX0_POS    (0)
#define ATTR_IDX1_UV     (1)
#define ATTR_IDX2_NORMAL (2)

#define ATTR_IDX0_POS_STRD    (3)
#define ATTR_IDX1_UV_STRD     (2)
#define ATTR_IDX2_NORMAL_STRD (3)

static inline unsigned int calculateAllInOneStride() {
    unsigned int sum = 0;
    for (unsigned int i = 0; i < SpecularShader::max_attrNum; i++) {
        sum += SpecularShader::attr_stride[i];
    }

    return sum;
}

const unsigned int SpecularShader::attr_idx[max_attrNum] = {
    ATTR_IDX0_POS,
    ATTR_IDX1_UV, 
    ATTR_IDX2_NORMAL
};

const unsigned int SpecularShader::attr_stride[max_attrNum] = {
    ATTR_IDX0_POS_STRD,
    ATTR_IDX1_UV_STRD, 
    ATTR_IDX2_NORMAL_STRD
};

const unsigned int SpecularShader::attr_offset[max_attrNum] = {
    0, 
    attr_stride[0],
    attr_stride[0] + attr_stride[1]
};

const unsigned int SpecularShader::all_in_one_stride = calculateAllInOneStride();


// This is optional, attr idx could be set explicitly within shader
// in latter versions of gl
void SpecularShader::bindAllAttributeLocations() {
    printf("    override for setting attr-idx called.\n");

    // This binding takes effect when the program is linked the next time—it
    // does not change the bindings used by the currently linked program.
    bindAttributeLocation(ATTR_IDX0_POS,    "vertexPosition");
    bindAttributeLocation(ATTR_IDX1_UV,     "vertexUV");
    bindAttributeLocation(ATTR_IDX2_NORMAL, "vertexNormal");
}

void SpecularShader::getAllUniformLocations() {
    printf("    override for getting uniform-loc called.\n");

    // transform_loc = getUniformLocation("transformMatrix");
    // view_loc = getUniformLocation("viewMatrix");
    // proj_loc = getUniformLocation("projMatrix");
    setTransformMatLoc(getUniformLocation("transformMatrix"));
    setViewMatLoc(getUniformLocation("viewMatrix"));
    setProjMatLoc(getUniformLocation("projMatrix"));

    // printf("    unif_loc for mvp loc: %d, %d, %d\n\n", transform_loc, view_loc, proj_loc);
    printf("    unif_loc for mvp loc: %d, %d, %d\n\n", 
        getTransformMatLoc(), getViewMatLoc(), getProjMatLoc());

    lightPosition_loc = getUniformLocation("lightPosition");
    lightColor_loc = getUniformLocation("lightColor");
    printf("    unif_loc for light_pos/color: %d, %d\n\n", lightPosition_loc, lightColor_loc);

    objShineDamper_loc = getUniformLocation("shineDamper");
    objReflect_loc = getUniformLocation("reflectivity");
    printf("    unif_loc for reflec/shineDamper: %d, %d\n\n", objReflect_loc, objShineDamper_loc);
}
