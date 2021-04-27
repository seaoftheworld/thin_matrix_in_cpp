// #include "Common/Loaderdata.h"
#include "Core/Renderer.h"
#include "GuiShader.h"
#include "Gui.h"

#include "Core/Common/gl_math.h"

class GuiRenderer : public BaseRenderer {

private:
    GuiType00Shader *guiShader = NULL;

    // contains Transform and Scale info for each gui tobe displayed
    std::vector<GuiType00 *> guis;

public:
    void allocShadersData() override;
    void freeShadersData() override;
    bool ready() override;

    void addGui(GuiType00 *gui) {
        guis.push_back(gui);
    }

    void run() {
        guiShader->start();
        glDisable(GL_DEPTH_TEST);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            render();

        glEnable(GL_DEPTH_TEST);
    }
    
    void render() {

        unsigned int vertices_count = 0;
        if (GuiType00::rect) {
            glBindBuffer(GL_ARRAY_BUFFER, GuiType00::rect->getVboID());
            glEnableVertexAttribArray(0);            
            glVertexAttribPointer(
                0, 
                2, GL_FLOAT, 
                GL_FALSE, 
                2 << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                0);

            vertices_count = GuiType00::rect->getVertexCount();
        }

        for (unsigned int i = 0; i < guis.size(); i++) {

            auto gui = guis.begin() + i;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, (*gui)->getTextureID());

            gl_math::mat4 transform_matrix = gl_math::mat4(1.0f); {
                gl_math::create_transform_matrix((*gui)->getPosition(), (*gui)->getScale(), &transform_matrix);
                guiShader->loadTransformMatrix(&transform_matrix[0][0]);
            }

            glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices_count);
        }
    }

public:
    GuiRenderer() {
        printf("  __ gui-renderer constructor called.\n");
        freeShadersData();
        allocShadersData();
    }
    ~GuiRenderer() {
        printf("  __ gui-renderer destructor called.\n");
        freeShadersData();
    }
};
