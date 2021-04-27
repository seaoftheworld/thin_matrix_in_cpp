#include "Core/Common/data.h"

// typedef float(*pA2f)[2];
// typedef pA2f fpA2f();

#define GUI_RECT_VERTICES_NUM    (4)
#define GUI_RECT_VERTICES_STRIDE (2)

class GuiType00 {
private:
    unsigned int textureID;
    float position[2];
    float scale[2];

public:
    // The model for a gui (and all guis)
    static RawModel *rect;

public:
    void init(unsigned int input_tex_id, float input_pos[][2], float input_scale[][2]) {
        textureID = input_tex_id;

        if (input_pos) {
            position[0] = (*input_pos)[0];
            position[1] = (*input_pos)[1];
        }

        if (input_scale) {
            scale[0] = (*input_scale)[0];
            scale[1] = (*input_scale)[1];
        }
    }

    unsigned int getTextureID() {
        return textureID;
    }

    float (*getPosition())[2] {
        return &position;
    }

    float (*getScale())[2] {
        return &scale;
    }

    // pA2f sadf() {
    //     return &scale;
    // }
};
