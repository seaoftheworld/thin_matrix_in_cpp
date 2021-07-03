#pragma once
#include "Core/Shader/Base/BaseShader.h"
#include "Core/Common/light.h"

#define TERRAIN_VSH_PATH "data/shaders/terrain/terrain_multi_lights_clip_plane.vsh"
#define TERRAIN_FSH_PATH "data/shaders/terrain/terrain_multi_lights_clip_plane.fsh"
#define TERRAIN_SHADER_MAX_LIGHTS (8)

class TerrainShader : public BaseShader {

public:
    enum attrNum {
        id0_pos3f = 0, id1_uv2f, id2_normal3f, max_attrNum
    };

    static const unsigned int attr_idx[max_attrNum];
    static const unsigned int attr_stride[max_attrNum];
    static const unsigned int attr_offset[max_attrNum];
    static const unsigned int all_in_one_stride;

    TerrainShader() : BaseShader(TERRAIN_VSH_PATH, TERRAIN_FSH_PATH) {
        printf("  subclass constructor called.\n");
        call_subclass_init_funcs();
    }
    ~TerrainShader(){
    }

    void bindAllAttributeLocations() override;
    void getAllUniformLocations() override;

    void bindTextureUnits() {
        // uniform1i(texture00_sampler_loc, GL_TEXTURE0);
        // uniform1i(texture01_sampler_loc, GL_TEXTURE1);
        // uniform1i(texture02_sampler_loc, GL_TEXTURE2);
        // uniform1i(texture03_sampler_loc, GL_TEXTURE3);
        // uniform1i(blendMap_sampler_loc, GL_TEXTURE4);

        uniform1i(texture00_sampler_loc, 0);
        uniform1i(texture01_sampler_loc, 1);
        uniform1i(texture02_sampler_loc, 2);
        uniform1i(texture03_sampler_loc, 3);
        uniform1i(blendMap_sampler_loc, 4);
    }

    // void loadLight(Light &light) {
    //     uniform3fv(lightPosition_loc, 1, light.getPosition3fv());
    //     uniform3fv(lightColor_loc, 1, light.getColor3fv());
    // }
    void loadLights(const std::vector<Light *> &lights) {

        unsigned int num_lights_input = lights.size();
        unsigned int num_lights_valid = (num_lights_input < TERRAIN_SHADER_MAX_LIGHTS) ? (num_lights_input) : TERRAIN_SHADER_MAX_LIGHTS;

        for (unsigned int i = 0; i < TERRAIN_SHADER_MAX_LIGHTS; i++) {

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

    void loadReflectivity(float input) {
        uniform1f(objReflect_loc, input);
    }

    void loadShineDamper(float input) {
        uniform1f(objShineDamper_loc, input);
    }

    // void loadAlpha(float p) {
    //     uniform1f(alpha_loc, p);
    // }

    void loadClipPlane(float pa4f[][4]) {
        uniform4fv(clipPlane_loc, 1, &(*pa4f)[0]);
    }

private:
    // int lightPosition_loc = -1, 
    //     lightColor_loc = -1,
    int    lightPosition_loc[TERRAIN_SHADER_MAX_LIGHTS];
    int       lightColor_loc[TERRAIN_SHADER_MAX_LIGHTS];
    int lightAttenuation_loc[TERRAIN_SHADER_MAX_LIGHTS];

    int objReflect_loc = -1,
        objShineDamper_loc = -1;

    int texture00_sampler_loc = -1, 
        texture01_sampler_loc = -1, 
        texture02_sampler_loc = -1, 
        texture03_sampler_loc = -1, 
        blendMap_sampler_loc = -1;

    int clipPlane_loc = -1;
};
