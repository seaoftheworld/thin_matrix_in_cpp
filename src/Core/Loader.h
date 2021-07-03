#pragma once
#include "Common/data.h"

#include <vector>
#include <string>

using namespace std;

// generate and manage buffers for a model/mesh,
// like vbos/vao for attributes in the shader (and ibo),
//      texture-buffer for the texture to be bound to before rendering...
// the model-object with those buffers' ids will be consumed by a renderer-obj, 
// alone with the shader-obj also as render's input parameter.
// Renderer also calcualtes matrices to feed the shader according to entity info.
class Loader {
public:
    Loader() {
        pSingleVboModel.clear();
        pStaticModels.clear();
        pSingleAttributeModels.clear();

        pStaticTextures.clear();
    }

    // delete allocated vao/vbos in the model,
    // and delete the allocated model object.
    void cleanUp();

    void loadStaticTextures(string *imgPaths, unsigned int num, StaticTexture **output_result);

    StaticTexture *loadStaticTextureCube(string imgPaths[][6]);

    StaticModel_SingleVbo *loadSingleVboModel(
        float *input_attr_data, unsigned int vertex_count, unsigned short *input_indices_data, unsigned int indices_count
    );
    // StaticModel is multi-vbo model
    StaticModel *loadStaticModel(
        float *input_attr_data[][StaticModel::vboNum], unsigned int vertex_count, 
        unsigned short *input_indices_data, unsigned int indices_count);

    // allocSingleAttributeModel()
    SingleAttributeModel *allocSingleAttributeModel(float *data, unsigned int vertices_stride, unsigned int vertices_count);

private:
    vector<StaticModel_SingleVbo *> pSingleVboModel;
    vector<StaticModel *> pStaticModels;

    // vector<RawModel *> pRawModels;
    vector<SingleAttributeModel *> pSingleAttributeModels;

    vector<StaticTexture *> pStaticTextures;

    unsigned int createStaticTextureBuffers(std::string *imgPaths, unsigned int num, unsigned int output_buff_texture_ids[][MAX_TEXTURE_NUM_ONCE]);
    void allocStaticTextureFromBuffers(unsigned int *input_textureIds, unsigned int num, StaticTexture **output_result);

    void creatSingleVboModelBuffers(
        float *input_attr_data, unsigned int vertex_count, unsigned short *input_indices_data, unsigned int indices_count, int *output_buffer_vbo_ibo_ids
    );
    StaticModel_SingleVbo *allocateSingleVboModelFromBuffers(int *input_vbo_ibo_ids, unsigned int indices_count);

    void createStaticModelBuffers(
        float *input_attr_data[][StaticModel::vboNum], unsigned int vertex_count,
        unsigned short *input_indices_data, unsigned int indices_count, 
        int output_buffer_vbos_ibo_ids[][StaticModel::allBuffNum]);

    StaticModel *allocStaticModelFromBuffers(int input_vbos_ibo_ids[][StaticModel::allBuffNum], unsigned int indices_count);
};
