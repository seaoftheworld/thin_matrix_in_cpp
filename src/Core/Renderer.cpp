#include "Renderer.h"

#include <iostream>

#define NEAR_PLANE (0.1f)
#define FAR_PLANE  (9999.0f)
#define FOV        (45.0f)

#define MOUSE_ROTATE_SPEED (0.005f)
#define     KEY_MOVE_SPEED (0.05f)
#define             FACTOR (1.0f)

static const float skybox_init_data_array[NUM_SKYBOX_VERTICES * 3] = {

    // Bottom
    -1,  1, -1,
    -1, -1, -1,
     1,  1, -1,
     1,  1, -1,
     1, -1, -1,
    -1, -1, -1,

    // Left
    -1,  1, -1,
    -1, -1, -1,
    -1, -1,  1,

    -1, -1,  1,
    -1,  1,  1,
    -1,  1, -1,

    // Right
     1, -1, -1,
     1,  1, -1,
     1,  1,  1,
     
     1,  1,  1,
     1, -1,  1,
     1, -1, -1,

    // Front
    -1,  1, -1,  // 1 +--+ 2
    -1,  1,  1,  //   | /
     1,  1,  1,  // 0 |/

     1,  1,  1,  //      + 3
     1,  1, -1,  //     /|
    -1,  1, -1,  // 5  /_| 4

    // Top
    -1, -1,  1,
     1, -1,  1,
     1,  1,  1,
     1,  1,  1,
    -1,  1,  1,
    -1, -1,  1,

    // Back
    -1, -1, -1,
    -1, -1,  1,
     1, -1, -1,
     1, -1, -1,
    -1, -1,  1,
     1, -1,  1,
};

void HighLevelRenderer::calculateProjMatrix() {
    projection_matrix = gl_math::perspective(gl_math::radians(FOV), (float)WIN_WIDTH / WIN_HEIGHT, NEAR_PLANE, FAR_PLANE);
}

// void EntityRenderer::process(Entity *entity, BaseShader *shader, unsigned int str_rot) {
void EntityRenderer::render(Entity *entity, BaseShader *shader) {
    // shader->start();
    if (!entity || !shader) {
        return;
    }

    // Setting this to be 0 could make drawing unable to crash the program,
    // when NULL-texture/model pointers are input by mistake.
    unsigned int indices_count = 0;

    StaticTexture *static_texture = entity->getTexture();
    BaseModel *static_model = entity->getModel();

    if (static_texture) {
        // bind a texture buffer object
        int texture_id = static_texture->getId();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }

    if (static_model) {

        switch (static_model->getModelType()) {    

            case BaseModel::single_vbo:
            {
                indices_count = ((StaticModel_SingleVbo *)static_model)->getNumIndices();

                // TODO: use to C++ static/dynamic cast instead
                int vbo_id = ((StaticModel_SingleVbo*)static_model)->getVboId();
                int ibo_id = ((StaticModel_SingleVbo*)static_model)->getIboId();

                // bind the all-in-one vbo
                {
                    // enable a corresponding-attribute in v-shader (pos, uv, or normal), then
                    // set how to interprete all-in-one vbo data for each attribute's data.
                    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

                    for (unsigned int i = 0; i < StaticModel_SingleVbo::valid_attr_num; i++) {
                        
                        glEnableVertexAttribArray(StaticModel_SingleVbo::valid_attr_idx[i]);

                        glVertexAttribPointer(
                            StaticModel_SingleVbo::valid_attr_idx[i],
                            StaticModel_SingleVbo::valid_attr_stride[i], GL_FLOAT,
                            GL_FALSE,
                            StaticModel_SingleVbo::single_vbo_stride_in_float << 2,// N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                            (const void *)StaticModel_SingleVbo::valid_attr_offset[i]);  // offset is measured in Bytes
                    }
                }

                // bind the ibo
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
            }
            break;

            case BaseModel::multi_vbos:
            {
                indices_count = ((StaticModel *)static_model)->getNumIndices();

                int vbos_ibo_ids[StaticModel::allBuffNum];
                ((StaticModel *)static_model)->getBuffers(&vbos_ibo_ids);
                    // printf("vbo-num: %d, all buffnum: %d", StaticModel::vboNum, StaticModel::allBuffNum);
                    // printf("ids: %d, %d, %d  ", vbos_ibo_ids[0], vbos_ibo_ids[1], vbos_ibo_ids[2]);
                    // printf("attr-idx: %d, %d ", StaticModel::vbo_atrr_idx[0], StaticModel::vbo_atrr_idx[1]);
                    // printf("stride: %d, %d\n", StaticModel::vbo_stride[0], StaticModel::vbo_stride[1]);

                // bind vbo, enable its corresponding-attribute in v-shader (pos, uv, or normal), then
                // set how to interpretate vbo data into the attribute.
                for (unsigned int i = 0; i < StaticModel::vboNum; i++) {
                    glBindBuffer(GL_ARRAY_BUFFER, vbos_ibo_ids[i]);
                    
                    glEnableVertexAttribArray(StaticModel::vbo_atrr_idx[i]);
                    
                    glVertexAttribPointer(
                        StaticModel::vbo_atrr_idx[i], 
                        StaticModel::vbo_stride[i], GL_FLOAT, 
                        GL_FALSE, 
                        StaticModel::vbo_stride[i] << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                        0);
                }

                // bind the ibo
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_ibo_ids[StaticModel::vboNum] );
            }
            break;

            default:
            break;
        }
    }

    // set the uniforms for shaders before every time's draw
    {
        // unsigned int num_entities = entity->getInfoSize();
        unsigned int transforms_num = entity->tranformsNum();

        for (unsigned int i = 0; i < transforms_num; i++) {
            
            {
                gl_math::mat4 model_matrix;
                // if (str_rot) {
                //     // gl_math::model_matrix_STR_Rot(entity->getInfo(i), &model_matrix);
                //     gl_math::model_matrix_STR_Rot(entity->getTransformValues(i), &model_matrix);
                // }
                // else 
                {
                    // gl_math::model_matrix_SRT_Normal(entity->getInfo(i), &model_matrix);
                    gl_math::model_matrix_SRT_Normal(entity->getTransformValues(i), &model_matrix);
                }

                // gl_math::mat4 mvp = gl_math::mat4_multiply(projection_matrix, view_matrix, model_matrix);
                // shader->loadTransformMatrix(&mvp[0][0]);

                // Proj-Matrix should have been loaded before 
                // the rendering loop is entered.
                shader->loadTransformMatrix(&model_matrix[0][0]);
            }

            // Draw the triangles
            // printf("draw: %d\n\n", indices_count);
            glDrawElements(
                GL_TRIANGLES,      // mode
                indices_count,     // num of indices
                GL_UNSIGNED_SHORT, // type
                0                  // element array buffer(indices buffer) offset
            );
        }
    }
}

void TerrainRenderer::render(Terrain *terrain, BaseShader *shader) {
    if (!terrain || !shader) {
        return;
    }

    // Setting this to be 0 could make drawing unable to crash the program,
    // when NULL-texture/model pointers are input by mistake.
    unsigned int indices_count = 0;

    TerrainTexturePack *texturePack = terrain->getTexturePack();
    TerrainTexture *blendMap = terrain->getBlendMap();
    BaseModel *static_model = terrain->getModel();

    if (texturePack && blendMap) {
        // bind a texture buffer object
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturePack->getTerrainTexture00()->getTextureID());
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texturePack->getTerrainTexture01()->getTextureID());

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texturePack->getTerrainTexture02()->getTextureID());
        
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texturePack->getTerrainTexture03()->getTextureID());

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, blendMap->getTextureID());
    }

    if (static_model) {
        switch (static_model->getModelType()) {
            // Terrain's model is now generated as multi-vbos model
            case BaseModel::multi_vbos:
            {
                // TODO: casting is no longer necessary here
                indices_count = ((StaticModel *)static_model)->getNumIndices();

                int vbos_ibo_ids[StaticModel::allBuffNum];
                ((StaticModel *)static_model)->getBuffers(&vbos_ibo_ids);

                // bind vbo, enable its corresponding-attribute in v-shader (pos, uv, or normal), then
                // set how to interpretate vbo data into the attribute.
                for (unsigned int i = 0; i < StaticModel::vboNum; i++) {
                    glBindBuffer(GL_ARRAY_BUFFER, vbos_ibo_ids[i]);
                    
                    glEnableVertexAttribArray(StaticModel::vbo_atrr_idx[i]);
                    
                    glVertexAttribPointer(
                        StaticModel::vbo_atrr_idx[i], 
                        StaticModel::vbo_stride[i], GL_FLOAT, 
                        GL_FALSE, 
                        StaticModel::vbo_stride[i] << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                        0);
                }

                // bind the ibo
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_ibo_ids[StaticModel::vboNum] );
            }
            break;

            default:
            break;
        }
    }

    // set the model-matrix uniform for the shaders before drawing the terrain
    {
        float terrain_model_matrix[Entity::transform::max] = {
            terrain->getXOffset(), terrain->getYOffset(), 0.0f, 
            0.0f, 0.0f, 0.0f, 
            1.0f
        };

        gl_math::mat4 model_matrix;
        gl_math::model_matrix_SRT_Normal(&terrain_model_matrix, &model_matrix);

        // Proj-Matrix and View-Matrix should have been loaded before 
        // reaching here
        shader->loadTransformMatrix(&model_matrix[0][0]);
        
        // Draw the triangles
        // printf("draw: %d\n\n", indices_count);
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices_count,     // num of indices
            GL_UNSIGNED_SHORT, // type
            0                  // element array buffer(indices buffer) offset
        );
    }
}

void Camera::input_update(WindowSystem &win) {
    // glfwGetTime is called only once for lastTime (the first time this function is called)
    //
    // static double lastTime = glfwGetTime();
    // float deltaTime; {
    //     // Compute time difference between current and last frame
    //     double currentTime = glfwGetTime();
    //     deltaTime = float(currentTime - lastTime);
    // }

    // Set cam's h/v angle according to cursor movement
    {
        // Get mouse position
        double xpos, ypos; {
            win.getCursorPos(&xpos, &ypos);
        }

        if (xpos != WIN_WIDTH / 2 || ypos != WIN_HEIGHT / 2) {
            // Reset mouse position for next frame
            win.setCursorPos(WIN_WIDTH / 2, WIN_HEIGHT / 2);

            // Compute new orientation
            hAngle += MOUSE_ROTATE_SPEED * (float)(xpos - WIN_WIDTH / 2);  // moveLeft --, moveRight ++
            vAngle += MOUSE_ROTATE_SPEED * (float)(WIN_HEIGHT / 2 - ypos); // moveUp ++, moveDown --

            calculateDirecRightUp();
        }
    }

    // Set cam's pos according to key input
    {
        // Move forward
        if (win.getKey( WIN_SYS_KEY_UP ) == WIN_SYS_PRESS) {
            position += direc * FACTOR * KEY_MOVE_SPEED;
        }
        // Move backward
        if (win.getKey( WIN_SYS_KEY_DOWN ) == WIN_SYS_PRESS) {
            position -= direc * FACTOR * KEY_MOVE_SPEED;
        }
        // Strafe right
        if (win.getKey( WIN_SYS_KEY_RIGHT ) == WIN_SYS_PRESS) {
            position += right * FACTOR * KEY_MOVE_SPEED;
        }
        // Strafe left
        if (win.getKey( WIN_SYS_KEY_LEFT ) == WIN_SYS_PRESS) {
            position -= right * FACTOR * KEY_MOVE_SPEED;
        }

        if (win.getKey( WIN_SYS_KEY_R ) == WIN_SYS_PRESS) {
            // float KEY_MOVE_SPEED = .01f;
            position.z += KEY_MOVE_SPEED * FACTOR;
        }
        if (win.getKey( WIN_SYS_KEY_F ) == WIN_SYS_PRESS) {
            // float KEY_MOVE_SPEED = .01f;
            position.z -= KEY_MOVE_SPEED * FACTOR;
        }

        if (win.getKey(WIN_SYS_KEY_V) == WIN_SYS_PRESS) {
            // float KEY_MOVE_SPEED = .01f;
            printf("cam  position: %f, %f, %f\n", position.x, position.y, position.z);
            printf("h angle      : %f\n", hAngle);
            printf("v angle      : %f\n\n", vAngle);
            printf("cam direction: %f, %f, %f\n", direc.x, direc.y, direc.z);
            printf("cam        up: %f, %f, %f\n\n\n", up.x, up.y, up.z);
        }
    }
}

Skybox::Skybox(Loader *loader, std::string imgPaths[][6], unsigned int cube_edge_length) {
    if (!loader || !imgPaths) {
        return;
    }

    float vertices_data[NUM_SKYBOX_VERTICES * 3];

    for (unsigned int i = 0; i < NUM_SKYBOX_VERTICES * 3; i++) {
        vertices_data[i] = cube_edge_length * skybox_init_data_array[i];
    }

    cube = loader->loadRawModel(vertices_data, 3, NUM_SKYBOX_VERTICES);
    printf("cube: %p\n", cube);
    if (cube) {
        printf("cube vbo: %d\n", cube->getVboID());
        printf("cube vcount: %d\n", cube->getVertexCount());
    }

    cubeTexture = loader->loadStaticTextureCube(imgPaths);
    printf("cubeTexture: %p\n", cubeTexture);
}

void SkyboxRenderer::render(Skybox *skybox) {
    // shader->start();
    // if (!terrain || !shader) {
    //     return;
    // }

    // Setting this to be 0 could make drawing unable to crash the program,
    // when NULL-texture/model pointers are input by mistake.
    unsigned int vertices_count = 0;
    if (!skybox) {
        return;
    }

    if (skybox->getCubeTexture()) {
        // bind a texture buffer object
        int texture_id = skybox->getCubeTexture()->getId();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    }

    if (skybox->getCubeModel()) {
        vertices_count = skybox->getCubeModel()->getVertexCount();

        glBindBuffer(GL_ARRAY_BUFFER, skybox->getCubeModel()->getVboID());
        
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(
            0, 
            3, GL_FLOAT, 
            GL_FALSE, 
            3 << 2,  // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
            0);
    }

    {
        glDrawArrays(GL_TRIANGLES, 0, vertices_count);
        // glDrawArrays(GL_TRIANGLES, 0, 3 * 3);
    }
}
