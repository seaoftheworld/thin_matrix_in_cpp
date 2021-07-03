#pragma once

#include "Core/Shader/Base/BaseShader.h"
#include "Core/Common/light.h"

// #define MULTI_LIGHTS_VSH_PATH "data/shaders/models/multi_lights_clip_plane.vsh"
// #define MULTI_LIGHTS_FSH_PATH "data/shaders/models/multi_lights_clip_plane.fsh"
#define MULTI_LIGHTS_VSH_PATH "data/shaders/models/multi_lights.vsh"
#define MULTI_LIGHTS_FSH_PATH "data/shaders/models/multi_lights.fsh"

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
        // specificSettingsOn();
    }
    ~MultiLightsShader(){
    }

    void bindAllAttributeLocations() override;
    void getAllUniformLocations() override;

    // void loadTransformMatrix(const float *p) {
    //     uniformMatrix4fv(transform_loc, p);
    // }

    // void loadViewMatrix(const float *p) {
    //     uniformMatrix4fv(view_loc, p);
    // }

    // void loadProjMatrix(const float *p) {
    //     uniformMatrix4fv(proj_loc, p);
    // }

    void loadReflectivity(float input) {
        uniform1f(objReflect_loc, input);
    }

    void loadShineDamper(float input) {
        uniform1f(objShineDamper_loc, input);
    }

    void loadClipPlane(float *p4f) {
        uniform4fv(clipPlane_loc, 1, p4f);
    }

    // void loadAlpha(float p) {
    //     uniform1f(alpha_loc, p);
    // }

    void loadLights(const std::vector<Light *> &lights) {

        unsigned int num_lights_input = lights.size();
        unsigned int num_lights_valid = (num_lights_input < SHADER_MAX_LIGHTS) ? (num_lights_input) : SHADER_MAX_LIGHTS;

        for (unsigned int i = 0; i < SHADER_MAX_LIGHTS; i++) {

            // Crash: can't dereference value initialized vector:
            // i may >= lights.size()
            // Light *light = (lights.begin() + i);

            if (i < num_lights_valid) {

                Light *light = *(lights.begin() + i);
                if (light) {
                    uniform3fv(lightPosition_loc[i], 1, light->getPosition3fv());
                    uniform3fv(lightColor_loc[i], 1, light->getColor3fv());
                    uniform3fv(lightAttenuation_loc[i], 1, light->getAttenuation3fv());
                    // debug info
                    // printf("%d: load light pos/color, %f, %f, %f, %f, %f, %f.\n", i, 
                    //     input_light_pos[i][0], input_light_pos[i][1], input_light_pos[i][2],
                    //     input_light_color[i][0], input_light_color[i][1], input_light_color[i][2]);
                }
            }
            else {
                float default_pos[] = { 0.0f, 0.0f, 1.0f };
                float default_color[] = { 0.0f, 0.0f, 0.0f };
                float default_attenuation[] = { 1.0f, 0.0f, 0.0f };

                uniform3fv(lightPosition_loc[i], 1, default_pos);
                uniform3fv(lightColor_loc[i], 1, default_color);
                uniform3fv(lightAttenuation_loc[i], 1, default_attenuation);

                // debug info
                // printf("%d: load default light pos/color, %f, %f, %f, %f, %f, %f.\n", i, 
                //     default_pos[0], default_pos[1], default_pos[2],
                //     default_color[0], default_color[1], default_color[2]);
            }
        }
    }

    void loadLights(Light *input_lights, unsigned int input_num_lights) {
        unsigned int num_lights = (input_num_lights < SHADER_MAX_LIGHTS) ? (input_num_lights) : SHADER_MAX_LIGHTS;

        for (unsigned int i = 0; i < SHADER_MAX_LIGHTS; i++) {
            
            Light *light = input_lights + i;
            if (!light) {
                continue;
            }

            if (i < num_lights) {
                uniform3fv(lightPosition_loc[i], 1, light->getPosition3fv());
                uniform3fv(lightColor_loc[i], 1, light->getColor3fv());
                uniform3fv(lightAttenuation_loc[i], 1, light->getAttenuation3fv());

                // debug info
                // printf("%d: load light pos/color, %f, %f, %f, %f, %f, %f.\n", i, 
                //     input_light_pos[i][0], input_light_pos[i][1], input_light_pos[i][2],
                //     input_light_color[i][0], input_light_color[i][1], input_light_color[i][2]);
            }
            else {
                float default_pos[] = { 0.0f, 0.0f, 1.0f };
                float default_color[] = { 0.0f, 0.0f, 0.0f };
                float default_attenuation[] = { 1.0f, 0.0f, 0.0f };

                uniform3fv(lightPosition_loc[i], 1, default_pos);
                uniform3fv(lightColor_loc[i], 1, default_color);
                uniform3fv(lightAttenuation_loc[i], 1, default_attenuation);

                // debug info
                // printf("%d: load default light pos/color, %f, %f, %f, %f, %f, %f.\n", i, 
                //     default_pos[0], default_pos[1], default_pos[2],
                //     default_color[0], default_color[1], default_color[2]);
            }
        }
        // printf("\n\n");
    }

private:
    int    lightPosition_loc[SHADER_MAX_LIGHTS]; 
    int       lightColor_loc[SHADER_MAX_LIGHTS];
    int lightAttenuation_loc[SHADER_MAX_LIGHTS];

    int objReflect_loc = -1,
        objShineDamper_loc = -1;

    int clipPlane_loc = -1;
};
