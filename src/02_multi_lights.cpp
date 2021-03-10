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


// int main_02() {
int main() {

    printf("  __ 02 multi lights __\n");

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
    MultiLightsShader multiLightsShader;
    if (multiLightsShader.getStatus() != BaseShader::link_prog_passed || multiLightsShader.getProgId() <= 0) {
        printf("  loading static-shader failed");

        multiLightsShader.cleanUp();

        win.stop();
        return -1;
    }

    // shader.cleanUp();
    // win.stop();
    // return 0;

    // printf("shader loaded, press anything to continue ...\n\n"); {
    //     int dbg;
    //     scanf("%d", &dbg);
    // }

    Renderer renderer;
    Loader loader;

    Camera cam(gl_math::vec3(0.0f, -2.0f, 2.0f), 0.0f, -3.14f / 4.0f);

    AssimpLib assimp_misa; {

        float values[Entity::transform::max] = {
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    1.0f
        };
        Transform transform(&values);
        Transform *transforms = &transform;

        // TODO: load model with multi transform info, done
        //       improve meshes' hierachy
        assimp_misa.loadModel("data/models/misa/misa_yForward_zUp.obj", loader, &transforms, 1);

        // printf("misa model loaded, press anything to continue ...\n\n"); {
        //     int dbg;
        //     scanf("%d", &dbg);
        // }
    }

    AssimpLib assimp_light_crate;
    Light lights[4]; {
        
        // float color[Light::max_color] = { 2.0f, 0.0f, 0.0f };      // red
        float light_color0[Light::max_color] = { 1.6f, 1.0f, 1.6f };  // pink
        // float light_color0[Light::max_color] = { 0.0f, 2.0f, 0.0f };  // green
        // float light_pos0[Light::max_pos] = {0.0f, 1.0f, 2.0f};
        // float light_color0[Light::max_color] = { 0.0f, 0.0f, 0.0f };  // none
        float light_pos0[Light::max_pos] = {0.0f, 2.0f, 2.0f};

        float light_color1[Light::max_color] = { 1.2f, 1.2f, 0.6f };  // red + green ==> yellow
        // float light_color1[Light::max_color] = { 1.6f, 1.2f, 1.6f };  // pink
        // float light_pos1[Light::max_pos] = {-2.0f, 0.0f, 1.5f};
        float light_pos1[Light::max_pos] = {0.0f, -2.0f, 1.5f};
        
        float light_color2[Light::max_color] = { 0.0f, 2.0f, 0.0f };  // green
        float light_pos2[Light::max_pos] = {0.0f, -2.0f, 1.0f};
        
        float light_color3[Light::max_color] = { 1.6f, 1.2f, 1.6f };  // pink
        float light_pos3[Light::max_pos] = {1.0f, 0.0f, 0.5f};

            lights[0].setPosition(&light_pos0);
            lights[0].setColor(&light_color0);

            // float pos[Light::max_pos] = {-4.0f, -4.0f, 1.0f};
            // float color[Light::max_color] = { 2.0f, 2.0f, 0.0f };  // red + green ==> yellow
            lights[1].setPosition(&light_pos1);
            lights[1].setColor(&light_color1);

            // float pos[Light::max_pos] = {0.0f, -4.0f, 1.0f};
            // float color[Light::max_color] = { 0.0f, 2.0f, 0.0f };  // green
            lights[2].setPosition(&light_pos2);
            lights[2].setColor(&light_color2);

            // float pos[Light::max_pos] = {0.0f, 1.0f, 2.0f};
            // float color[Light::max_color] = { 1.6f, 1.2f, 1.6f };  // pink
            lights[3].setPosition(&light_pos3);
            lights[3].setColor(&light_color3);

            {
                // Modify transform info in the following lines for 
                // the 4 models(of 1 entity) to be displayed:
                float crate_transform_value0[] = {   
                    light_pos0[0], light_pos0[1], light_pos0[2],
                    0.0f, 0.0f, 0.0f,
                    0.2f
                };
                float crate_transform_value1[] = {
                    light_pos1[0], light_pos1[1], light_pos1[2],
                    // 0.0f, 0.0f, 6.28 / 4.0f,
                    0.0f, 0.0f, 0.0f,
                    0.2f
                };
                float crate_transform_value2[] = {   
                    light_pos2[0], light_pos2[1], light_pos2[2],
                    // 0.0f, 0.0f, 3.14f,
                    0.0f, 0.0f, 0.0f,
                    0.2f
                };
                float crate_transform_value3[] = {   
                    light_pos3[0], light_pos3[1], light_pos3[2],
                    // 0.0f, 0.0f, 3.14f + 6.28 / 4.0f,
                    0.0f, 0.0f, 0.0f,
                    0.2f
                };

                // To load and display the 4 models (of 1 entity) with different transforms(pos, rot, scale),
                // modify the lines above to change their transforms, 
                // the following lines doesnot has to be modified.
                {
                    Transform crate_transform0(&crate_transform_value0);
                    Transform crate_transform1(&crate_transform_value1);
                    Transform crate_transform2(&crate_transform_value2);
                    Transform crate_transform3(&crate_transform_value3);

                    Transform *crate_transforms[] = {
                        &crate_transform0,
                        &crate_transform1,
                        &crate_transform2,
                        &crate_transform3
                    };

                    assimp_light_crate.loadModel( "data/models/crate/Crate1.obj", loader, crate_transforms, ARRAY_SIZE(crate_transforms) );
                }
            }

        // Doesnt work before shader is bound/opened/started .... !!!
        // multiLightsShader.loadLights(&light_pos[0], &light_color[0], 4);

        // printf("4 lights inited and crate model loaded, press anything to continue ...\n\n"); {
        //     int dbg;
        //     scanf("%d", &dbg);
        // }
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
            multiLightsShader.cleanUp();
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
                multiLightsShader.cleanUp();
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

            unsigned int test_vertices_count_from_pos = ARRAY_SIZE(test_pos) / StaticShader::attr_stride[StaticShader::id0_pos3f];
            unsigned int test_vertices_count_from_uv = ARRAY_SIZE(test_uv) / StaticShader::attr_stride[StaticShader::id1_uv2f];

            unsigned short test_indices[] = { 0, 1, 2, 0, 3, 2 };

            printf("  vertices counts from pos/uv are %d, %d\n", test_vertices_count_from_pos, test_vertices_count_from_uv);
            printf("  indices counts are %d\n", (int)ARRAY_SIZE(test_indices));
            
            StaticModel *model = loader.loadStaticModel(
                &test_vertex_attr, test_vertices_count_from_pos, 
                test_indices, ARRAY_SIZE(test_indices));

            if (!model) {
                multiLightsShader.cleanUp();
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


    multiLightsShader.start();
    multiLightsShader.loadProjMatrix(renderer.getProjMatrix());
    multiLightsShader.stop();

    const float *light_color[4] = {NULL, NULL, NULL, NULL};
    const float *light_pos[4] = {NULL, NULL, NULL, NULL};

    float light1_pos[3] = {0.0f};
    float light2_pos[3] = {0.0f};
    float light3_pos[3] = {0.0f};
    float light4_pos[3] = {0.0f};

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

        // Update data (view-mat) according to input, update entity pos, rot, scale...
        // if ( now - last_update_time >= update_cycle ) {
        if (1) {
            
            win.pollEvents();  // not respond when close win with mouse without this
            cam.input_update(win);

            // entity.increasePosition(0.0f, 0.0f, 0.002f);
            // entity.increaseRotation(0.0005f, 0.0f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0005f, 0.0f);
            // entity.increaseRotation(0.0f, 0.0f, 0.005f);

            // Update the lights
            {
                {
                    //*
                    unsigned short transform_idx = 0;
                    float rot_z_step = 0.016f;
                    float y_step = 0.016f;

                    enum Move {
                        rotate = 0, get_closer, get_further, max
                    };
                    static Move move = rotate;

                    if (!stop) {

                        for (auto itr = assimp_light_crate.entities.begin(); itr != assimp_light_crate.entities.end(); itr++) {

                            float rot_z = (*(itr->getTransformValues(0)))[Entity::transform::rot_z];
                            // float pos_y = (*(itr->getTransformValues(0)))[Entity::transform::y];
                            float pos_y;

                            switch (move) {
                                case rotate:
                                    rot_z = (rot_z > 6.28f) ? (0.0f) : (rot_z + rot_z_step);
                                    // rot_z += rot_z_step;
                                    // if (rot_z > 6.28f) {
                                    //     rot_z = 0;
                                    // }

                                    // for (unsigned int i = 0; i < itr->tranformsNum(); i++) {
                                    for (unsigned int i = 0; i < 2; i++) {
                                        // itr->increaseRotation(i, 0.0f, 0.0f, rot_z_step);
                                        // itr->setRotZ(i, rot_z);
                                        itr->setTransformValue(i, Entity::transform::rot_z, rot_z);
                                    }

                                    if (rot_z > 3.14f / 2.0f && rot_z < 3.14f / 2.0f + rot_z_step) {
                                        stop = true;
                                        move = get_closer;
                                    }
                                    if (rot_z > 3.14f + 3.14f/2.0f && rot_z < 3.14f + 3.14f / 2.0f + rot_z_step) {
                                        stop = true;
                                        move = get_closer;
                                    }
                                break;

                                case get_closer:
                                    // for (unsigned int i = 0; i < itr->tranformsNum(); i++) {
                                    for (unsigned int i = 0; i < 2; i++) {
                                        itr->increaseTransformValue(i, Entity::transform::y, -y_step);
                                    }

                                    pos_y = (*(itr->getTransformValues(0)))[Entity::transform::y];
                                    if ( pos_y < 1.0f ) {
                                        stop = true;
                                        move = get_further;
                                    }
                                break;

                                case get_further:
                                    // for (unsigned int i = 0; i < itr->tranformsNum(); i++) {
                                    for (unsigned int i = 0; i < 2; i++) {
                                        itr->increaseTransformValue(i, Entity::transform::y, y_step);
                                    }

                                    pos_y = (*(itr->getTransformValues(0)))[Entity::transform::y];
                                    if ( pos_y > 2.0f ) {
                                        stop = true;
                                        move = rotate;
                                    }
                                default:
                                break;
                            }
                        }
                    }

                    float light_rot_z = ((*(assimp_light_crate.entities.begin()->getTransformValues(transform_idx)))[Entity::transform::rot_z]);
                    // printf("light-1 crate rot_z: %f\n", light_rot_z);
                    
                    float light_pos_input[3] = {0.0f}; {
                        // position-data from light_crate_1 is always -2, -2, 1, this crate's on-screen position
                        // depends on its rot_z data, calculated by STR matrix, not SRT matrix (to rotate around world Z-axis, not model's Z-axis),
                        // its corresonding light's pos shall also be calculated from (-2, -2, 1) and 'rot_z'
                        light_pos_input[0] = ((*(assimp_light_crate.entities.begin()->getTransformValues(transform_idx)))[Entity::transform::x]);
                        light_pos_input[1] = ((*(assimp_light_crate.entities.begin()->getTransformValues(transform_idx)))[Entity::transform::y]);
                        light_pos_input[2] = ((*(assimp_light_crate.entities.begin()->getTransformValues(transform_idx)))[Entity::transform::z]);
                        gl_math::xyz_rot_z(&light_pos_input, light_rot_z, &light1_pos);

                        light_pos_input[0] = ((*(assimp_light_crate.entities.begin()->getTransformValues(transform_idx + 1)))[Entity::transform::x]);
                        light_pos_input[1] = ((*(assimp_light_crate.entities.begin()->getTransformValues(transform_idx + 1)))[Entity::transform::y]);
                        light_pos_input[2] = ((*(assimp_light_crate.entities.begin()->getTransformValues(transform_idx + 1)))[Entity::transform::z]);
                        gl_math::xyz_rot_z(&light_pos_input, light_rot_z, &light2_pos);

                        // light_pos_input[0] = ((*(assimp_light_crate.entities.begin()->getTransformValues(et_idx + 2)))[Entity::transform::x]);
                        // light_pos_input[1] = ((*(assimp_light_crate.entities.begin()->getTransformValues(et_idx + 2)))[Entity::transform::y]);
                        // light_pos_input[2] = ((*(assimp_light_crate.entities.begin()->getTransformValues(et_idx + 2)))[Entity::transform::z]);
                        // gl_math::xyz_rot_z(&light_pos_input, light_rot_z, &light3_pos);

                        // light_pos_input[0] = ((*(assimp_light_crate.entities.begin()->getTransformValues(et_idx + 3)))[Entity::transform::x]);
                        // light_pos_input[1] = ((*(assimp_light_crate.entities.begin()->getTransformValues(et_idx + 3)))[Entity::transform::y]);
                        // light_pos_input[2] = ((*(assimp_light_crate.entities.begin()->getTransformValues(et_idx + 3)))[Entity::transform::z]);
                        // gl_math::xyz_rot_z(&light_pos_input, light_rot_z, &light4_pos);
                    }

                    // printf("calculated rot pos: %f, %f, %f\n", 
                    //     light1_pos[0], light1_pos[1], light1_pos[2]);

                    lights[0].setPosition(&light1_pos);
                    lights[1].setPosition(&light2_pos);
                    lights[2].setPosition(&light3_pos);
                    lights[3].setPosition(&light4_pos);
                    //*/

                // const float *light_pos[4]; 
                    light_pos[0] = lights[0].getPosition3fv();
                    light_pos[1] = lights[1].getPosition3fv();
                    light_pos[2] = lights[2].getPosition3fv();
                    light_pos[3] = lights[3].getPosition3fv();
                // }
                // const float *light_color[4]; {
                    light_color[0] = lights[0].getColor3fv();
                    light_color[1] = lights[1].getColor3fv();
                    light_color[2] = lights[2].getColor3fv();
                    light_color[3] = lights[3].getColor3fv();
                }

                        // printf("light-1 crate pos: %f, %f, %f\n", 
                        //     *(assimp_light_crate1.entities.begin()->getInfo(et_idx))[Entity::x],
                        //     *(assimp_light_crate1.entities.begin()->getInfo(et_idx))[Entity::y],
                        //     *(assimp_light_crate1.entities.begin()->getInfo(et_idx))[Entity::z]);
                        // printf("light-1 crate rot_z: %f\n", light_rot_z);
                            // *(assimp_light_crate1.entities.begin()->getInfo(et_idx))[Entity::rot_z]);

                        // printf("light-1 pos: %f, %f, %f\n", 
                        //     light_pos[1][0], light_pos[1][1], light_pos[1][2]);
                        // printf("\n\n");
            }

            /*
            // Updating misa model
            {
                unsigned short et_idx = 0;
                float rot_z_step = 0.016f;

                static bool increase = false;

                if (!stop) {
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
                        printf("rot_z min: %f\n", (*assimp_misa.entities[0].getTransformValues(et_idx))[Entity::transform::rot_z]);
                        increase = true;
                        stop = true;
                    }
                }
            }
            //*/

            renderer.calculateViewMatrix(cam.getPosition(), cam.getDirection(), cam.getUp());

            last_update_time = now;
            updated_times++;
        }

        // Render entities
        // if ( now - last_render_time >= render_cycle ) {
        if (1) {
            renderer.prepare();    
                multiLightsShader.start();
                multiLightsShader.loadViewMatrix(renderer.getViewMatrix());

                    // multiLightsShader.loadLightPosition(light.getPosition3fv());
                    // multiLightsShader.loadLightColor(light.getColor3fv());
                    // multiLightsShader.loadLights(lights[0].getPosition3fv(), );

                    multiLightsShader.loadReflectivity(15.0f);
                    multiLightsShader.loadShineDamper(20.0f);
                    // printf("loadMultiLights(), use invalid pointers could crash !!! \n");
                    multiLightsShader.loadMultiLights(light_pos, light_color, 2);

                    // printf("render misa\n");
                    for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
                        renderer.render(&(*itr), &multiLightsShader, 0);
                    }

                    for (auto itr = assimp_light_crate.entities.begin(); itr != assimp_light_crate.entities.end(); itr++) {
                        renderer.render(&(*itr), &multiLightsShader, 1);
                    }

                        // // printf("render light crate 0\n");
                        // for (auto itr = assimp_light_crate0.entities.begin(); itr != assimp_light_crate0.entities.end(); itr++) {
                        //     renderer.render(&(*itr), &multiLightsShader, 1);
                        // }
                        // // printf("render light crate 1\n");
                        // for (auto itr = assimp_light_crate1.entities.begin(); itr != assimp_light_crate1.entities.end(); itr++) {
                        //     renderer.render(&(*itr), &multiLightsShader, 1);
                        // }

                    multiLightsShader.loadReflectivity(1.0f);
                    multiLightsShader.loadShineDamper(1.0f);
                    renderer.render(&multi_vbo_entity, &multiLightsShader, 0);
                    renderer.render(&single_vbo_entity, &multiLightsShader, 0);

            last_render_time = now;
            rendered_times++;
        }

        win.swapBuffers();
        fps++;
    }

    for (auto itr = assimp_misa.entities.begin(); itr != assimp_misa.entities.end(); itr++) {
        itr->cleanUp();
    }

    for (auto itr = assimp_light_crate.entities.begin(); itr != assimp_light_crate.entities.end(); itr++) {
        itr->cleanUp();
    }
        // for (auto itr = assimp_light_crate0.entities.begin(); itr != assimp_light_crate0.entities.end(); itr++) {
        //     itr->cleanUp();
        // }
        // for (auto itr = assimp_light_crate1.entities.begin(); itr != assimp_light_crate1.entities.end(); itr++) {
        //     itr->cleanUp();
        // }

    multi_vbo_entity.cleanUp();
    single_vbo_entity.cleanUp();

    multiLightsShader.stop();
    multiLightsShader.cleanUp();

    loader.cleanUp();
    assimp_misa.cleanUp();
    
    // assimp_light_crate0.cleanUp();
    // assimp_light_crate1.cleanUp();
    assimp_light_crate.cleanUp();
    
    win.stop();
    return 0;
}
