// To use OpenGL, Libs depended on are: 
// 1. glfw (included in WindowSystem.h)
// 2. gl3w (included in Renderer.h)
// 3. gl_math

#define __USE_INLINE_METHODS__
#include "WindowSystem/WindowSystem.h"

#include "test_cases/01_specular_lighting/SpecularRenderer.h"
#include "Core/Loader.h"

#include "Core/AssimpLib.h"

#include <iostream>
#include <stdio.h>

int test_01_specular_lighting() {

    printf("  __ 01 specular lighting __\n");

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

    // -----------------------------
    SpecularRenderer renderer;
    if (!renderer.getEntityShaderStatus()) {
        // loading/compiling/linking shader-program failed
        renderer.cleanUp();
        win.stop();
        return -1;
    }
    renderer.specificSettingsOn();
    Loader loader;

    AssimpLib assimp_misa; {
        // float info[Entity::transform::max] = {
        //             0.0f, 0.0f, 0.0f,
        //             0.0f, 0.0f, 0.0f,
        //             1.0f
        // };
        // EntityInfo transform(&info);
        // assimp_misa.loadModel("data/models/rb73/rb73_yForward_zUp.obj", transform, loader);

        // float info[Entity::transform::max] = {
        //             0.0f, 0.0f, 0.0f,
        //             3.14f / 2.0f, 0.0f, 0.0f,
        //             4.0f
        // };
        // EntityInfo transform(&info);
        // assimp_misa.loadModel("data/models/gun/Handgun_obj.obj", transform, loader);

        float info[Entity::transform::max] = {
                    0.0f, 0.0f, 0.0f,
                    // 0.0f, 0.0f, 3.14f / 2.0f,
                    0.0f, 0.0f, 0.0f,
                    1.0f
        };
        Transform transform(&info);
        Transform *transforms[] = { &transform };

        assimp_misa.loadModel("data/models/misa/misa_yForward_zUp.obj", loader, transforms, 1);
    }

    Entity single_vbo_entity;
    Entity multi_vbo_entity; 
    {

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

            single_vbo_entity.setTextureModel(textures[1], m); 

            {
                float transform_values[Entity::transform::max] = {
                    -5.0f, -5.0f, 1.0f,
                    0.0f, 0.0f, 0.0f,
                    4.0f
                };
                Transform transform(&transform_values);

                single_vbo_entity.addTransform(transform);
            }

            {
                float transform_values[Entity::transform::max] = {
                    -5.0f, -5.0f, 2.0f,
                    0.0f, 0.0f, 0.0f,
                    1.0f
                };
                Transform transform(&transform_values);

                single_vbo_entity.addTransform(transform);
            }
        }

        {
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

            unsigned int test_vertices_count_from_pos = ARRAY_SIZE(test_pos) / SpecularShader::attr_stride[SpecularShader::id0_pos3f];
            unsigned int test_vertices_count_from_uv = ARRAY_SIZE(test_uv) / SpecularShader::attr_stride[SpecularShader::id1_uv2f];

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
                float values[Entity::transform::max] = {
                    (float)i * 1.0f + 0.5f, (float)i * 1.0f + 0.1f, (float)i * 1.0f + 0.1f,
                    0.0f, 0.0f, 0.0f,
                    (float)i * 0.3f + 0.3f
                };

                Transform transform(&values);
                multi_vbo_entity.addTransform(transform);
            }
        }
    }

    unsigned int num_misa_target = assimp_misa.entities.size(), num_all_targets = 0; {
        // This way doesn't work !!!
        //
        // for (auto misa_entity : assimp_misa.entities) {
        //     hlRenderer.addEntity(&misa_entity);
        // }

        for (auto misa_entity = assimp_misa.entities.begin(); misa_entity != assimp_misa.entities.end(); misa_entity++) {
            renderer.addEntity( &(*misa_entity) );
        }

        renderer.addEntity(&single_vbo_entity);
        renderer.addEntity(&multi_vbo_entity);

        num_all_targets = renderer.getEntitiesSize();
        printf("  __num of meshes for misa/all: %d, %d\n", num_misa_target, num_all_targets);
    }

    Light light; {
        float position[Light::Position::max_pos] = {0.0f, 1.0f, 2.0f};
        float color[Light::Color::max_color] = {1.6f, 1.2f, 1.6f};
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

    double update_cycle = (float)(1.0f / (50)),
           render_cycle = (float)(1.0f / (50)),
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

        // Print info to debug and reset debug infos every 1-second
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
                // unsigned short transform_idx = 0;
                // float rot_z_step = 0.785f;

                // static bool increase = false;
                
                // for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
                //     float delta_rot_z = ( increase ) ? (rot_z_step) : (-rot_z_step);
                //     (*itr).increaseRotation(transform_idx, 0.0f, 0.0f, delta_rot_z);
                // }

                // if ((*assimp_misa.entities[0].getInfo(transform_idx))[Entity::transform::rot_z] > 3.14f) {
                //     // printf("rot_z max: %f\n", (*assimp_misa.entities[0].getInfo(transform_idx))[Entity::transform::rot_z]);
                //     increase = false;
                // }
                // else if ((*assimp_misa.entities[0].getInfo(transform_idx))[Entity::transform::rot_z] < -3.14f) {
                //     // printf("rot_z min: %f\n", (*assimp_misa.entities[0].getInfo(transform_idx))[Entity::transform::rot_z]);
                //     increase = true;
                // }
            }

            if (stop) {
                static unsigned char i = 0;
                i++;
                // printf("\n\n");
                // printf("  __ stop is 1\n", i);
                // printf("  __ i is %d\n", i);
                if (i > 0) {
                    stop = false;
                    i = 0;
                    // printf("  __ stop is 0 now\n\n", i);
                }
            }

            {
                printf("  __ 1s: %d fps\n", fps);
                fps = 0;
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

        // Update data (view-mat) according to input, update entity pos, rot, scale...
        // if ( now - last_update_time >= update_cycle ) {
        {   
            win.pollEvents();  // not respond when close win with mouse without this
            
            cam.input_update(win);
            renderer.calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());

            // entity.increasePosition(0.0f, 0.0f, 0.002f);
            // entity.increaseRotation(0.0005f, 0.0f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0005f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0f, 0.005f);

            {
                unsigned short transform_idx = 0;
                float rot_z_step = 0.048f;

                static bool facing_left = true;
                static bool turn_back = false;

                if (!stop) {
                    float delta_rot_z = ( facing_left ) ? (rot_z_step) : (-rot_z_step);
                    delta_rot_z = (turn_back) ? (-delta_rot_z) : (delta_rot_z);
                    
                    for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
                        itr->increaseRotation(transform_idx, 0.0f, 0.0f, delta_rot_z);
                        // itr->setRotZ(0, 3.14f);
                    }

                    Entity *misaEntity = &assimp_misa.entities[0];
                    float misa_angle = (*(misaEntity->getTransformValues(transform_idx)))[Entity::transform::rot_z];

                    if (!turn_back) {
                        if (facing_left && misa_angle > 3.14f + 3.14f / 4.0f) {
                            printf("rot_z max: %f\n", misa_angle);
                            turn_back = true;
                            // stop = true;
                        }
                        else if (!facing_left && misa_angle < -3.14f - 3.14f / 4.0f) {
                            printf("rot_z min: %f\n", misa_angle);
                            turn_back = true;
                            // stop = true;
                        }
                    }
                    else {
                        if (misa_angle > -rot_z_step && misa_angle < rot_z_step) {
                            turn_back = false;
                            facing_left = (!facing_left);
                            stop = true;
                        }
                    }
                    
                }
            }

            last_update_time = now;
            updated_times++;
        }

        // Render entities
        // if ( now - last_render_time >= render_cycle ) {
        if (1) {
            renderer.process(light, num_misa_target, num_all_targets);

            last_render_time = now;
            rendered_times++;
        }

        win.swapBuffers();
        fps++;
    }

    for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
        itr->cleanUp();
    }
    multi_vbo_entity.cleanUp();
    single_vbo_entity.cleanUp();

    assimp_misa.cleanUp();
    
    renderer.cleanUp();
    loader.cleanUp();

    win.stop();
    return 0;
}
