#include "Core/Renderer.h"             // BasicRenderer corresponds to StaticShader
#include "Core/Shader/StaticShader.h"  //

class BasicRenderer : public HighLevelRenderer {
public:
    BasicRenderer() {
        printf("  __ basic-renderer constructor called.\n");
        cleanUp();
        allocEntityShader();
    }
    ~BasicRenderer() {
        printf("  __ basic-renderer destructor called.\n");
        cleanUp();
    }

    void allocEntityShader() override;  // only have to be called once before the rendering loop
    void cleanUp() override;

    bool getEntityShaderStatus();

    void prepare() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        entityShader->start();
            entityShader->loadViewMatrix( getViewMatrix() );
    }
    
    void process(Entity *entity) {

            StaticShader *shader = dynamic_cast<StaticShader *>(entityShader);
                entityRendererWraper(entity);
    }

private:
    void entityRendererWraper(Entity *entity) {
        entityRenderer.render(entity, entityShader);
    }

    EntityRenderer entityRenderer;
    bool entityShaderLinked = false;
};
