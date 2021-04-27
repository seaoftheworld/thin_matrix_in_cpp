// #include "Core/Renderer.h"

// #include "Core/Shader/SpecularShader.h"  // Terrain shader is now copied from specular shader
// #include "Core/Shader/TerrainShader.h"
// #include "Core/Shader/SkyboxShader.h"
// #include "Core/Gui/GuiRenderer.h"

#include "Core/Renderers/Model/ModelRenderer.h"
#include "Core/Renderers/Terrain/TerrainRenderer.h"
#include "Core/Renderers/Skybox/SkyboxRenderer.h"
#include "Core/Renderers/Gui/GuiRenderer.h"


#define PROFILING (0)
#include "Core/profile.h"

class WrappingRenderer {  // need a base-wrapping-renderer ???
public:
    EntityRenderer   entityRenderer;
    TerrainRenderer terrainRenderer;
    SkyboxRenderer   skyboxRenderer;
    GuiRenderer         guiRenderer;
    // WaterRenderer water;

    void specificSettingsOff();
    void specificSettingsOn();

    void process(Light &light) {
        prepare();
            // entityRenderer.run(light);
            entityRenderer.run();
            terrainRenderer.run(light);
            // skyboxRenderer.run();
            // guiRenderer.run();
    }

private:
    void prepare() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};

class VersatileRenderer : public HighLevelRenderer {

public:
    // GuiRenderer guiRenderer;

public:
    VersatileRenderer() {
        printf("  __ VersatileRenderer-renderer constructor called.\n");
        // cleanUp();  // guiRenderer will be deleted and invalidated here
        allocShaders();
    }
    ~VersatileRenderer() {
        printf("  __ VersatileRenderer-renderer destructor called.\n");
        cleanUp();
    }

    void allocShaders() override;  // only have to be called once before the rendering loop
    void cleanUp() override;

    bool getEntityAndTerrainShaderStatus();

    // void specificSettingsOff();
    // void specificSettingsOn();
    
    void process(Light &light, unsigned int misa_entities_num, unsigned int total_entities_num) {
        PROFILE_FUNCTION();
        
        prepare();
        {
            PROFILE_SCOPE("entity");
            // entityShader->start();
            // entityShader->loadViewMatrix( getViewMatrix() );
            
                // entityShader->loadLight(light);
                
                // entityShader->loadReflectivity(20.0f);
                // entityShader->loadShineDamper(20.0f);
                unsigned int misa_start_idx = 0;
                // entityRendererWraper(misa_start_idx, misa_entities_num);

                // entityShader->loadReflectivity(10.0f);
                // entityShader->loadShineDamper(10.0f);
                unsigned int the_rest_entities_start_idx = misa_entities_num;
                // entityRendererWraper(the_rest_entities_start_idx, total_entities_num - misa_entities_num);
        }

        {
            // PROFILE_SCOPE("terrain");
            // terrainShader->start();
            // terrainShader->loadViewMatrix( getViewMatrix() );

            //     terrainShader->loadLight(light);

            //     terrainShader->loadReflectivity(20.0f);
            //     terrainShader->loadShineDamper(20.0f);
            //     terrainRendererWraper();
        }

        {
            PROFILE_SCOPE("skybox");
            // if (sky) {
                // skyboxShader->start();
                // skyboxShader->loadSkyboxViewMatrix( getViewMatrix() );
                //     skyboxRenderer.render(sky);
            // }
        }

        {
            PROFILE_SCOPE("gui overlay");
            // guiRenderer.render();
        }
    }

    // void setSkybox(Skybox *input_sky) {
    //     sky = input_sky;
    // }

    // void addGui(GuiTextureType00 *gui) {
    //     guiRenderer.addGui(gui);
    // }

private:
    void prepare() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // void entityRendererWraper(unsigned int start_entity_idx, unsigned int num) {

    //     if (start_entity_idx < getEntitiesSize()) {
    //         for (unsigned int i = start_entity_idx; i < getEntitiesSize() && (i - start_entity_idx) < num; i++) {
    //             auto entity = getEntities().begin() + i;
    //             entityRenderer.render(*entity, entityShader);
    //         }
    //     }
    // }

    // void terrainRendererWraper(unsigned int start_entity_idx, unsigned int num) {
    // void terrainRendererWraper() {
    //     // if (start_entity_idx < getEntitiesSize()) {
    //         // for (unsigned int i = start_entity_idx; i < terrains.size() && (i - start_entity_idx) < num; i++) {
    //         for (unsigned int i = 0; i < terrains.size(); i++) {
    //             auto terrain = terrains.begin() + i;
    //             terrainRenderer.render(*terrain, terrainShader);
    //         }
    //     // }
    // }

    // TODO: only use 1 pionter to indicate shader status
    // TerrainShader *terrainShader = NULL;
    // SpecularShader *entityShader = NULL;
    // SkyboxShader *skyboxShader = NULL;
    
    // TerrainRenderer terrainRenderer;
    // EntityRenderer entityRenderer;
    // SkyboxRenderer skyboxRenderer;

    // GuiRenderer guiRenderer;

    // Skybox *sky = NULL;
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
