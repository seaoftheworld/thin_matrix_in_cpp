// To use OpenGL, Libs depended on are: 
// 1. glfw (included in WindowSystem.h)
// 2. gl3w (included in Renderer.h)
// 3. gl_math

#define __USE_INLINE_METHODS__
#include "WindowSystem/WindowSystem.h"

#include "EntityTerrainRenderer.h"
#include "Core/Loader.h"

#include "Core/AssimpLib.h"

#include <iostream>
#include <stdio.h>

#define PROFILING (1)
#include "Core/profile.h"

int test_03_terrain() {

    printf("  __ 03 terrain __\n");

    WindowSystem &win = WindowSystem::instance();
    if (win.start() < 0) {
        win.stop();
        return -3;
    }

    // std::cout << "  OpenGL Window/Context created (on MsWin), Init GL3W for modern OpenGL" << std::endl;
    // if (!HighLevelRenderer::init()) {
    //     std::cout << "  Renderer::init()/gl3wInit() failed !" << std::endl;
    //     win.stop();
    //     return -2;
    // }

    if (!BaseRenderer::init()) {
        std::cout << "  BaseRenderer::init()/gl3wInit() failed !" << std::endl;
        win.stop();
        return -2;
    }

    // ---------------------------------
    // VersatileRenderer renderer;
    // Loader loader;

    WrappingRenderer renderers; {
        if (!renderers.entityRenderer.ready()) {
            // renderer.cleanUp();

            // renderers.terrainRenderer.freeShadersData();
            win.stop();
            return -1;
        }

        if (!renderers.terrainRenderer.ready()) {
            // renderer.cleanUp();

            // renderers.terrainRenderer.freeShadersData();
            win.stop();
            return -1;
        }

        if (!renderers.skyboxRenderer.ready()) {
            // renderer.cleanUp();

            // renderers.terrainRenderer.freeShadersData();
            // renderers.skyboxRenderer.freeShadersData();
            win.stop();
            return -1;
        }

        renderers.guiRenderer.setLoader(&loader);
        if (!renderers.guiRenderer.ready()) {
            // renderer.cleanUp();
            win.stop();
            return -1;
        }
    }
    renderers.specificSettingsOn();

    // Instrumentor::Get().BeginSession("init part profiling");

    Terrain terrain; {

        // InstrumentationTimer timer("init terrain data");
        TerrainTexturePack texturePack; {

            TerrainTexture terrainTexture00; {
                std::string terrainTexturePath = "data/tex/terrain/grassy2.png";
                StaticTexture *terrainTexture = NULL;

                loader.loadStaticTextures(&terrainTexturePath, 1, &terrainTexture);
                terrainTexture00.init(terrainTexture->getId());
            }

            TerrainTexture terrainTexture01; {
                std::string terrainTexturePath = "data/tex/terrain/mud.png";
                StaticTexture* terrainTexture = NULL;

                loader.loadStaticTextures(&terrainTexturePath, 1, &terrainTexture);
                terrainTexture01.init(terrainTexture->getId());
            }

            TerrainTexture terrainTexture02; {
                std::string terrainTexturePath = "data/tex/terrain/grassFlowers.png";
                StaticTexture* terrainTexture = NULL;

                loader.loadStaticTextures(&terrainTexturePath, 1, &terrainTexture);
                terrainTexture02.init(terrainTexture->getId());
            }

            TerrainTexture terrainTexture03; {
                std::string terrainTexturePath = "data/tex/terrain/path.png";
                StaticTexture* terrainTexture = NULL;

                loader.loadStaticTextures(&terrainTexturePath, 1, &terrainTexture);
                terrainTexture03.init(terrainTexture->getId());
            }

            texturePack.init(terrainTexture00, terrainTexture01, terrainTexture02, terrainTexture03);
        }

        TerrainTexture blendmap; {
            std::string terrainTexturePath = "data/tex/terrain/blendmap.png";
            StaticTexture *terrainTexture = NULL;

            loader.loadStaticTextures(&terrainTexturePath, 1, &terrainTexture);
            blendmap.init(terrainTexture->getId());
        }

        terrain.init(&loader, 0, 0, texturePack, blendmap, "data/tex/terrain/height_map/heightmap.png");

        printf("grass tex id: %d.\n", terrain.getTexturePack()->getTerrainTexture00()->getTextureID());
        printf("  mud tex id: %d.\n", terrain.getTexturePack()->getTerrainTexture01()->getTextureID());
        printf("flowertex id: %d.\n", terrain.getTexturePack()->getTerrainTexture02()->getTextureID());
        printf(" path tex id: %d.\n", terrain.getTexturePack()->getTerrainTexture03()->getTextureID());
        printf("blend tex id: %d.\n", terrain.getBlendMap()->getTextureID());

        // renderer.addTerrain(&terrain);
        renderers.terrainRenderer.addTerrain(&terrain);

        printf("terrain init done, press anything to continue ...\n\n"); {
            int dbg;
            scanf("%d", &dbg);

            if (dbg == 0) {
                // renderer.cleanUp();
                loader.cleanUp();
                win.stop();
                return 0;
            }
        }
    }

    {
        // InstrumentationTimer timer("init skybox data");

        std::string cloudySky[] = {
            "data/tex/sky/right.png", 
            "data/tex/sky/left.png", 
            "data/tex/sky/back.png", 
            "data/tex/sky/front.png",
            "data/tex/sky/top.png", 
            "data/tex/sky/bottom.png"
        };
        Skybox sky(&loader, &cloudySky, 50);
        // renderer.setSkybox(&sky);
        renderers.skyboxRenderer.setSkybox(&sky);

        printf("skybox init done, press anything to continue ...\n\n"); {
            int dbg;
            scanf("%d", &dbg);
        }
    }

    GuiType00 gui; {

        // InstrumentationTimer timer("init gui data");
        
        StaticTexture *guiTexture = NULL; {
            std::string guiTexturePath = "data/tex/marker.png";
            loader.loadStaticTextures(&guiTexturePath, 1, &guiTexture);
        }

        if (guiTexture) {
            float gui_pos[] = {-0.5f, 0.5f};
            float gui_scale[] = { 0.5f, 0.5f };

            gui.init(guiTexture->getId(), &gui_pos, &gui_scale);
            
            // printf("gui tex id: %d", gui.getTextureID());
            // printf("gui addr: 0x%p\n", &gui);

            renderers.guiRenderer.addGui(&gui);
            // printf("%s gui-texture added\n\n", guiTexturePath.c_str());
        }
    }

    // Instrumentor::Get().EndSession();

    AssimpLib assimp_misa; {

        float values[Entity::transform::max] = {
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    1.0f
        };
        Transform transform(&values);
        Transform *transforms = &transform;

        // TODO: improve meshes' hierachy
        assimp_misa.loadModel("data/models/misa/misa_yForward_zUp.obj", loader, &transforms, 1);
    }

    AssimpLib assimp_terrain_bottom; {

        float values[Entity::transform::max] = {
                    10.0f, 10.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    10.0f
        };
        Transform transform(&values);
        Transform *transforms = &transform;

        // TODO: improve meshes' hierachy
        assimp_terrain_bottom.loadModel("data/models/terrain_bottom/rocks.obj", loader, &transforms, 1);
    }

    Entity single_vbo_entity;
    Entity multi_vbo_entity; {

        std::string texture_files[2] = {
            "data/tex/font.png",
            "data/tex/road.png"
        };

        StaticTexture *textures[2] = { NULL };
        loader.loadStaticTextures(texture_files, 2, textures);
        if (!textures[0] || !textures[1]) {
            // renderer.cleanUp();
            loader.cleanUp();

            win.stop();
            return -1;
        }

        printf("  textures generated, id:\n");
        printf("    ");
        for (unsigned int i = 0; i < 2; i++) {
            printf("%d, ", textures[i]->getId());
        }
        printf("\n\n");

        // Init display for the big square and small square
        // single_vbo display
        {
            float single_vbo_data[] = { 
                -0.5f, 0.5f,  -0.0f, 
                 0.0f, 0.0f, 
                 0.0f, 0.0f, 0.0f, 

                 0.5f, 0.5f,  -0.0f, 
                 1.0f, 0.0f, 
                 0.0f, 0.0f, 0.0f, 
   
                 0.5f, -0.5f, -0.0f, 
                 1.0f, 1.0f, 
                 0.0f, 0.0f, 0.0f, 

                -0.5f, -0.5f, -0.0f,
                 0.0f, 1.0f,
                 0.0f, 0.0f, 0.0f
            };

            unsigned short indices[] = { 0, 1, 2, 0, 3, 2 };

            unsigned int vertices_count = ARRAY_SIZE(single_vbo_data) / StaticModel_SingleVbo::single_vbo_stride_in_float;

            printf("  single-vbo: vertices count is: %d\n", vertices_count);
            printf("               indices count is: %d\n", (int)ARRAY_SIZE(indices));

            StaticModel_SingleVbo *m = loader.loadSingleVboModel(single_vbo_data, vertices_count, indices, ARRAY_SIZE(indices));
            if (!m) {
                // renderer.cleanUp();
                loader.cleanUp();

                win.stop();
                return -1;
            }

            single_vbo_entity.setTextureModel(textures[1], m); {
                float trans_big_square_values[Entity::transform::max] = {
                    // (float)i * 1.0f + 0.5f, (float)i * 1.0f + 0.1f, (float)i * 1.0f + 0.1f,
                    // 0.0f, 0.0f, 0.0f,
                    // (float)i * 0.3f + 0.3f

                    -5.0f, -5.0f, 1.0f,
                    0.0f, 0.0f, 0.0f,
                    4.0f
                };
                Transform trans_big_square(&trans_big_square_values);
                single_vbo_entity.addTransform(trans_big_square);

                float trans_small_square_values[Entity::transform::max] = {
                    // -1.5f, -1.5f, 2.0f,
                    -5.0f, -5.0f, 2.0f,
                    0.0f, 0.0f, 0.0f,
                    1.0f
                };
                Transform trans_small_square(&trans_small_square_values);
                single_vbo_entity.addTransform(trans_small_square);
            }
        }

        // Init display for the many transparent squares
        // multi_vbo display
        {
            //*
            float test_pos[] = { -0.5f, 0.5f,  0.0f, 
                                  0.5f, 0.5f,  0.0f, 
                                  0.5f, -0.5f, 0.0f, 
                                 -0.5f, -0.5f, 0.0f };

            float test_uv[] = { 0.0f, 0.0f, 
                                1.0f, 0.0f, 
                                1.0f, 1.0f, 
                                0.0f, 1.0f };

            float dummy_normal[] = {0.0f, 0.0f, 0.0f};

            float *test_vertex_attr[] = {
                test_pos, 
                test_uv,
                dummy_normal
            };

            unsigned int test_vertices_count_from_pos = ARRAY_SIZE(test_pos) / StaticShader::attr_stride[StaticShader::id0_pos3f];
            unsigned int test_vertices_count_from_uv = ARRAY_SIZE(test_uv) / StaticShader::attr_stride[StaticShader::id1_uv2f];

            unsigned short test_indices[] = { 0, 1, 2, 0, 3, 2 };

            printf("  vertices counts from pos/uv are %d, %d\n", test_vertices_count_from_pos, test_vertices_count_from_uv);
            printf("  indices counts are %d\n", (int)ARRAY_SIZE(test_indices));
            
            StaticModel *model = loader.loadStaticModel(
                &test_vertex_attr, test_vertices_count_from_pos, 
                test_indices, ARRAY_SIZE(test_indices));

            if (!model) {
                // renderer.cleanUp();
                loader.cleanUp();

                win.stop();
                return -1;
            }

            multi_vbo_entity.setTextureModel(textures[0], model);

            for (unsigned int i = 0; i < 10; i++) {
                float trans_values[Entity::transform::max] = {
                    (float)i * 1.0f + 0.5f, (float)i * 1.0f + 0.1f, (float)i * 1.0f + 0.1f,
                    0.0f, 0.0f, 0.0f,
                    (float)i * 0.3f + 0.3f
                };

                Transform transform(&trans_values);
                multi_vbo_entity.addTransform(transform);
            }
            //*/
        }

        // printf("big, small, transparent squares entities loaded, press anything to continue ...\n\n"); {
        //     int dbg;
        //     scanf("%d", &dbg);
        // }
    }

    unsigned int num_misa_entities = assimp_misa.entities.size(), num_all_entities = 0; {
        // This way doesn't work !!!
        //
        // for (auto misa_entity : assimp_misa.entities) {
        //     hlRenderer.addEntity(&misa_entity);
        // }

        for (auto misa_entity = assimp_misa.entities.begin(); misa_entity != assimp_misa.entities.end(); misa_entity++) {
            // renderer.addEntity( &(*misa_entity) );
            // renderers.entityRenderer.addEntity( &(*misa_entity) );
        }

        for (auto rock_entity = assimp_terrain_bottom.entities.begin(); rock_entity != assimp_terrain_bottom.entities.end(); rock_entity++) {
            // renderer.addEntity( &(*rock_entity) );
            // renderers.entityRenderer.addEntity( &(*rock_entity) );
        }

        // renderer.addEntity(&single_vbo_entity);
        // renderer.addEntity(&multi_vbo_entity);

        renderers.entityRenderer.addEntity(&single_vbo_entity);
        // renderers.entityRenderer.addEntity(&multi_vbo_entity);

        // num_all_entities = renderer.getEntitiesSize();
        num_all_entities = renderers.entityRenderer.getEntitiesSize();
        printf("  __num of meshes for misa/all: %d, %d\n", num_misa_entities, num_all_entities);
    }

    // {
    //     std::string terrainTexturePath = "data/tex/grass_summer.png";
    //     StaticTexture *terrainTexture = NULL;
    //     loader.loadStaticTextures(&terrainTexturePath, 1, &terrainTexture);

    //     Terrain terrain00(&loader, terrainTexture, -1, -1); 
    //     // Terrain terrain01(&loader, terrainTexture, 0, 1); 
    //     {
    //         renderer.addTerrain(&terrain00);
    //         // renderer.addTerrain(&terrain01);
    //     }
    // }

    Light light; {
        // float position[Light::Position::max_pos] = {0.0f, 1.0f, 2.0f};
        float position[Light::Position::max_pos] = {5.0f, 5.0f, 10.0f};

        // float color[Light::Color::max_color] = {1.6f, 1.2f, 1.6f};
        float color[Light::Color::max_color] = { 1.6f, 1.6f, 1.6f };
        float dummy_attenuation[Light::Attenuation::max_att] = {0.0f, 0.0f, 0.0f};
        light.setValues(&position, &color, &dummy_attenuation);
    }

    Camera cam(gl_math::vec3(-1.36f, 3.15f, 4.25f), 2.65f, -0.765f);

    // double prevFrameTime = 0;
    double now = 0, 
           last_wake_up_time = 0, 
           last_update_time = 0, 
           last_render_time = 0,
           last_1s_time = 0;

    unsigned int rendered_times = 0,  // cleared every 1-sec
                 updated_times = 0,   // recording how many times sleep/wakeup/update/render
                 wakeup_times = 0,    // codes are executed in 1-sec
                 sleep_times = 0;
    unsigned int fps = 0;

    double update_cycle = (float)(1.0f / (55)),
           render_cycle = (float)(1.0f / (55)),
           min_cycle = (update_cycle < render_cycle) ? (update_cycle) : (render_cycle);

    Instrumentor::Get().BeginSession("main loop profiling");
    while ( win.isValid() ) {

        PROFILE_SCOPE("main loop");

        static bool first = true;
        if (first) {
            win.setCursorPos(WIN_WIDTH / 2, WIN_HEIGHT / 2);
            now = win.getTime();
            
            last_wake_up_time = now; // assigned value after min_cycle-time has passed
            last_update_time = now;  // assigned value after update-codes executed
            last_render_time = now;  // assigned value after render-codes executed
            last_1s_time = now;
            
            first = false;
        }
        else {
            // // elapasedTime = elapasedTime + prevFrameTime;  // += crashes
            // prevFrameTime = now - last_wake_up_time;
            now = win.getTime();
        }

        // 1-second-update: Print dbg info
        static bool stop = false;
        if (now - last_1s_time > 1.0f) {
            {
                // printf("  __ 1s: sleep/wakeup: %d--%d, update/render: %d--%d \n\n", 
                //     sleep_times, wakeup_times, updated_times, rendered_times);

                // if (updated_times < update_freq) {
                // }
                // if (rendered_times < render_freq) {
                // }

                rendered_times = 0;
                updated_times = 0;
                wakeup_times = 0;
                sleep_times = 0;

                last_1s_time = now;
            }

            {
                // unsigned short et_idx = 0;
                // float rot_z_step = 0.785f;

                // static bool increase = false;
                
                // for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
                //     float delta_rot_z = ( increase ) ? (rot_z_step) : (-rot_z_step);
                //     (*itr).increaseRotation(et_idx, 0.0f, 0.0f, delta_rot_z);
                // }

                // if ((*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z] > 3.14f) {
                //     // printf("rot_z max: %f\n", (*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
                //     increase = false;
                // }
                // else if ((*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z] < -3.14f) {
                //     // printf("rot_z min: %f\n", (*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
                //     increase = true;
                // }
            }

            {
                printf("  __ 1s: %d fps\n", fps);
                fps = 0;
            }

            if (stop) {
                static unsigned char i = 0;
                i++;
                if (i > 0) {
                    stop = false;
                    i = 0;
                }
            }
        }

        // When less than the amount of 'min_cycle' time has passed since the last valid loop,
        // sleep for 'min_cycle' time before the next loop for power-consumption.
        //
        // if (now - last_wake_up_time < min_cycle) {
        //     // sleep for 'min_cycle' time using the OS's api
        //     sleep_times++;
        //     continue;
        // }
        // else {
        //     wakeup_times++;
        //     last_wake_up_time = now;
        // }

        // Updating rountine
        // update view-matrix according to input, 
        // update entity pos, rot, scale...  
        // and the others...
        // if ( now - last_update_time >= update_cycle ) {
        if (1) {

            PROFILE_SCOPE("Update inputs, view...");
            
            win.pollEvents();  // not respond when close win with mouse without this
            
            cam.input_update(win);
            // renderer.calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());
            BaseRenderer::calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());
            // hlRenderer.terrainRenderer.calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());

            // entity.increasePosition(0.0f, 0.0f, 0.002f);
            // entity.increaseRotation(0.0005f, 0.0f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0005f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0f, 0.005f);

            /*
            // Updating misa model
            {
                unsigned short et_idx = 0;
                float rot_z_step = 0.016f;

                static bool increase = false;

                // if (!stop) {
                    float delta_rot_z = ( increase ) ? (rot_z_step) : (-rot_z_step);
                    
                    for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
                    // auto itr = assimp_misa.entities.begin(); {
                        itr->increaseRotation(et_idx, 0.0f, 0.0f, delta_rot_z);
                        // itr->setRotZ(0, 3.14f);
                    }

                    if ((*assimp_misa.entities[0].getTransformValues(et_idx))[Entity::transform::rot_z] > 3.14f) {
                        printf("rot_z max: %f\n", (*assimp_misa.entities[0].getTransformValues(et_idx))[Entity::transform::rot_z]);
                        increase = false;
                        stop = true;
                    }
                    else if ((*assimp_misa.entities[0].getTransformValues(et_idx))[Entity::transform::rot_z] < -3.14f) {
                        // printf("rot_z min: %f\n", (*assimp_misa.entities[0].getTransformValues(et_idx))[Entity::transform::rot_z]);
                        // increase = true;
                        // stop = true;
                    }
                // }
            }
            //*/

            last_update_time = now;
            updated_times++;
        }

        // Render entities
        // if ( now - last_render_time >= render_cycle ) {
        {
            PROFILE_SCOPE("Render");

            // renderer.process(light, num_misa_entities, num_all_entities);
            renderers.process(light);

            last_render_time = now;
            rendered_times++;
        }

        {
            PROFILE_SCOPE("Swap");
            win.swapBuffers();
        }

        {
            PROFILE_SCOPE("misc");
            fps++;
        }
    }

TOTAL_CLEANUP:
    for (auto misa_entity = assimp_misa.entities.begin(); misa_entity != assimp_misa.entities.end(); misa_entity++) {
        misa_entity->cleanUp();
    }
    assimp_misa.cleanUp();

    for (auto rock_entity = assimp_terrain_bottom.entities.begin(); rock_entity != assimp_terrain_bottom.entities.end(); rock_entity++) {
        rock_entity->cleanUp();
    }
    assimp_terrain_bottom.cleanUp();

    single_vbo_entity.cleanUp();
    multi_vbo_entity.cleanUp();

PARTIAL_CLEANUP:
    // renderer.cleanUp();

    // renderers.terrainRenderer.freeShadersData();
    loader.cleanUp();
    
    win.stop();

    Instrumentor::Get().EndSession();
    return 0;
}

// printf("shader loaded, press anything to continue ...\n\n"); {
//     int dbg;
//     scanf("%d", &dbg);
// }
