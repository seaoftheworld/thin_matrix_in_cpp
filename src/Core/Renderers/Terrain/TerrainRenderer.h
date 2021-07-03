#include "Core/Renderer.h"
#include "TerrainShader.h"
#include "Terrain.h"

class TerrainRenderer : public BaseRenderer {
private:
    TerrainShader *terrainShader = NULL;
    std::vector<Terrain *> terrains;

public:
    void allocShadersData() override;
    void freeShadersData() override;
    bool ready() override;

    void addTerrain(Terrain *terrain) {
        terrains.push_back(terrain);
    }

public:
    TerrainRenderer() {
        printf("TerrainRenderer() constructor called.\n\n\n");
        freeShadersData();
        allocShadersData();
    }
    virtual ~TerrainRenderer() {
        printf("TerrainRenderer() destructor called.\n\n\n");
        freeShadersData();
    }

    void run(std::vector<Light *> lights, float clipPlane[][4]) {

        if (!terrainShader) {
            return;
        }

        terrainShader->start();
        terrainShader->loadViewMatrix(getViewMatrix());
        terrainShader->loadLights(lights);

        terrainShader->loadShineDamper(200.0f);
        terrainShader->loadReflectivity(0.1f);

        if (clipPlane) {
            terrainShader->loadClipPlane(clipPlane);
        }

        render();
    }

    void render() {
        for (int i = 0; i < terrains.size(); i++) {
            auto terrain = terrains.begin() + i;
            render((*terrain));
        }
    }

    void render(Terrain *terrain) {
        if (!terrain || !terrainShader) {
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
                    {
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

                        // printf("\n\n\n");
                        // printf(" terrain model info, vboNum: %d\n", StaticModel::vboNum);
                        // printf("  vbos_ibo ids: %d, %d, %d, %d\n", \
                        //     vbos_ibo_ids[0], vbos_ibo_ids[1], vbos_ibo_ids[2], vbos_ibo_ids[3]);

                        // printf("   idx: %d, %d, %d\n", \
                        //     StaticModel::vbo_atrr_idx[0],
                        //     StaticModel::vbo_atrr_idx[1],
                        //     StaticModel::vbo_atrr_idx[2]
                        // );

                        // printf(" strides: %d, %d, %d\n", \
                        //     StaticModel::vbo_stride[0],
                        //     StaticModel::vbo_stride[1],
                        //     StaticModel::vbo_stride[2]
                        // );
                        // printf("\n\n\n");
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
            terrainShader->loadTransformMatrix(&model_matrix[0][0]);
            
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
};
