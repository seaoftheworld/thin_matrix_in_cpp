// #pragma once

#include "Core/Shader/Base/BaseShader.h"

#define GUI_VSH_PATH "data/shaders/Gui/Gui.vsh"
#define GUI_FSH_PATH "data/shaders/Gui/Gui.fsh"

class GuiType00Shader : public BaseShader {

public:
    // enum attrNum {
    //     id0_pos2f = 0, max_attrNum
    // };

    // static const unsigned int attr_idx[max_attrNum];
    // static const unsigned int attr_stride[max_attrNum];
    // static const unsigned int attr_offset[max_attrNum];
    // static const unsigned int all_in_one_stride;

    GuiType00Shader() : BaseShader(GUI_VSH_PATH, GUI_FSH_PATH) {
        
        printf("  subclass constructor called.\n");
        call_subclass_init_funcs();

        // enable specific settings
        // specificSettingsOn();
    }
    ~GuiType00Shader(){
    }

    void bindAllAttributeLocations() override;
    void getAllUniformLocations() override;
};
