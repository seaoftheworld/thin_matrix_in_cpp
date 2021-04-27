// To use OpenGL, Libs depended on are: 
// 1. glfw (included in WindowSystem.h)
// 2. gl3w (included in Renderer.h)
// 3. gl_math

#define __USE_INLINE_METHODS__
#include "WindowSystem/WindowSystem.h"

#include "WrappingRenderer_00.h"

#include "LoadTargets.h"

#include <iostream>
#include <stdio.h>

int test_00_basic_entity_renderer() {

    printf("  __ 00 basic entity renderer __ \n");

    WindowSystem &win = WindowSystem::instance();
    if (win.start() < 0) {
        win.stop();
        return -3;
    }

    std::cout << "  OpenGL Window/Context created (on MsWin), Init GL3W for modern OpenGL" << std::endl;
    if (!BaseRenderer::init()) {
        std::cout << "  Renderer::init()/gl3wInit() failed !" << std::endl;
        win.stop();
        return -2;
    }

    // -----------------------------
    WrappingRenderer_00 abstractRenderer;
    if (!abstractRenderer.entityRenderer.ready()) {
        // loading/compiling/linking shader-program failed
        win.stop();
        return -1;
    }
    abstractRenderer.specificSettingsOn();

    LoadTargets targets; {
        abstractRenderer.entityRenderer.addEntity(targets.getSingleVboEntity());
        abstractRenderer.entityRenderer.addEntity(targets.getMultiVboEntity());

        for (auto misa_entity = targets.getMisa()->entities.begin(); misa_entity != targets.getMisa()->entities.end(); misa_entity++) {
            abstractRenderer.entityRenderer.addEntity(&(*misa_entity));
        }
    }

    // Light test_light; {
    //     // float position[Light::Position::max_pos] = {0.0f, 1.0f, 2.0f};
    //     // float position[Light::Position::max_pos] = {20.0f, 0.0f, 20.0f};
    //     float position[Light::Position::max_pos] = { 20.0f, 20.0f, 20.0f };
    //     // float color[Light::Color::max_color] = {1.6f, 1.2f, 1.6f};
    //     float color[Light::Color::max_color] = { 1.6f, 1.6f, 1.6f };
    //     float dummy_attenuation[Light::Attenuation::max_att] = { 0.0f, 0.0f, 0.0f };
    //     test_light.setValues(&position, &color, &dummy_attenuation);
    // }

    // Camera cam(gl_math::vec3(0.0f, -2.0f, 2.0f), 0.0f, -3.14f / 4.0f);
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
                // unsigned short et_idx = 0;
                // float rot_z_step = 0.785f;

                // static bool increase = false;
                
                // for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
                //     float delta_rot_z = ( increase ) ? (rot_z_step) : (-rot_z_step);
                //     (*itr).increaseRotation(et_idx, 0.0f, 0.0f, delta_rot_z);
                // }

                // if ((*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z] > 3.14f) {
                //     printf("rot_z max: %f\n", (*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
                //     increase = false;
                // }
                // else if ((*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z] < -3.14f) {
                //     printf("rot_z min: %f\n", (*assimp_misa.entities[0].getInfo(et_idx))[Entity::transform::rot_z]);
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
                // if (i > 2) {
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
        {    
            win.pollEvents();  // not respond when close win with mouse without this
            cam.input_update(win);
            BaseRenderer::calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());

            // entity.increasePosition(0.0f, 0.0f, 0.002f);
            // entity.increaseRotation(0.0005f, 0.0f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0005f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0f, 0.005f);

            //*
            {
                unsigned short transform_idx = 0;
                float rot_z_step = 0.048f;

                static bool facing_left = true;
                static bool turn_back = false;

                if (!stop) {
                    float delta_rot_z = ( facing_left ) ? (rot_z_step) : (-rot_z_step);
                    delta_rot_z = (turn_back) ? (-delta_rot_z) : (delta_rot_z);
                    
                    for (auto itr = targets.getMisa()->entities.begin(); itr != targets.getMisa()->entities.end(); itr++) {
                        itr->increaseRotation(transform_idx, 0.0f, 0.0f, delta_rot_z);
                    }

                    Entity *misaEntity = &targets.getMisa()->entities[0];
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
            //*/

            last_update_time = now;
            updated_times++;
        }

        // Render entities
        // if ( now - last_render_time >= render_cycle ) {
        {
            // abstractRenderer.process(test_light);
            abstractRenderer.process();

            last_render_time = now;
            rendered_times++;
        }

        win.swapBuffers();
        fps++;
    }

    win.stop();
    return 0;
}
