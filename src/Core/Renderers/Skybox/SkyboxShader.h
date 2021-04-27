#pragma once
#include "Core/Shader/Base/BaseShader.h"
// #include "Core/Common/light.h"

#define SKYBOX_VSH_PATH "data/shaders/skybox.vsh"
#define SKYBOX_FSH_PATH "data/shaders/skybox.fsh"

class SkyboxShader : public BaseShader {

public:
    enum attrNum {
        id0_pos3f = 0, max_attrNum
    };

    static const unsigned int attr_idx[max_attrNum];
    static const unsigned int attr_stride[max_attrNum];
    static const unsigned int attr_offset[max_attrNum];
    static const unsigned int all_in_one_stride;

    SkyboxShader() : BaseShader(SKYBOX_VSH_PATH, SKYBOX_FSH_PATH) {
        
        printf("  subclass constructor called.\n");
        call_subclass_init_funcs();
    }
    ~SkyboxShader(){
    }

    void bindAllAttributeLocations() override;
    void getAllUniformLocations() override;

    void loadSkyboxViewMatrix(float *org_view_matrix) {

        // skybox fixed
        // loadViewMatrix(org_view_matrix);

        // skybox non-fixed, always centers on camera
        float skyboxViewMat[4 * 4]; {
            skyboxViewMat[0 + 0 * 4] = org_view_matrix[0 + 0 * 4];
            skyboxViewMat[1 + 0 * 4] = org_view_matrix[1 + 0 * 4];
            skyboxViewMat[2 + 0 * 4] = org_view_matrix[2 + 0 * 4];
            skyboxViewMat[3 + 0 * 4] = org_view_matrix[3 + 0 * 4];

            skyboxViewMat[0 + 1 * 4] = org_view_matrix[0 + 1 * 4];
            skyboxViewMat[1 + 1 * 4] = org_view_matrix[1 + 1 * 4];
            skyboxViewMat[2 + 1 * 4] = org_view_matrix[2 + 1 * 4];
            skyboxViewMat[3 + 1 * 4] = org_view_matrix[3 + 1 * 4];

            skyboxViewMat[0 + 2 * 4] = org_view_matrix[0 + 2 * 4];
            skyboxViewMat[1 + 2 * 4] = org_view_matrix[1 + 2 * 4];
            skyboxViewMat[2 + 2 * 4] = org_view_matrix[2 + 2 * 4];
            skyboxViewMat[3 + 2 * 4] = org_view_matrix[3 + 2 * 4];

            // skyboxViewMat[0 + 3 * 4] = org_view_matrix[0 + 3 * 4];
            // skyboxViewMat[1 + 3 * 4] = org_view_matrix[1 + 3 * 4];
            // skyboxViewMat[2 + 3 * 4] = org_view_matrix[2 + 3 * 4];
            // skyboxViewMat[3 + 3 * 4] = org_view_matrix[3 + 3 * 4];
            skyboxViewMat[0 + 3 * 4] = 0.0f;
            skyboxViewMat[1 + 3 * 4] = 0.0f;
            skyboxViewMat[2 + 3 * 4] = 0.0f;
            skyboxViewMat[3 + 3 * 4] = org_view_matrix[3 + 3 * 4];
        }

        loadViewMatrix(skyboxViewMat);

        // printf("orignal view matrix\n");
        // for (int i = 0; i < 4; i++) {
        //     for (int j = 0; j < 4; j++) {
        //         printf("%4f, ", org_view_matrix[j + i * 4]);
        //     }
        //     printf("\n");
        // }
        // printf("\n\n");

        // printf("skybox view matrix:\n");
        // for (int i = 0; i < 4; i++) {
        //     for (int j = 0; j < 4; j++) {
        //         printf("%4f, ", skyboxViewMat[j + i * 4]);
        //     }
        //     printf("\n");
        // }
        // printf("\n\n");

    }

    // void loadLight(Light &light) {
    //     uniform3fv(lightPosition_loc, 1, light.getPosition3fv());
    //     uniform3fv(lightColor_loc, 1, light.getColor3fv());
    // }

    // void loadReflectivity(float input) {
    //     uniform1f(objReflect_loc, input);
    // }

    // void loadShineDamper(float input) {
    //     uniform1f(objShineDamper_loc, input);
    // }

    // void loadAlpha(float p) {
    //     uniform1f(alpha_loc, p);
    // }

private:
    // int lightPosition_loc = -1, 
    //     lightColor_loc = -1,

    //     objReflect_loc = -1,
    //     objShineDamper_loc = -1;
};
