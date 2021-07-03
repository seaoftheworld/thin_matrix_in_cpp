#include "WindowSystem/WindowSystem.h"
#include "Core/Common/gl_header.h"

#define REFLECTION_WIDTH  (WIN_WIDTH / 2)
#define REFLECTION_HEIGHT (WIN_HEIGHT / 2)

#define REFRACTION_WIDTH  (WIN_WIDTH / 4)
#define REFRACTION_HEIGHT (WIN_HEIGHT / 4)

class WaterFrameBuffers {

private:
    unsigned int reflectionFBO = -1,
                 reflectionTexture     = -1,
                 reflectionDepthBuffer = -1;

    unsigned int refractionFBO = -1,
                 refractionTexture      = -1,
                 refractionDepthTexture = -1;

    void bindFrameBuffer(unsigned int fb, unsigned int width, unsigned int height) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        glViewport(0, 0, width, height);
    }

    unsigned int addColorBufferAttachment_Texture(unsigned int width, unsigned int height) {
        unsigned int texture = 0;
        glGenTextures(1, (GLuint *)&texture);
        glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

        return texture;
    }

    unsigned int addDepthBufferAttachment_NonTexture() {
        unsigned int depthBuffer = 0;
        glGenRenderbuffers(1, (GLuint *)&depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, REFLECTION_WIDTH, REFLECTION_HEIGHT);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        return depthBuffer;
    }

    unsigned int addDepthBufferAttachment_Texture() {
        unsigned int texture = 0;
        glGenTextures(1, (GLuint *)&texture);
        glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, REFRACTION_WIDTH, REFRACTION_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

        return texture;
    }

    void initReflectionFBO() {
        glGenFramebuffers(1, (GLuint *)&reflectionFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
            glDrawBuffer(GL_COLOR_ATTACHMENT0); //indicates render to color attachment 0 when bound

            reflectionTexture = addColorBufferAttachment_Texture(REFLECTION_WIDTH, REFLECTION_HEIGHT);
            reflectionDepthBuffer = addDepthBufferAttachment_NonTexture();

        unbindCurrentFBO();
    }

    void initRefractionFBO() {
        glGenFramebuffers(1, (GLuint *)&refractionFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, refractionFBO);
            glDrawBuffer(GL_COLOR_ATTACHMENT0); //indicates render to color attachment 0 when bound

            refractionTexture = addColorBufferAttachment_Texture(REFRACTION_WIDTH, REFRACTION_HEIGHT);
            refractionDepthTexture = addDepthBufferAttachment_Texture();

        unbindCurrentFBO();
    }

public:
    unsigned int getReflectionTexture() {
        return reflectionTexture;
    }

    // unsigned int getReflectionDepthBuffer() {
    //     return reflectionDepthBuffer
    // }

    unsigned int getRefractionTexture() {
        return refractionTexture;
    }

    unsigned int getRefractionDepthTexture() {
        return refractionDepthTexture;
    }

public:
    WaterFrameBuffers() {
        initReflectionFBO();
        initRefractionFBO();
    }

    void cleanUp() {
        glDeleteFramebuffers(1, &reflectionFBO);
        glDeleteTextures(1, &reflectionTexture);
        glDeleteRenderbuffers(1, &reflectionDepthBuffer);

        glDeleteFramebuffers(1, &refractionFBO);
        glDeleteTextures(1, &refractionTexture);
        glDeleteTextures(1, &refractionDepthTexture);
    }

public:
    void bindReflectionFBO() {
        bindFrameBuffer(reflectionFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT);
    }

    void bindRefractionFBO() {
        bindFrameBuffer(refractionFBO, REFRACTION_WIDTH, REFRACTION_HEIGHT);
    }

    void unbindCurrentFBO() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    }
};
