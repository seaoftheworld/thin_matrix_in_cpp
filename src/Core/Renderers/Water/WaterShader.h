#include "Core/Shader/Base/BaseShader.h"

// #define WATER_VSH_PATH "data/shaders/Water/Water.vsh"
// #define WATER_FSH_PATH "data/shaders/Water/Water.fsh"
#define WATER_VSH_PATH "data/shaders/Water/pureColor.vsh"
#define WATER_FSH_PATH "data/shaders/Water/pureColor.fsh"

class WaterShader : public BaseShader {

    int reflectionTexture_loc = -1,
        refractionTexture_loc = -1,
        dudvTexture_loc = -1,
        normalTexture_loc = -1,
        moveSpeed_loc = -1,
        lightPosition_loc = -1;

public:
    WaterShader() : BaseShader(WATER_VSH_PATH, WATER_FSH_PATH) {
        printf("\n\n  subclass(Water-shader) constructor called.\n");
        call_subclass_init_funcs();
    }
    ~WaterShader(){
    }

public:
    void bindAllAttributeLocations() override;
    void getAllUniformLocations() override;

    void bindTextureUnits() {                 // set corresponding texture-units for each texture, when using this shader, 
        uniform1i(reflectionTexture_loc, 0);  // activate GL_TEXTURE0 unit before bind to reflectionTexture's id
        uniform1i(refractionTexture_loc, 1);  // and activate GL_TEXTURE1 unit before bind to refractionTexture's id
        uniform1i(dudvTexture_loc, 2);
        uniform1i(normalTexture_loc, 3);
    }

    void loadMoveSpeed(float input) {
        uniform1f(moveSpeed_loc, input);
    }

    void setDefaultLightPosition() {
        float dummy_pos[3] = {10.0f, 10.0f, 100.0f};
        uniform3fv(lightPosition_loc, 1, dummy_pos);
    }
};
