#include "SkyboxShader.h"

#define ATTR_IDX0_POS      (0)
#define ATTR_IDX0_POS_STRD (3)

static inline unsigned int calculateAllInOneStride() {
    unsigned int sum = 0;
    for (unsigned int i = 0; i < SkyboxShader::max_attrNum; i++) {
        sum += SkyboxShader::attr_stride[i];
    }

    return sum;
}

const unsigned int SkyboxShader::attr_idx[max_attrNum] = {
    ATTR_IDX0_POS
};

const unsigned int SkyboxShader::attr_stride[max_attrNum] = {
    ATTR_IDX0_POS_STRD
};

const unsigned int SkyboxShader::attr_offset[max_attrNum] = {
    0
};

const unsigned int SkyboxShader::all_in_one_stride = calculateAllInOneStride();

// This is optional, attr idx could be set explicitly within shader
// in latter versions of gl
void SkyboxShader::bindAllAttributeLocations() {
    printf("    override for setting attr-idx called.\n");

    // This binding takes effect when the program is linked the next time --- 
    // it does not change the bindings used by the currently linked program.
    bindAttributeLocation(ATTR_IDX0_POS,    "vertexPosition");
    // bindAttributeLocation(ATTR_IDX1_UV,     "vertexUV");
    // bindAttributeLocation(ATTR_IDX2_NORMAL, "vertexNormal");
}

void SkyboxShader::getAllUniformLocations() {
    printf("    override for getting uniform-loc called.\n");

    setViewMatLoc(getUniformLocation("viewMatrix"));
    setProjMatLoc(getUniformLocation("projMatrix"));
    printf("    unif_loc for view/proj matrix loc: %d, %d\n\n", 
        getViewMatLoc(), 
        getProjMatLoc());
}
