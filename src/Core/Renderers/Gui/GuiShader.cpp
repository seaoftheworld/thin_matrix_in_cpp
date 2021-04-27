#include "GuiShader.h"

#define ATTR_IDX0_POS      (0)
#define ATTR_IDX0_POS_STRD (2)

    // static inline unsigned int calculateAllInOneStride() {
    //     unsigned int sum = 0;
    //     for (unsigned int i = 0; i < GuiType00Shader::max_attrNum; i++) {
    //         sum += GuiType00Shader::attr_stride[i];
    //     }

    //     return sum;
    // }

    // const unsigned int GuiType00Shader::attr_idx[max_attrNum] = {
    //     ATTR_IDX0_POS,
    //     // ATTR_IDX1_UV, 
    //     // ATTR_IDX2_NORMAL
    // };

    // const unsigned int GuiType00Shader::attr_stride[max_attrNum] = {
    //     ATTR_IDX0_POS_STRD,
    //     // ATTR_IDX1_UV_STRD, 
    //     // ATTR_IDX2_NORMAL_STRD
    // };

    // const unsigned int GuiType00Shader::attr_offset[max_attrNum] = {
    //     0, 
    //     // attr_stride[0],
    //     // attr_stride[0] + attr_stride[1]
    // };

    // const unsigned int GuiType00Shader::all_in_one_stride = calculateAllInOneStride();


// This is optional, attr idx could be set explicitly within shader
// in latter versions of gl
void GuiType00Shader::bindAllAttributeLocations() {
    printf("    override for setting attr-idx called.\n");

    // This binding takes effect when the program is linked the next time—it
    // does not change the bindings used by the currently linked program.
    bindAttributeLocation(ATTR_IDX0_POS,    "vertexPosition");
}

void GuiType00Shader::getAllUniformLocations() {
    printf("    override for getting uniform-loc called.\n");

    setTransformMatLoc(getUniformLocation("transformMatrix"));
    printf("    unif_loc for transform-Matrix loc: %d \n\n", getTransformMatLoc());
}
