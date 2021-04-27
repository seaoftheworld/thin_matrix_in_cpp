#include "Core/Common/data.h"
// #include "Loader.h"

#define REFLECTION_WIDTH  (320)
#define REFLECTION_HEIGHT (180)

#define REFRACTION_WIDTH  (1280)
#define REFRACTION_HEIGHT (720)

class WaterFrameBuffers {

private:
    unsigned int reflectionFrameBuffer = -1,
                 reflectionTexture     = -1,
                 reflectionDepthBuffer = -1;

    unsigned int refractionFramebuffer  = -1,
                 refractionTexture      = -1,
                 refractionDepthTexture = -1;

    void allocFrameBufferObj(unsigned int width, unsigned int height, unsigned int *fb, unsigned int *texture, unsigned int depth_buff_texture) {

    }

    void bindFrameBuffer(unsigned int fb, unsigned int width, unsigned int height) {

    }

public:
    WaterFrameBuffers() {
        allocFrameBufferObj();  // assign values for members
        allocFrameBufferObj();
    }

    void cleanUp() {
        glDeleteFramebuffers(reflectionFrameBuffer);
        glDeleteTextures(reflectionTexture);
        glDeleteRenderbuffers(reflectionDepthBuffer);

        glDeleteFramebuffers(refractionFramebuffer);
        glDeleteTextures(refractionTexture);
        glDeleteTextures(refractionDepthTexture);
    }

    void bindReflectionFrameBufferObj() {
        bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
    }

    void bindRefractionFrameBufferObj() {
        bindFrameBuffer(refractionFramebuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
    }

    void unbindFrameBufferObj() {
    }
};
