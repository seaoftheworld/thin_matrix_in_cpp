// To use OpenGL, Libs depended on are: 
// 1. glfw (included in WindowSystem.h)
// 2. gl3w (included in Renderer.h)
// 3. gl_math

#define __USE_INLINE_METHODS__
#include "WindowSystem/WindowSystem.h"

#include "WrappingRenderer_02.h"

#include "LoadTargets_02.h"

int test_02_multi_lights_entity_renderer() {

    printf("  __ 02 multi-lights entity renderer __\n");

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

    // ---------------------------------
    WrappingRenderer_02 abstractRenderer;
    if (!abstractRenderer.entityRenderer.ready()) {
        win.stop();
        return -1;
    }
    abstractRenderer.specificSettingsOn();

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
            lights[i].setValues(&(LightsPositionsUpdate::initPosition[i]), &light_color[i], &default_att);
        }

        for (int i = 0; i < NUM_LIGHTS; i++) {
        // for (int i = 0; i < 1; i++) {
            abstractRenderer.addLight(&lights[i]);
        }
    }

    // 4 crates represents the positions of 4 lights
    LoadTargets_02 targets; {
        for (auto misa_entity = targets.getMisa()->entities.begin(); misa_entity != targets.getMisa()->entities.end(); misa_entity++) {
            abstractRenderer.entityRenderer.addEntity(&(*misa_entity));
        }
        for (auto crate_entity = targets.getCrate()->entities.begin(); crate_entity != targets.getCrate()->entities.end(); crate_entity++) {
            abstractRenderer.entityRenderer.addEntity(&(*crate_entity));
        }
    }

    // unsigned int num_misa_entities = assimp_misa.entities.size(), num_all_entities = 0; {
    //     // This way doesn't work !!!
    //     //
    //     // for (auto misa_entity : assimp_misa.entities) {
    //     //     hlRenderer.addEntity(&misa_entity);
    //     // }

    //     for (auto misa_entity = assimp_misa.entities.begin(); misa_entity != assimp_misa.entities.end(); misa_entity++) {
    //         renderer.addEntity( &(*misa_entity) );
    //     }
    //     for (auto crate_entity = assimp_crate.entities.begin(); crate_entity != assimp_crate.entities.end(); crate_entity++) {
    //         renderer.addEntity( &(*crate_entity) );
    //     }

    //     renderer.addEntity(&single_vbo_entity);
    //     renderer.addEntity(&multi_vbo_entity);

    //     num_all_entities = renderer.getEntitiesSize();
    //     printf("  __num of meshes for misa/all: %d, %d\n", num_misa_entities, num_all_entities);
    // }

    // Camera cam(gl_math::vec3(-1.93f, 2.4f, 3.9f), 0.0f, -3.14f / 4.0f);
    // Camera cam(gl_math::vec3(-1.93f, 2.4f, 3.9f), 2.68f, -0.7f);
    Camera cam(gl_math::vec3(-1.36f, 3.15f, 4.25f), 2.65f, -0.765f);
    LightsPositionsUpdate lightsPositionsUpdate;

    // float clipPlane_dummy[][4] = {0.0f, 0.0f, -1.0f, 0.0f};  // dummy since clipPlane not enabled

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
        {    
            win.pollEvents();  // not respond when close win with mouse without this
            cam.input_update(win);
            BaseRenderer::calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());

            // entity.increasePosition(0.0f, 0.0f, 0.002f);
            // entity.increaseRotation(0.0005f, 0.0f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0005f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0f, 0.005f);

            // Update the 4 lights according to positions of the 4 crates
            {
                // The 1st and the only entity in 'assimp_crate' is the crate model with 4 transforms
                Entity *crate = &(targets.getCrate()->entities[0]);

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
                    lightsPositionsUpdate.run();
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

                        lightsPositionsUpdate.getPosRotz(i, &xyz_rotz);

                        // Revize the xyz info according to rot-z, the xyz from UpdatePosRotz
                        // is not rotated, rotation info is only in rotz value
                        gl_math::get_xyz_from_rotz(&xyz_rotz.pos, xyz_rotz.rot, &xyz_rotz.pos);
                            // xyz_rotz.pos[Light::Position::x] += 2.0f;
                            // xyz_rotz.pos[Light::Position::y] += 2.0f;

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
                    
                    for (auto itr = targets.getMisa()->entities.begin(); itr != targets.getMisa()->entities.end(); itr++) {
                    // auto itr = assimp_misa.entities.begin(); {
                        itr->increaseRotation(et_idx, 0.0f, 0.0f, delta_rot_z);
                        // itr->setRotZ(0, 3.14f);
                    }

                    if ( (*(targets.getMisa()->entities[0].getTransformValues(et_idx)))[Entity::transform::rot_z] > 3.14f ) {
                        printf("rot_z max: %f\n", (*targets.getMisa()->entities[0].getTransformValues(et_idx))[Entity::transform::rot_z]);
                        increase = false;
                        stop = true;
                    }
                    else if ( (*(targets.getMisa()->entities[0].getTransformValues(et_idx)))[Entity::transform::rot_z] < -3.14f ) {
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
