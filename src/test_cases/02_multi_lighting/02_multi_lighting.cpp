// To use OpenGL, Libs depended on are: 
// 1. glfw (included in WindowSystem.h)
// 2. gl3w (included in Renderer.h)
// 3. gl_math

#define __USE_INLINE_METHODS__
#include "WindowSystem/WindowSystem.h"

#include "test_cases/02_multi_lighting/MultiLightingRenderer.h"
#include "LightsPositionData.h"
#include "Core/Loader.h"

#include "Core/AssimpLib.h"

#include <iostream>
#include <stdio.h>

int test_02_multi_lighting() {

    printf("  __ 02 multi lighting __\n");

    WindowSystem &win = WindowSystem::instance();
    if (win.start() < 0) {
        win.stop();
        return -3;
    }

    std::cout << "  OpenGL Window/Context created (on MsWin), Init GL3W for modern OpenGL" << std::endl;
    if (!HighLevelRenderer::init()) {
        std::cout << "  Renderer::init()/gl3wInit() failed !" << std::endl;
        win.stop();
        return -2;
    }

    // ---------------------------------
    MultiLightsRenderer renderer;
    if (!renderer.getEntityShaderStatus()) {
        // loading/compiling/linking shader-program failed
        renderer.cleanUp();
        win.stop();
        return -1;
    }

    Loader loader;

    LightsPositionData lightsPositionData;
    Light lights[NUM_LIGHTS]; {
        float light_color[NUM_LIGHTS][Light::Color::max_color] = {
            { 1.8f, 1.0f, 1.8f },  // pink
                // float color[Light::max_color] = { 2.0f, 0.0f, 0.0f };         // red
                // float light_color0[Light::max_color] = { 0.0f, 2.0f, 0.0f };  // green
                // float light_pos0[Light::max_pos] = {0.0f, 1.0f, 2.0f};
                // float light_color0[Light::max_color] = { 0.0f, 0.0f, 0.0f };  // none
            { 1.0f, 2.6f, 1.0f },  // green
            { 1.8f, 1.8f, 1.0f },  // red + green ==> yellow
            { 0.6f, 0.6f, 2.2f }   // blue
        };

        float default_att[Light::max_att] = { 1.0f, 0.3f, 0.4f };

        for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
            lights[i].setValues(lightsPositionData.getPosition(i), &light_color[i], &default_att);
        }
    }

    // 4 crates represents the positions of 4 lights
    AssimpLib assimp_crate; {

        // Modify the 4 init-transforms of the crate-entity
        // in the following lines:
        float crate_transform_values[NUM_LIGHTS][Entity::transform::max];
        for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
            const float crate_scale = 0.12f;
            
            crate_transform_values[i][Entity::transform::x] = (*lightsPositionData.getPosition(i))[Light::Position::x];
            crate_transform_values[i][Entity::transform::y] = (*lightsPositionData.getPosition(i))[Light::Position::y];
            crate_transform_values[i][Entity::transform::z] = (*lightsPositionData.getPosition(i))[Light::Position::z];
            crate_transform_values[i][Entity::transform::rot_x] = 0.0f;
            crate_transform_values[i][Entity::transform::rot_y] = 0.0f;
            crate_transform_values[i][Entity::transform::rot_z] = 0.0f; 
            crate_transform_values[i][Entity::transform::scale] = crate_scale;
        }

        Transform crate_transforms[NUM_LIGHTS]; {
            for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
                crate_transforms[i].values[Transform::x] = crate_transform_values[i][Entity::transform::x];
                crate_transforms[i].values[Transform::y] = crate_transform_values[i][Entity::transform::y];
                crate_transforms[i].values[Transform::z] = crate_transform_values[i][Entity::transform::z];
                crate_transforms[i].values[Transform::rot_x] = crate_transform_values[i][Entity::transform::rot_x];
                crate_transforms[i].values[Transform::rot_y] = crate_transform_values[i][Entity::transform::rot_y];
                crate_transforms[i].values[Transform::rot_z] = crate_transform_values[i][Entity::transform::rot_z];
                crate_transforms[i].values[Transform::scale] = crate_transform_values[i][Entity::transform::scale];
            }
        }

        Transform *p_crate_transforms[NUM_LIGHTS]; {
            for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
                p_crate_transforms[i] = &crate_transforms[i];
            }
        };

        assimp_crate.loadModel( "data/models/crate/Crate1.obj", loader, p_crate_transforms, NUM_LIGHTS );

        // Doesnt work before shader is bound/opened/started .... !!!
        // multiLightsShader.loadLights(&light_pos[0], &light_color[0], 4);

        // printf("4 lights inited and crate model loaded, press anything to continue ...\n\n"); {
        //     int dbg;
        //     scanf("%d", &dbg);
        // }
    }

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

    Entity single_vbo_entity;
    Entity multi_vbo_entity; {

        std::string texture_files[2] = {
            "data/tex/font.png",
            "data/tex/road.png"
        };

        StaticTexture *textures[2] = { NULL };
        loader.loadStaticTextures(texture_files, 2, textures);
        if (!textures[0] || !textures[1]) {
            renderer.cleanUp();
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
                renderer.cleanUp();
                loader.cleanUp();

                win.stop();
                return -1;
            }

            single_vbo_entity.setTextureModel(textures[1], m); {
                float trans_big_square_values[Entity::transform::max] = {
                    // (float)i * 1.0f + 0.5f, (float)i * 1.0f + 0.1f, (float)i * 1.0f + 0.1f,
                    // 0.0f, 0.0f, 0.0f,
                    // (float)i * 0.3f + 0.3f

                    -2.0f, -2.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    4.0f
                };
                Transform trans_big_square(&trans_big_square_values);
                single_vbo_entity.addTransform(trans_big_square);

                float trans_small_square_values[Entity::transform::max] = {
                    -1.5f, -1.5f, 2.0f,
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

            float *test_vertex_attr[] = {
                test_pos, 
                test_uv
            };

            unsigned int test_vertices_count_from_pos = ARRAY_SIZE(test_pos) / MultiLightsShader::attr_stride[MultiLightsShader::id0_pos3f];
            unsigned int test_vertices_count_from_uv = ARRAY_SIZE(test_uv) / MultiLightsShader::attr_stride[MultiLightsShader::id1_uv2f];

            unsigned short test_indices[] = { 0, 1, 2, 0, 3, 2 };

            printf("  vertices counts from pos/uv are %d, %d\n", test_vertices_count_from_pos, test_vertices_count_from_uv);
            printf("  indices counts are %d\n", (int)ARRAY_SIZE(test_indices));
            
            StaticModel *model = loader.loadStaticModel(
                &test_vertex_attr, test_vertices_count_from_pos, 
                test_indices, ARRAY_SIZE(test_indices));

            if (!model) {
                renderer.cleanUp();
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
            renderer.addEntity( &(*misa_entity) );
        }
        for (auto crate_entity = assimp_crate.entities.begin(); crate_entity != assimp_crate.entities.end(); crate_entity++) {
            renderer.addEntity( &(*crate_entity) );
        }

        renderer.addEntity(&single_vbo_entity);
        renderer.addEntity(&multi_vbo_entity);

        num_all_entities = renderer.getEntitiesSize();
        printf("  __num of meshes for misa/all: %d, %d\n", num_misa_entities, num_all_entities);
    }

    // Camera cam(gl_math::vec3(-1.93f, 2.4f, 3.9f), 0.0f, -3.14f / 4.0f);
    // Camera cam(gl_math::vec3(-1.93f, 2.4f, 3.9f), 2.68f, -0.7f);
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

    while ( win.isValid() ) {

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
            
            win.pollEvents();  // not respond when close win with mouse without this
            
            cam.input_update(win);
            renderer.calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());

            // entity.increasePosition(0.0f, 0.0f, 0.002f);
            // entity.increaseRotation(0.0005f, 0.0f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0005f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0f, 0.005f);

            // Update the 4 lights according to positions of the 4 crates
            {
                // The 1st and the only entity in 'assimp_crate' is the crate model with 4 transforms
                Entity *crate = &(assimp_crate.entities[0]);

                // float rot_z_step = 0.032f;
                // float xy_step = 0.048f;

                // enum Move {
                //     rotate = 0, 
                //     crate0_get_closer, crate0_get_further, 
                //     crate3_get_closer, crate3_get_further, 
                //     crate2_get_closer, crate2_get_further, 
                //     crate1_get_closer, crate1_get_further, 
                //     max
                // };
                // static Move move = rotate;
                // static float curr_rot_z = 0.0f;

                // 1. update crate's 4 transforms
                if (!stop) {
                    // MultiLighsData::MoveType move_type_before = lightsPositionData.getMoveType();
                    lightsPositionData.update();
                    // MultiLighsData::MoveType move_type_after = lightsPositionData.getMoveType();
                    // if (move_type_before != move_type_after) {
                    //     stop = true;
                    // }

                    unsigned int offsets[] = {
                        Entity::transform::x, Entity::transform::y,
                        Entity::transform::z, Entity::transform::rot_z
                    };

                    XYZRotz xyz_rotz;

                    // Update each transform for the crate entiry
                    for (unsigned int i = 0; i < NUM_LIGHTS; i++) {

                        lightsPositionData.getPosRotz(i, &xyz_rotz);

                        // Revize the xyz info according to rot-z, the xyz from UpdatePosRotz
                        // is not rotated, rotation info is only in rotz value
                        gl_math::get_xyz_from_rotz(&xyz_rotz.pos, xyz_rotz.rot, &xyz_rotz.pos);

                        crate->setTransformValues(i, xyz_rotz.pos, offsets, ARRAY_SIZE(offsets));
                    }
                }

                // 2. update 4 lights' positions according to crate's 4 transforms
                {
                    float updated_light_pos[NUM_LIGHTS][Light::Position::max_pos];
                    for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
                        for (unsigned int j = 0; j < 3; j++) {
                            updated_light_pos[i][j] = (*(crate->getTransformValues(i)))[Entity::transform::x + j];
                        }
                    }
                    
                    for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
                        lights[i].setPosition(&updated_light_pos[i]);
                    }
                }

                // 3. write the 4 lights' pos, color, and attenuation into variables to be used
                //    by the multi-lights shader
                // TODO: use something like 'addLight()' dynamically 
                // instead of manually adding lights every time before rendering. {}
            }

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
            renderer.process(lights, ARRAY_SIZE(lights), num_misa_entities, num_all_entities);

            last_render_time = now;
            rendered_times++;
        }

        win.swapBuffers();
        fps++;
    }

    for (auto itr = assimp_crate.entities.begin(); itr != assimp_crate.entities.end(); itr++) {
        itr->cleanUp();
    }
    for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
        itr->cleanUp();
    }
    assimp_crate.cleanUp();
    assimp_misa.cleanUp();

    single_vbo_entity.cleanUp();
    multi_vbo_entity.cleanUp();

    renderer.cleanUp();
    loader.cleanUp();
    
    win.stop();
    return 0;
}

// printf("shader loaded, press anything to continue ...\n\n"); {
//     int dbg;
//     scanf("%d", &dbg);
// }
