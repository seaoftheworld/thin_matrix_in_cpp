// To use OpenGL, Libs depended on are: 
// 1. glfw (included in WindowSystem.h)
// 2. gl3w (included in Renderer.h)
// 3. gl_math

#define __USE_INLINE_METHODS__
#include "WindowSystem/WindowSystem.h"

#include "Core/Renderer.h"
#include "Core/Loader.h"

#include "Core/AssimpLib.h"

#include <iostream>
#include <stdio.h>


int main_01() {
// int main() {

    printf("  __ 01 specular lighting __\n");

    WindowSystem &win = WindowSystem::instance();
    if (win.start() < 0) {
        win.stop();
        return -3;
    }

    std::cout << "  OpenGL Window/Context created, Init GL3W to extend OpenGL for extentions" << std::endl;
    if (!Renderer::init()) {
        std::cout << "  Renderer::init()/gl3wInit() failed !" << std::endl;
        win.stop();
        return -2;
    }

    // -----------------------------
    SpecularShader specular_shader;
    if (specular_shader.getStatus() != BaseShader::link_prog_passed || specular_shader.getProgId() <= 0) {
        printf("  loading static-shader failed");

        specular_shader.cleanUp();

        win.stop();
        return -1;
    }

    // shader.cleanUp();
    // win.stop();
    // return 0;

    Renderer renderer;
    Loader loader;

    Camera cam(gl_math::vec3(0.0f, -2.0f, 2.0f), 0.0f, -3.14f / 4.0f);
    
    Light light; {
        float pos[Light::max_pos] = {0.0f, 1.0f, 2.0f};
        // float color[Light::max_color] = { .8f, .9f, .8f };
        float color[Light::max_color] = { 1.6f, 1.2f, 1.6f };
        light.setPosition(&pos);
        light.setColor(&color);
    }

    //*
    AssimpLib assimp; {
        // float info[Entity::transform::max] = {
        //             0.0f, 0.0f, 0.0f,
        //             3.14f / 2.0f, 0.0f, 0.0f,
        //             0.05f
        // };
        // EntityInfo transform(&info);
        // assimp.loadModel("data/models/qp_01/qp_01.obj", transform, loader);

        // float info[Entity::transform::max] = {
        //             0.0f, 0.0f, 0.0f,
        //             0.0f, 0.0f, 0.0f,
        //             1.0f
        // };
        // EntityInfo transform(&info);
        // assimp.loadModel("data/models/rb73/rb73_yForward_zUp.obj", transform, loader);

        // float info[Entity::transform::max] = {
        //             0.0f, 0.0f, 0.0f,
        //             3.14f / 2.0f, 0.0f, 0.0f,
        //             1.0f
        // };
        // EntityInfo transform(&info);
        // assimp.loadModel("data/models/garbage_contain/Garbage_Container_.obj", transform, loader);

        float info[Entity::transform::max] = {
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    1.0f
        };
        EntityInfo transform(&info);

        // TODO: load model with multi transform info
        assimp.loadModel("data/models/misa/misa_yForward_zUp.obj", transform, loader);

        // float info[Entity::transform::max] = {
        //             0.0f, 0.0f, 0.0f,
        //             3.14f / 2.0f, 0.0f, 0.0f,
        //             4.0f
        // };
        // EntityInfo transform(&info);
        // assimp.loadModel("data/models/gun/Handgun_obj.obj", transform, loader);
    }
    //*/

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
            specular_shader.cleanUp();
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
                specular_shader.cleanUp();
                loader.cleanUp();

                win.stop();
                return -1;
            }

            single_vbo_entity.setTextureModel(textures[1], m); 

            {
                float entity_info[Entity::transform::max] = {
                    // (float)i * 1.0f + 0.5f, (float)i * 1.0f + 0.1f, (float)i * 1.0f + 0.1f,
                    // 0.0f, 0.0f, 0.0f,
                    // (float)i * 0.3f + 0.3f

                    -2.0f, -2.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    4.0f
                };

                EntityInfo info(&entity_info);
                single_vbo_entity.addInfo(info);
            }

            {
                float entity_info[Entity::transform::max] = {
                    -0.5f, -0.5f, 2.0f,
                    0.0f, 0.0f, 0.0f,
                    1.0f
                };

                EntityInfo info(&entity_info);
                single_vbo_entity.addInfo(info);
            }
        }

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

            unsigned int test_vertices_count_from_pos = ARRAY_SIZE(test_pos) / StaticShader::attr_stride[StaticShader::id0_pos3f];
            unsigned int test_vertices_count_from_uv = ARRAY_SIZE(test_uv) / StaticShader::attr_stride[StaticShader::id1_uv2f];

            unsigned short test_indices[] = { 0, 1, 2, 0, 3, 2 };

            printf("  vertices counts from pos/uv are %d, %d\n", test_vertices_count_from_pos, test_vertices_count_from_uv);
            printf("  indices counts are %d\n", (int)ARRAY_SIZE(test_indices));
            
            StaticModel *model = loader.loadStaticModel(
                &test_vertex_attr, test_vertices_count_from_pos, 
                test_indices, ARRAY_SIZE(test_indices));

            if (!model) {
                specular_shader.cleanUp();
                loader.cleanUp();

                win.stop();
                return -1;
            }

            multi_vbo_entity.setTextureModel(textures[0], model);

            for (unsigned int i = 0; i < 10; i++) {
                float entity_info[Entity::transform::max] = {
                    (float)i * 1.0f + 0.5f, (float)i * 1.0f + 0.1f, (float)i * 1.0f + 0.1f,
                    0.0f, 0.0f, 0.0f,
                    (float)i * 0.3f + 0.3f
                };

                EntityInfo info(&entity_info);
                multi_vbo_entity.addInfo(info);
            }
            //*/
        }
    }

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

    specular_shader.start();
    specular_shader.loadProjMatrix(renderer.getProjMatrix());
    specular_shader.stop();

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
                // unsigned short et_idx = 0;
                // float rot_z_step = 0.785f;

                // static bool increase = false;
                
                // for (auto itr = assimp.entities.begin(); itr != assimp.entities.end(); itr++) {
                //     float delta_rot_z = ( increase ) ? (rot_z_step) : (-rot_z_step);
                //     (*itr).increaseRotation(et_idx, 0.0f, 0.0f, delta_rot_z);
                // }

                // if ((*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z] > 3.14f) {
                //     // printf("rot_z max: %f\n", (*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
                //     increase = false;
                // }
                // else if ((*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z] < -3.14f) {
                //     // printf("rot_z min: %f\n", (*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
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

        // Update data (view-mat) according to input, update entity pos, rot, scale...
        // if ( now - last_update_time >= update_cycle ) {
        if (1) {
            
            win.pollEvents();  // not respond when close win with mouse without this
            cam.input_update(win);

            // entity.increasePosition(0.0f, 0.0f, 0.002f);
            // entity.increaseRotation(0.0005f, 0.0f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0005f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0f, 0.005f);

            {
                //*
                unsigned short et_idx = 0;
                float rot_z_step = 0.016f;

                static bool increase = false;

                if (!stop) {
                    float delta_rot_z = ( increase ) ? (rot_z_step) : (-rot_z_step);
                    
                    for (auto itr = assimp.entities.begin(); itr != assimp.entities.end(); itr++) {
                        itr->increaseRotation(et_idx, 0.0f, 0.0f, delta_rot_z);
                        // itr->setRotZ(0, 3.14f);
                    }

                    if ((*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z] > 3.14f) {
                        printf("rot_z max: %f\n", (*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
                        increase = false;
                        stop = true;
                    }
                    else if ((*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z] < -3.14f) {
                        printf("rot_z min: %f\n", (*assimp.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
                        increase = true;
                        stop = true;
                    }
                }
                //*/
            }

            renderer.calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());

            last_update_time = now;
            updated_times++;
        }

        // Render entities
        // if ( now - last_render_time >= render_cycle ) {
        if (1) {
            renderer.prepare();    
                specular_shader.start();
                specular_shader.loadViewMatrix(renderer.getViewMatrix());

                    specular_shader.loadLightPosition(light.getPosition3fv());
                    specular_shader.loadLightColor(light.getColor3fv());

                    specular_shader.loadReflectivity(15.0f);
                    specular_shader.loadShineDamper(20.0f);
                    for (auto itr = assimp.entities.begin(); itr != assimp.entities.end(); itr++) {
                        renderer.render(&(*itr), &specular_shader);
                    }

                    specular_shader.loadReflectivity(1.0f);
                    specular_shader.loadShineDamper(1.0f);
                    renderer.render(&multi_vbo_entity, &specular_shader);
                    renderer.render(&single_vbo_entity, &specular_shader);

            last_render_time = now;
            rendered_times++;
        }

        win.swapBuffers();
        fps++;
    }

    for (auto itr = assimp.entities.begin(); itr != assimp.entities.end(); itr++) {
        itr->cleanUp();
    }
    multi_vbo_entity.cleanUp();
    single_vbo_entity.cleanUp();

    specular_shader.stop();
    specular_shader.cleanUp();

    loader.cleanUp();
    assimp.cleanUp();

    win.stop();
    return 0;
}
