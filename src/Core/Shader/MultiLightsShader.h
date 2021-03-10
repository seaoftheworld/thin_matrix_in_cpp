#pragma once
#include "./Base/BaseShader.h"

#define MULTI_LIGHTS_VSH_PATH "data/shaders/multi_lights.vsh"
#define MULTI_LIGHTS_FSH_PATH "data/shaders/multi_lights.fsh"

#define SHADER_MAX_LIGHTS (8)

class MultiLightsShader : public BaseShader {

public:
    enum attrNum {
        id0_pos3f = 0, id1_uv2f, id2_normal3f, max_attrNum
    };

    static const unsigned int attr_idx[max_attrNum];
    static const unsigned int attr_stride[max_attrNum];
    static const unsigned int attr_offset[max_attrNum];
    static const unsigned int all_in_one_stride;

    MultiLightsShader() : BaseShader(MULTI_LIGHTS_VSH_PATH, MULTI_LIGHTS_FSH_PATH) {
        
        printf("  subclass constructor called.\n");
        call_subclass_init_funcs();

        // enable specific settings
        specificSettingsOn();
    }
    ~MultiLightsShader(){
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

    void loadReflectivity(float input) {
        uniform1f(objReflect_loc, input);
    }

    void loadShineDamper(float input) {
        uniform1f(objShineDamper_loc, input);
    }

    // void loadAlpha(float p) {
    //     uniform1f(alpha_loc, p);
    // }

        void loadMultiLights(const float *input_light_pos[], const float *input_light_color[], unsigned int input_num_lights) {
            
            unsigned int num_lights = (input_num_lights < SHADER_MAX_LIGHTS) ? (input_num_lights) : SHADER_MAX_LIGHTS;

            for (unsigned int i = 0; i < SHADER_MAX_LIGHTS; i++) {
                if (!input_light_pos[i] || !input_light_color[i]) {
                    continue;
                }

                if (i < num_lights) {
                    uniform3fv(lightPosition_loc[i], 1, input_light_pos[i]);
                    uniform3fv(lightColor_loc[i], 1, input_light_color[i]);

                    // debug info
                    // printf("%d: load light pos/color, %f, %f, %f, %f, %f, %f.\n", i, 
                    //     input_light_pos[i][0], input_light_pos[i][1], input_light_pos[i][2],
                    //     input_light_color[i][0], input_light_color[i][1], input_light_color[i][2]);
                }
                else {
                    float default_pos[] = {0.0f, 0.0f, 0.0f};
                    float default_color[] = {0.0f, 0.0f, 0.0f};

                    uniform3fv(lightPosition_loc[i], 1, default_pos);
                    uniform3fv(lightColor_loc[i], 1, default_color);

                    // debug info
                    // printf("%d: load default light pos/color, %f, %f, %f, %f, %f, %f.\n", i, 
                    //     default_pos[0], default_pos[1], default_pos[2],
                    //     default_color[0], default_color[1], default_color[2]);
                }
            }
            // printf("\n\n");
        }

        // void loadFixedLights() {

        //     float light_pos0[] = {-4.0f, 0.0f, 1.0f};
        //     float light_color0[] = { 2.0f, 0.0f, 0.0f };  // red

        //     float light_pos1[] = {-4.0f, -4.0f, 1.0f};
        //     float light_color1[] = { 2.0f, 2.0f, 0.0f };  // red + green ==> yellow

        //     float light_pos2[] = {0.0f, -4.0f, 1.0f};
        //     float light_color2[] = { 0.0f, 2.0f, 0.0f };  // green

        //     float light_pos3[] = {0.0f, 1.0f, 2.0f};
        //     float light_color3[] = { 1.6f, 1.2f, 1.6f };  // pink

        //     // float *input_light_pos[] = {
        //     //     &light_pos0[0],
        //     //     &light_pos1[0],
        //     //     &light_pos2[0],
        //     //     &light_pos3[0]
        //     // };

        //     // float *input_light_color[] = {
        //     //     &light_color0[0],
        //     //     &light_color1[0],
        //     //     &light_color2[0],
        //     //     &light_color3[0]
        //     // };

        //     // uniform3fv(lightPosition_loc[0], 1, input_light_pos[0]);
        //     // uniform3fv(lightColor_loc[0], 1, input_light_color[0]);

        //     // uniform3fv(lightPosition_loc[1], 1, input_light_pos[1]);
        //     // uniform3fv(lightColor_loc[1], 1, input_light_color[1]);

        //     // uniform3fv(lightPosition_loc[2], 1, input_light_pos[2]);
        //     // uniform3fv(lightColor_loc[2], 1, input_light_color[2]);

        //     // uniform3fv(lightPosition_loc[3], 1, input_light_pos[3]);
        //     // uniform3fv(lightColor_loc[3], 1, input_light_color[3]);

        //     uniform3fv(lightPosition_loc[0], 1, light_pos0);
        //     uniform3fv(lightColor_loc[0], 1, light_color0);

        //     uniform3fv(lightPosition_loc[1], 1, light_pos1);
        //     uniform3fv(lightColor_loc[1], 1, light_color1);
        // }

private:
    int view_loc = -1, 
        proj_loc = -1;

    int lightPosition_loc[SHADER_MAX_LIGHTS]; 
    int lightColor_loc[SHADER_MAX_LIGHTS];

    int objReflect_loc = -1,
        objShineDamper_loc = -1;
    
    void specificSettingsOn() {
        // glClearColor(0.7f, 0.7f, 0.8f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
