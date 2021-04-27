#include "LoadTargets.h"

#include "Core/Renderers/Model/StaticShader.h"

void LoadTargets::initSingleVboEntity() {

    float single_vbo_data[] = { 
        -0.5f, 0.5f,  -0.0f, 
         0.0f, 0.0f, 
         0.0f, 0.0f, 1.0f, 

         0.5f, 0.5f,  -0.0f, 
         1.0f, 0.0f, 
         0.0f, 0.0f, 1.0f, 

         0.5f, -0.5f, -0.0f, 
         1.0f, 1.0f, 
         0.0f, 0.0f, 1.0f, 

        -0.5f, -0.5f, -0.0f,
         0.0f, 1.0f,
         0.0f, 0.0f, 1.0f
    };

    unsigned short indices[] = { 
        0, 1, 2, 0, 3, 2 
    };

    StaticModel_SingleVbo *m = NULL; {
        unsigned int vertices_count = ARRAY_SIZE(single_vbo_data) / StaticModel_SingleVbo::single_vbo_stride_in_float;
        unsigned int indices_count = ARRAY_SIZE(indices);
        printf("  single-vbo: vertices count is: %d\n", vertices_count);
        printf("               indices count is: %d\n", indices_count);

        m = loader.loadSingleVboModel(single_vbo_data, vertices_count, indices, indices_count);
        if (!m) {
            printf("  Failed to generate model for single_vbo_entity\n");
            // return false;
        }

        printf("  3D model for single_vbo_entity generated,");
        printf("    vbo/ibo-ids: %d, %d\n\n", m->getVboId(), m->getIboId());
    }

    StaticTexture *texture = NULL; {
        std::string texture_file = "data/tex/road.png";

        loader.loadStaticTextures(&texture_file, 1, &texture);
        if (!texture) {
            printf("  Failed to generate texture for single_vbo_entity\n");
            // return false;
        }

        printf("  Texture for single_vbo_entity generated,");
        printf("    tex-id: %d\n\n", texture->getId());
    }

    if (texture && m) {

        single_vbo_entity.setTextureModel(texture, m);

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

void LoadTargets::initMultiVboEntity() {

    float test_pos[] = { -0.5f, 0.5f,  0.0f, 
                          0.5f, 0.5f,  0.0f, 
                          0.5f, -0.5f, 0.0f, 
                         -0.5f, -0.5f, 0.0f };

    float test_uv[] = { 0.0f, 0.0f, 
                        1.0f, 0.0f, 
                        1.0f, 1.0f, 
                        0.0f, 1.0f };

    float dummy_normal[] = { 0.0f, 0.0f, 1.0f, 
                             0.0f, 0.0f, 1.0f, 
                             0.0f, 0.0f, 1.0f, 
                             0.0f, 0.0f, 1.0f };

    float *test_vertex_attr[] = {
        test_pos, 
        test_uv,
        dummy_normal
    };

    unsigned short test_indices[] = { 0, 1, 2, 0, 3, 2 };

    StaticModel *model = NULL; {
        unsigned int vertices_count_from_pos    = ARRAY_SIZE(test_pos) / StaticShader::attr_stride[StaticShader::id0_pos3f];
        unsigned int vertices_count_from_uv     = ARRAY_SIZE(test_uv) / StaticShader::attr_stride[StaticShader::id1_uv2f];
        unsigned int vertices_count_from_normal = ARRAY_SIZE(dummy_normal) / StaticShader::attr_stride[StaticShader::id2_normal3f];

        unsigned int vertices_count = \
            (vertices_count_from_pos == vertices_count_from_uv && \
             vertices_count_from_pos == vertices_count_from_normal) ? (vertices_count_from_pos) : (0);
        unsigned int indices_count = (vertices_count) ? ( ARRAY_SIZE(test_indices) ) : (0);

        printf("  vertices counts is: %d\n", vertices_count);
        printf("  indices counts are %d\n", indices_count);
        
        model = loader.loadStaticModel(
            &test_vertex_attr, vertices_count, 
            test_indices, indices_count
        );

        if (!model) {
            printf("  Failed to generate model for multi_vbo_entity\n");
        }

        printf("  3D model for multi_vbo_entity generated,");
        printf("    vbos/ibo-ids:  ");
        
        int ids[StaticModel::allBuffNum];
        model->getBuffers(&ids);

        for (int i = 0; i < StaticModel::allBuffNum; i++) {
            printf("%d", ids[i]);
        }
        printf("\n\n");
    }

    StaticTexture *texture = NULL; {
        std::string texture_file = "data/tex/font.png";

        loader.loadStaticTextures(&texture_file, 1, &texture);
        if (!texture) {
            printf("  Failed to generate texture for multi_vbo_entity\n");
            // return false;
        }

        printf("  Texture for multi_vbo_entity generated,");
        printf("    tex-id: %d\n\n", texture->getId());
    }

    if (texture && model) {

        multi_vbo_entity.setTextureModel(texture, model);

        for (unsigned int i = 0; i < 10; i++) {
            float trans_values[Entity::transform::max] = {
                (float)i * 1.0f + 0.5f, (float)i * 1.0f + 0.1f, (float)i * 1.0f + 0.1f,
                0.0f, 0.0f, 0.0f,
                (float)i * 0.3f + 0.3f
            };

            Transform transform(&trans_values);
            multi_vbo_entity.addTransform(transform);
        }
    }
}

void LoadTargets::initMisa() {

    float values[Entity::transform::max] = {
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                1.0f
    };
    Transform transform(&values);
    Transform *transforms = &transform;

    // TODO: improve meshes' hierachy
    misa.loadModel("data/models/misa/misa_yForward_zUp.obj", loader, &transforms, 1);
}
