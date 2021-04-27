#include "Core/Renderer.h"

#include "Core/Renderers/Model/SpecularShader.h"

#include "Core/Common/data.h"

class SpecularEntityRenderer : public BaseRenderer {
private:
    // StaticShader *entityShader = NULL;
    SpecularShader *entityShader = NULL;

    std::vector<Entity *> entities;

public:
    void addEntity(Entity *input_entity) {
        if (input_entity) {
            entities.push_back(input_entity);
        }
    }

    unsigned int getEntitiesSize() {
        return entities.size();
    }

    void clearEntities() {
        entities.clear();
    }

    std::vector<Entity *> &getEntities() {
        return entities;
    }

public:
    SpecularEntityRenderer() {
        printf("  __ model-renderer constructor called.\n");
        freeShadersData();
        allocShadersData();
    }
    ~SpecularEntityRenderer() {
        printf("  __ model-renderer destructor called.\n");
        freeShadersData();
    }

    void allocShadersData() override;
    void freeShadersData() override;
    bool ready() override;
    
    void run(Light &light) {
    // void run() {

        if (!entityShader) {
            return;
        }

        entityShader->start();
        entityShader->loadViewMatrix( getViewMatrix() );
        
        // entityShader->loadReflectivity(20.0f);
        // entityShader->loadShineDamper(20.0f);
        // entityShader->loadLight(light);        
        //     unsigned int misa_start_idx = 0;
        //     entityRendererWraper(misa_start_idx, misa_entities_num);

        // entityShader->loadReflectivity(10.0f);
        // entityShader->loadShineDamper(10.0f);
        //     unsigned int the_rest_entities_start_idx = misa_entities_num;
        //     entityRendererWraper(the_rest_entities_start_idx, total_entities_num - misa_entities_num);

        entityShader->loadReflectivity(20.0f);
        entityShader->loadShineDamper(20.0f);
        entityShader->loadLight(light);
            render();



        // PROFILE_FUNCTION();
        // prepare();
        // {
        //     PROFILE_SCOPE("entity");
        //     // entityShader->start();
        //     // entityShader->loadViewMatrix( getViewMatrix() );
            
        //     //     entityShader->loadLight(light);
                
        //     //     entityShader->loadReflectivity(20.0f);
        //     //     entityShader->loadShineDamper(20.0f);
        //     //     unsigned int misa_start_idx = 0;
        //     //     entityRendererWraper(misa_start_idx, misa_entities_num);

        //     //     entityShader->loadReflectivity(10.0f);
        //     //     entityShader->loadShineDamper(10.0f);
        //     //     unsigned int the_rest_entities_start_idx = misa_entities_num;
        //     //     entityRendererWraper(the_rest_entities_start_idx, total_entities_num - misa_entities_num);
        // }

        // {
        //     // PROFILE_SCOPE("terrain");
        //     // terrainShader->start();
        //     // terrainShader->loadViewMatrix( getViewMatrix() );

        //     //     terrainShader->loadLight(light);

        //     //     terrainShader->loadReflectivity(20.0f);
        //     //     terrainShader->loadShineDamper(20.0f);
        //     //     terrainRendererWraper();
        // }

        // {
        //     PROFILE_SCOPE("skybox");
        //     // if (sky) {
        //         // skyboxShader->start();
        //         // skyboxShader->loadSkyboxViewMatrix( getViewMatrix() );
        //         //     skyboxRenderer.render(sky);
        //     // }
        // }

        // {
        //     PROFILE_SCOPE("gui overlay");
        //     // guiRenderer.render();
        // }
    }

    void render() {
        for (int i = 0; i < entities.size(); i++) {
            auto entity = entities.begin() + i;
            render((*entity));
        }
    }

    void render(Entity *entity) {

        if (!entity || !entityShader) {
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
                                    StaticModel_SingleVbo::single_vbo_stride_in_float << 2,   // N-floats * 4 ==> stride in bytes (N = 3, 2, ...)
                                    (const void *)StaticModel_SingleVbo::valid_attr_offset[i] // offset is measured in Bytes
                                );
                            }

                            // printf("\n\n\n\n");
                            // printf(" single vbo info, valid_attr_num: %d\n", StaticModel_SingleVbo::valid_attr_num);
                            // printf(" indices count: %d\n", indices_count);
                            // printf("   idx: %d, %d, %d\n", \
                            //     StaticModel_SingleVbo::valid_attr_idx[0],
                            //     StaticModel_SingleVbo::valid_attr_idx[1],
                            //     StaticModel_SingleVbo::valid_attr_idx[2]
                            // );

                            // printf(" strides: %d, %d, %d\n", \
                            //     StaticModel_SingleVbo::valid_attr_stride[0],
                            //     StaticModel_SingleVbo::valid_attr_stride[1],
                            //     StaticModel_SingleVbo::valid_attr_stride[2]
                            // );

                            // printf(" total stride: %d\n", \
                            //     StaticModel_SingleVbo::single_vbo_stride_in_float
                            // );

                            // printf(" offsets: %d, %d, %d\n", \
                            //     StaticModel_SingleVbo::valid_attr_offset[0],
                            //     StaticModel_SingleVbo::valid_attr_offset[1],
                            //     StaticModel_SingleVbo::valid_attr_offset[2]
                            // );
                            // printf("\n\n\n\n");
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
                    // for (unsigned int i = 0; i < 3 - 3; i++) {
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
                    entityShader->loadTransformMatrix(&model_matrix[0][0]);
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

// private:
//     void entityRendererWraper(unsigned int start_entity_idx, unsigned int num) {
//         if (start_entity_idx < getEntitiesSize()) {
//             for (unsigned int i = start_entity_idx; i < getEntitiesSize() && (i - start_entity_idx) < num; i++) {
//                 auto entity = getEntities().begin() + i;
//                 entityRenderer.render(*entity, entityShader);
//             }
//         }
//     }

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
};