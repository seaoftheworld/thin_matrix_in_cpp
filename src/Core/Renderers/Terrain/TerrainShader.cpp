#include "TerrainShader.h"

#define ATTR_IDX0_POS    (0)
#define ATTR_IDX1_UV     (1)
#define ATTR_IDX2_NORMAL (2)

#define ATTR_IDX0_POS_STRD    (3)
#define ATTR_IDX1_UV_STRD     (2)
#define ATTR_IDX2_NORMAL_STRD (3)

static inline unsigned int calculateAllInOneStride() {
    unsigned int sum = 0;
    for (unsigned int i = 0; i < TerrainShader::max_attrNum; i++) {
        sum += TerrainShader::attr_stride[i];
    }

    return sum;
}

const unsigned int TerrainShader::attr_idx[max_attrNum] = {
    ATTR_IDX0_POS,
    ATTR_IDX1_UV, 
    ATTR_IDX2_NORMAL
};

const unsigned int TerrainShader::attr_stride[max_attrNum] = {
    ATTR_IDX0_POS_STRD,
    ATTR_IDX1_UV_STRD, 
    ATTR_IDX2_NORMAL_STRD
};

const unsigned int TerrainShader::attr_offset[max_attrNum] = {
    0, 
    attr_stride[0],
    attr_stride[0] + attr_stride[1]
};

const unsigned int TerrainShader::all_in_one_stride = calculateAllInOneStride();


// This is optional, attr idx could be set explicitly within shader
// in latter versions of gl
void TerrainShader::bindAllAttributeLocations() {
    printf("    override for setting attr-idx called.\n");

    // This binding takes effect when the program is linked the next time—it
    // does not change the bindings used by the currently linked program.
    bindAttributeLocation(ATTR_IDX0_POS,    "vertexPosition");
    bindAttributeLocation(ATTR_IDX1_UV,     "vertexUV");
    bindAttributeLocation(ATTR_IDX2_NORMAL, "vertexNormal");
}

void TerrainShader::getAllUniformLocations() {
    printf("    override for getting uniform-loc called.\n");

    setTransformMatLoc(getUniformLocation("transformMatrix"));
    setViewMatLoc(getUniformLocation("viewMatrix"));
    setProjMatLoc(getUniformLocation("projMatrix"));
    printf("    unif_loc for mvp loc: %d, %d, %d\n\n", 
        getTransformMatLoc(), getViewMatLoc(), getProjMatLoc());

    // lightPosition_loc = getUniformLocation("lightPosition");
    // lightColor_loc = getUniformLocation("lightColor");
    // printf("    unif_loc for light_pos/color: %d, %d\n\n", lightPosition_loc, lightColor_loc);
    {
        std::string prefix0 = "lightPosition[";
        std::string prefix1 = "lightColor[";
        std::string prefix2 = "lightAttenuation[";
        
        for (int i = 0; i < TERRAIN_SHADER_MAX_LIGHTS; i++) {

            std::string result = prefix0 + std::to_string(i) + "]";
            // std::string result = prefix0 + std::to_string(i);
            // printf("light pos: %s\n", result.c_str());
            lightPosition_loc[i] = getUniformLocation(result.c_str());

            result = prefix1 + std::to_string(i) + "]";
            // result = prefix1 + std::to_string(i);
            // printf("light color: %s\n", result.c_str());
            lightColor_loc[i] = getUniformLocation(result.c_str());

            result = prefix2 + std::to_string(i) + "]";
            // printf("light att: %s\n", result.c_str());
            lightAttenuation_loc[i] = getUniformLocation(result.c_str());

            printf("    unif_loc-%d for light_pos:         %d\n", i,    lightPosition_loc[i]);
            printf("    unif_loc-%d for light_color:       %d\n", i,       lightColor_loc[i]);
            printf("    unif_loc-%d for light_attenuation: %d\n\n", i, lightAttenuation_loc[i]);
        }
        printf("\n\n");
    }

    objShineDamper_loc = getUniformLocation("shineDamper");
    objReflect_loc = getUniformLocation("reflectivity");
    printf("    unif_loc for reflec/shineDamper: %d, %d\n\n", objReflect_loc, objShineDamper_loc);

    // Get uniform locations for 5 samplers
    texture00_sampler_loc = getUniformLocation("texture00");
    texture01_sampler_loc = getUniformLocation("texture01");
    texture02_sampler_loc = getUniformLocation("texture02");
    texture03_sampler_loc = getUniformLocation("texture03");
    blendMap_sampler_loc = getUniformLocation("blendMap");
    printf("    terrain unif_loc for 5 textures: %d, %d, %d, %d, %d\n\n", 
        texture00_sampler_loc, texture01_sampler_loc, texture02_sampler_loc, 
        texture03_sampler_loc, blendMap_sampler_loc);

    clipPlane_loc = getUniformLocation("terrainClipPlane");
    printf("    terrain clipPlane_loc : %d\n\n\n\n", clipPlane_loc);
}
