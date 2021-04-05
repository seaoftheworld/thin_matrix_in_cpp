#include "Core/Renderer.h"
#include "Core/Shader/SpecularShader.h"

class SpecularRenderer : public HighLevelRenderer {
public:
    SpecularRenderer() {
        printf("  __ specular-renderer constructor called.\n");
        cleanUp();
        allocShaders();
    }
    ~SpecularRenderer() {
        printf("  __ specular-renderer destructor called.\n");
        cleanUp();
    }

    void allocShaders() override;  // only have to be called once before the rendering loop
    void cleanUp() override;

    bool getEntityShaderStatus();

    void specificSettingsOff();
    void specificSettingsOn();
    
    void process(Light &light, unsigned int misa_entities_num, unsigned int total_entities_num) {
        prepare();
            entityShader->start();
            entityShader->loadViewMatrix( getViewMatrix() );

            SpecularShader *shader = entityShader;
                shader->loadLight(light);
                shader->loadReflectivity(20.0f);
                shader->loadShineDamper(20.0f);

                unsigned int misa_start_idx = 0;
                entityRendererWraper(misa_start_idx, misa_entities_num);

                shader->loadReflectivity(10.0f);
                shader->loadShineDamper(10.0f);
                unsigned int the_rest_entities_start_idx = misa_entities_num;
                entityRendererWraper(the_rest_entities_start_idx, total_entities_num - misa_entities_num);
    }

private:
    void prepare() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void entityRendererWraper(unsigned int start_entity_idx, unsigned int num) {

        if (start_entity_idx < getEntitiesSize()) {
            for (unsigned int i = start_entity_idx; i < getEntitiesSize() && (i - start_entity_idx) < num; i++) {
                auto entity = getEntities().begin() + i;
                entityRenderer.render(*entity, entityShader);
            }
        }
    }

    SpecularShader *entityShader = NULL;
    bool entityShaderLinked = false;

    EntityRenderer entityRenderer;
};

// // void process(Light &light) {
// void process() {

//     // set some uniforms (like light) common to all entities for shaders
//     // setShadersUniforms(light);

//     // Process all the Entities
//     for (auto entity : entities) {
//         entityRenderer.process(entity, entityShader);
//     }

//     // Process something else ...
//     trnRenderer.process();
// }
