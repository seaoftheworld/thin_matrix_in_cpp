#include "Core/Renderer.h"
#include "SkyboxShader.h"

#include "Core/Loader.h"

// SkyboxShader-class doesn't have a valid loadTransformMatrix() method now, because 
// the skybox-shader itself only have view/proj-matrixes, no transform matrix
#define NUM_SKYBOX_VERTICES (6 * (3 + 3))

class Skybox {

    StaticTexture *cubeTexture = NULL;
    RawModel *cube = NULL;  // rename tobe: rawCube ???

public:
    void init(Loader *loader, std::string imgPaths[][6], unsigned int cube_edge_length);
    
    RawModel *getCubeModel() {
        return cube;
    }

    StaticTexture *getCubeTexture() {
        return cubeTexture;
    }
};

class SkyboxRenderer : public BaseRenderer {
private:
    SkyboxShader *skyboxShader = NULL;
    Skybox *skybox = NULL;

public:
    void allocShadersData() override;
    void freeShadersData() override;
    bool ready() override;

    void setSkybox(Skybox *sb) {
        skybox = sb;
    }

public:
    SkyboxRenderer() {
        printf("SkyboxRenderer() constructor called.\n\n\n");
        freeShadersData();
        allocShadersData();
    }
    virtual ~SkyboxRenderer() {
        printf("SkyboxRenderer() destructor called.\n\n\n");
        freeShadersData();
    }

    void run() {
        if (!skybox || !skyboxShader) {
            return;
        }

        skyboxShader->start();
        skyboxShader->loadSkyboxViewMatrix( getViewMatrix() );
            render();
    }

    void render() {
        // Setting this to be 0 could make drawing unable to crash the program,
        // when NULL-texture/model pointers are input by mistake.
        unsigned int vertices_count = 0;

        if (skybox->getCubeTexture()) {
            // bind a texture buffer object
            int texture_id = skybox->getCubeTexture()->getId();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
        }

        if (skybox->getCubeModel()) {
            vertices_count = skybox->getCubeModel()->getVertexCount();

            glBindBuffer(GL_ARRAY_BUFFER, skybox->getCubeModel()->getVboID());
            
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(
                0, 
                3, GL_FLOAT, 
                GL_FALSE, 
                3 << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                0);
        }

        {
            glDrawArrays(GL_TRIANGLES, 0, vertices_count);
            // glDrawArrays(GL_TRIANGLES, 0, 3 * 3);
        }
    }
};
