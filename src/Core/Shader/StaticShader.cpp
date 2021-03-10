#include "StaticShader.h"

#define ATTR_IDX0_POS    (0)
#define ATTR_IDX1_UV     (1)
#define ATTR_IDX2_NORMAL (2)

#define ATTR_IDX0_POS_STRD    (3)
#define ATTR_IDX1_UV_STRD     (2)
#define ATTR_IDX2_NORMAL_STRD (3)

const unsigned int StaticShader::attr_idx[max_attrNum] = {
    ATTR_IDX0_POS,
    ATTR_IDX1_UV, 
    ATTR_IDX2_NORMAL
};

const unsigned int StaticShader::attr_stride[max_attrNum] = {
    ATTR_IDX0_POS_STRD,
    ATTR_IDX1_UV_STRD, 
    ATTR_IDX2_NORMAL_STRD
};

const unsigned int StaticShader::attr_offset[max_attrNum] = {
    0, 
    attr_stride[0],
    attr_stride[0] + attr_stride[1]
};

const unsigned int StaticShader::all_in_one_stride = 
    (attr_stride[0] + attr_stride[1] + attr_stride[2]);



void StaticShader::bindAllAttributeLocations() {
    printf("    override for setting attr-idx called.\n");

    // This binding takes effect when the program is linked the next time—it
    // does not change the bindings used by the currently linked program.
        // bindAttributeLocation(ATTR_IDX0_POS,    "pos");
        // bindAttributeLocation(ATTR_IDX1_UV,     "uv");
        // bindAttributeLocation(ATTR_IDX2_NORMAL, "normal");

        // This random string also works ???!!!
        // bindAttributeLocation(ATTR_IDX0_POS,    "afsdfasd");
        // bindAttributeLocation(ATTR_IDX1_UV,     "afsd");

        bindAttributeLocation(ATTR_IDX0_POS,    "vertexPosition");
        bindAttributeLocation(ATTR_IDX1_UV,     "vertexUV");
}

void StaticShader::getAllUniformLocations() {
    printf("    override for getting uniform-loc called.\n");
    
    // mvp_loc = getUniformLocation("mvp");
    // alpha_loc = getUniformLocation("blendAlpha");
    // // alpha_loc = getUniformLocation("texSampler");

    // transform_loc = getUniformLocation("transformMatrix");
    setTransformMatLoc(getUniformLocation("transformMatrix"));

    view_loc = getUniformLocation("viewMatrix");
    proj_loc = getUniformLocation("projMatrix");

    // printf("    unif_loc for mvp loc: %d, %d, %d\n\n", transform_loc, view_loc, proj_loc);
    printf("    unif_loc for mvp loc: %d, %d, %d\n\n", getTransformMatLoc(), view_loc, proj_loc);
}
