#pragma once
#include "./Base/BaseShader.h"
#include "Core/Common/light.h"

#define SPECULAR_VSH_PATH "data/shaders/specular.vsh"
#define SPECULAR_FSH_PATH "data/shaders/specular.fsh"

class SpecularShader : public BaseShader {

public:
    enum attrNum {
        id0_pos3f = 0, id1_uv2f, id2_normal3f, max_attrNum
    };

    static const unsigned int attr_idx[max_attrNum];
    static const unsigned int attr_stride[max_attrNum];
    static const unsigned int attr_offset[max_attrNum];
    static const unsigned int all_in_one_stride;

    SpecularShader() : BaseShader(SPECULAR_VSH_PATH, SPECULAR_FSH_PATH) {
        
        printf("  subclass constructor called.\n");
        call_subclass_init_funcs();

        // enable specific settings
        specificSettingsOn();
    }
    ~SpecularShader(){
    }

    void bindAllAttributeLocations() override;
    void getAllUniformLocations() override;

    void loadLight(Light &light) {
        uniform3fv(lightPosition_loc, 1, light.getPosition3fv());
        uniform3fv(lightColor_loc, 1, light.getColor3fv());
    }

    void loadReflectivity(float input) {
        uniform1f(objReflect_loc, input);
    }

    void loadShineDamper(float input) {
        uniform1f(objShineDamper_loc, input);
    }

    // void loadAlpha(float p) {
    //     uniform1f(alpha_loc, p);
    // }

private:
    int lightPosition_loc = -1, 
        lightColor_loc = -1,

        objReflect_loc = -1,
        objShineDamper_loc = -1;
    
    void specificSettingsOn() {
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(0.7f, 0.7f, 0.8f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_MULTISAMPLE); // Usually enabled by default in driver

        // glEnable(GL_CULL_FACE);  // requried the model to be enclosed, otherwise
        // glCullFace(GL_BACK);     // will not be correctly displayed ???
        // glCullFace(GL_FRONT);
    }

    void specificSettingsOff() {
    }
};
