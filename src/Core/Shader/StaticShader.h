#pragma once
#include "./Base/BaseShader.h"

#define VSH_PATH "data/shaders/image_mod.vsh"
#define FSH_PATH "data/shaders/image_mod.fsh"

class StaticShader : public BaseShader {

public:

    // For all-in-one (Struct-of-arrays) vbo
    enum attrNum {
        id0_pos3f = 0, id1_uv2f, id2_normal3f, max_attrNum
    };

    static const unsigned int attr_idx[max_attrNum];
    static const unsigned int attr_stride[max_attrNum];
    static const unsigned int attr_offset[max_attrNum];
    static const unsigned int all_in_one_stride;

    // StaticShader() : BaseShader(vshPath, fshPath) {
    StaticShader() : BaseShader(VSH_PATH, FSH_PATH) {
        printf("  subclass constructor called.\n");
        call_subclass_init_funcs();

        // if (getProgId() > 0) {
        //     bindAllAttributeLocations();
        //     linkProg();
        // }
        // if (getProgId() > 0) {
        //     getAllUniformLocations();
        // }

        // enable specific settings
        specificSettingsOn();
    }
    ~StaticShader(){
    }

    void bindAllAttributeLocations() override;
    void getAllUniformLocations() override;

    // void loadTransformMatrix(const float *p) {
    //     uniformMatrix4fv(transform_loc, p);
    // }

    void loadViewMatrix(const float *p) {
        uniformMatrix4fv(view_loc, p);
    }

    void loadProjMatrix(const float *p) {
        uniformMatrix4fv(proj_loc, p);
    }

    // void loadAlpha(float p) {
    //     uniform1f(alpha_loc, p);
    // }

private:
    // int transform_loc = -1, // alpha_loc = -1;
    int view_loc = -1, 
        proj_loc = -1;
    
    void specificSettingsOn() {
        glClearColor(0.7f, 0.7f, 0.8f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);

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
