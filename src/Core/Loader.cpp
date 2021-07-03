#include "Loader.h"
#include "Common/gl_header.h"

#include <stdlib.h>  // for NULL to be used

    extern void destroyTex(unsigned int num, unsigned int* texIds);

    static void deleteVertBuffs(unsigned int num, unsigned int *vbo) {
        if (num) {
            glDeleteBuffers(num, vbo);
        }
    }

    static void genVertBuffs(unsigned int num, unsigned int *vbo) {
        if (num) {
            glGenBuffers(num, vbo);
        }
    }

    static int vboData(const void *data, unsigned int size_in_bytes, unsigned int vbo) {
        if (!vbo) {
            printf("vboData(): vbo is is 0, means this buffer does not exist\n");
            return 0;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, size_in_bytes, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return 1;
    }

    static int iboData(const void *data, unsigned int size_in_bytes, unsigned int ibo) {
        if (!ibo) {
            printf("iboData(): vbo is is 0, means this buffer does not exist\n");
            return 0;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_in_bytes, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return 1;
    }

static inline void createVbos(
          float *input_attr_data[][StaticModel::vboNum],
    unsigned int vertexCount, 
             int output_buffer_vbos_ibo_ids[][StaticModel::allBuffNum]) {  // input is array[allBuffNum] integers, but only [allBuffNum - 1] of them are used,
                                                                           // the last 1 is for ibo, it's written by createIbo()
    genVertBuffs(StaticModel::vboNum, (unsigned int *)(*output_buffer_vbos_ibo_ids));

    for (int i = 0; i < StaticModel::vboNum; i++) {
        vboData( (*input_attr_data)[i], sizeof(float) * StaticModel::vbo_stride[i] * vertexCount, (*output_buffer_vbos_ibo_ids)[i] );
    }
}

static inline void createIbo(unsigned short *indices_data, unsigned int indicesCount, int *ibo) {
    if (!indices_data || !ibo) {
        return;
    }

    genVertBuffs(1, (unsigned int *)ibo);
    iboData(indices_data, sizeof(unsigned short) * indicesCount, *ibo);
}



// Load Model with a single vbo
// ============================

void Loader::creatSingleVboModelBuffers(float *input_attr_data, unsigned int vertex_count, unsigned short *input_indices_data, unsigned int indices_count, int *output_buffer_vbo_ibo_ids) {
    if (!input_attr_data || !input_indices_data || !output_buffer_vbo_ibo_ids) {
        return;
    }
    genVertBuffs(2, (unsigned int *)output_buffer_vbo_ibo_ids);

    vboData(input_attr_data, sizeof(float) * StaticModel_SingleVbo::single_vbo_stride_in_float * vertex_count, output_buffer_vbo_ibo_ids[0] );
    iboData(input_indices_data, sizeof(unsigned short) * indices_count, output_buffer_vbo_ibo_ids[1]);
}

StaticModel_SingleVbo *Loader::allocateSingleVboModelFromBuffers(int *input_vbo_ibo_ids, unsigned int indices_count) {
    if (!input_vbo_ibo_ids) {
        return NULL;
    }

    bool buff_init_success = false;
    if (input_vbo_ibo_ids[0] > 0 && input_vbo_ibo_ids[1] > 0) {
        buff_init_success = true;
    }

    if ( buff_init_success ) {
        StaticModel_SingleVbo *p = new StaticModel_SingleVbo(input_vbo_ibo_ids[0], input_vbo_ibo_ids[1], indices_count);
        if (!p) {
            deleteVertBuffs(2, (unsigned int *)input_vbo_ibo_ids);
            return NULL;
        }
            // Debugging info
            printf("  \'%d\' buffers allocated for a single-vbo model, ids:\n", 2);
            for (int i = 0; i < 2; i++) {
                printf("    %d, ", input_vbo_ibo_ids[i]);
            }
            printf("\n\n");

        pSingleVboModel.push_back(p);
        return p;
    }
    else {
        deleteVertBuffs(2, (unsigned int *)input_vbo_ibo_ids);
        return NULL;
    }
}

StaticModel_SingleVbo *Loader::loadSingleVboModel(
        float *input_attr_data, unsigned int vertex_count, unsigned short *input_indices_data, unsigned int indices_count
    ) {

    int buff_ids[2];
    fill_n(buff_ids, 2, -1);

    creatSingleVboModelBuffers(input_attr_data, vertex_count, input_indices_data, indices_count, buff_ids);
    return allocateSingleVboModelFromBuffers(buff_ids, indices_count);
}

// Load Model with muti vbos
// =========================
// 'vbos' as output for the ids of buffers generated for the target StaticModel
void Loader::createStaticModelBuffers(
    float *input_attr_data[][StaticModel::vboNum], unsigned int vertex_count,
    unsigned short *input_indices_data, unsigned int indices_count,
    int output_buffer_vbos_ibo_ids[][StaticModel::allBuffNum]) {

    createVbos(input_attr_data, vertex_count, output_buffer_vbos_ibo_ids);
    createIbo(input_indices_data, indices_count, (*output_buffer_vbos_ibo_ids) + StaticModel::vboNum);
}

// 'vbos' as input for the StaticModel to be allocated now
StaticModel *Loader::allocStaticModelFromBuffers(int input_vbos_ibo_ids[][StaticModel::allBuffNum], unsigned int indices_count) {
    if (!input_vbos_ibo_ids) {
        return NULL;
    }

    bool buff_init_success = false;
    for (int i = 0; i < StaticModel::allBuffNum; i++) {
        
        if ((*input_vbos_ibo_ids)[i] <= 0) {
            break;
        }

        if (i == StaticModel::allBuffNum - 1) {
            buff_init_success = true;
        }
    }

    if ( buff_init_success ) {
        StaticModel *p = new StaticModel(input_vbos_ibo_ids, indices_count);
        if (!p) {
            deleteVertBuffs(StaticModel::allBuffNum, (unsigned int *)(*input_vbos_ibo_ids));
            return NULL;
        }
            // Debugging info
            printf("  \'%d\' buffers allocated for a static model, ids:\n", StaticModel::allBuffNum);
            for (int i = 0; i < StaticModel::allBuffNum; i++) {
                printf("    %d, ", (*input_vbos_ibo_ids)[i]);
            }
            printf("\n\n");

        pStaticModels.push_back(p);
        return p;
    }
    else {
        deleteVertBuffs(StaticModel::allBuffNum, (unsigned int*)(*input_vbos_ibo_ids));
        return NULL;
    }
}

// each Vertex: N sets of float[] (3/2/3... floats each set) for N attributes(attr-0 pos, 1 uv, 2 normal, .....)
//              1 int for index
//
// Vertices in a model/mesh:
//              N sets of long float[] (3 * N / 2 *N / 3 * N ... floats each set) for N attributes(attr-0 pos, 1 uv, 2 normal, .....)
//              1 int[] for indices (sequences of vertices)
//      3 vbos + 1 ibo
//
StaticModel *Loader::loadStaticModel(
    float *input_attr_data[][StaticModel::vboNum], unsigned int vertex_count,
    unsigned short *input_indices_data, unsigned int indices_count) {
    
    int buff_ids[StaticModel::allBuffNum];
    fill_n(buff_ids, StaticModel::allBuffNum, -1);

    createStaticModelBuffers(input_attr_data, vertex_count, input_indices_data, indices_count, &buff_ids);
    return allocStaticModelFromBuffers(&buff_ids, indices_count);
}

SingleAttributeModel *Loader::allocSingleAttributeModel(float *data, unsigned int vertices_stride, unsigned int vertices_count) {

    if (!data) {
        return NULL;
    }

    SingleAttributeModel *p = new SingleAttributeModel;
    if (!p) {
        return NULL;
    }

    unsigned int vboID; {
        genVertBuffs(1, &vboID);
        vboData(data, sizeof(float) * vertices_stride * vertices_count, vboID);
    }

    p->setData(vertices_stride, vertices_count, vboID);
    pSingleAttributeModels.push_back(p);
        printf("vertices count for this singleAttributeModel: %d.\n", p->getVerticesCount());
        printf("vboID for this singleAttributeModel: %d.\n", p->getVboID());

    return p;
}

void Loader::cleanUp() {
    for (unsigned int i = 0; i < pSingleVboModel.size(); i++) {
        // free buffers inside model
        printf("  \'%d\' buffers will be free-ed for a single-vbo model, ids:\n", 2);

        int buffers[2];
        buffers[0] = pSingleVboModel[i]->getVboId();
        buffers[1] = pSingleVboModel[i]->getIboId();
        
        printf("  vbo: \'%d\'\n", buffers[0]);
        printf("  ibo: \'%d\'\n", buffers[1]);
        deleteVertBuffs(2, (unsigned int*)buffers);

        // free the model class' object
        // could delete both base-class ptr or subclass-ptr in the future ???];
        delete pSingleVboModel[i];
    }
    pSingleVboModel.clear();

    // delete allocated vao/vbos in the model,
    // and delete the allocated model object.
    for (unsigned int i = 0; i < pStaticModels.size(); i++) {
        // free buffers inside model
        printf("  \'%d\' buffers will be free-ed for a static model, ids:\n", StaticModel::allBuffNum);

        int buffers[StaticModel::allBuffNum];
        pStaticModels[i]->getBuffers(&buffers);

        printf("  vbos:\n");
        for (int i = 0; i < StaticModel::vboNum; i++) {
            printf("    %d, ", buffers[i]);
        }   printf("\n\n");
        printf("  ibo: \'%d\'\n", buffers[StaticModel::vboNum]);

        deleteVertBuffs(StaticModel::allBuffNum, (unsigned int*)buffers);

        // free the model class' object
        // could delete both base-class ptr or subclass-ptr in the future ???];
        delete pStaticModels[i];
    }
    pStaticModels.clear();

    for (unsigned int i = 0; i < pSingleAttributeModels.size(); i++) {

        int vboID = pSingleAttributeModels[i]->getVboID();

        if (vboID >= 0) {
            // free buffers inside model
            printf("  vbo-%d will be free-ed for a RawModel\n", vboID);
            deleteVertBuffs(1, (unsigned int *)&vboID);
        }

        // free the model class' object
        delete pSingleAttributeModels[i];
    }
    pSingleAttributeModels.clear();

    for (unsigned int i = 0; i < pStaticTextures.size(); i++) {
        
        int target_tex_id = pStaticTextures[i]->getId();
        destroyTex(1, (unsigned int *)(&target_tex_id));

        printf("  tex-id: %d delted\n", target_tex_id);
        delete pStaticTextures[i];
    }
    pStaticTextures.clear();
}
