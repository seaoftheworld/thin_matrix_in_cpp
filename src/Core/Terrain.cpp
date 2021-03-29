#include "Terrain.h"

// MAKE SURE THAT YOUR CAMERA IS IN A POSITION THAT ALLOWS YOU TO SEE THE TERRAIN! MAKE SURE THE CAMERA'S Y
// POSITION IS ABOVE 0, AND MAKE SURE THAT THE TERRAIN IS INFRONT OF THE CAMERA (EITHER MOVE THE
// CAMERA BACK, ROTATE THE CAMERA AROUND, OR CHOOSE NEGATIVE GRIDX & GRIDZ VALUES WHEN CALLING THE TERRAIN
// CONSTRUCTOR).

// COPY AND PASTE INTO TERRAIN CLASS:

StaticModel* Terrain::generateTerrainModel(Loader* loader) {

    float  positions[TOTAL_VERTEX_COUNT * 3];
    float textureUVs[TOTAL_VERTEX_COUNT * 2];
    float    normals[TOTAL_VERTEX_COUNT * 3];

    unsigned short indices[6 * (EDGE_VERTEX_COUNT - 1) * (EDGE_VERTEX_COUNT - 1)];

    unsigned int indices_count = 6 * (EDGE_VERTEX_COUNT - 1) * (EDGE_VERTEX_COUNT - 1),
                 vertex_count = TOTAL_VERTEX_COUNT;

    // Assign values to the 3 attributes array:
    //     pos[TOTAL_VERTEX_COUNT * 3]
    //      uv[TOTAL_VERTEX_COUNT * 2]
    // normals[TOTAL_VERTEX_COUNT * 3]
    {
        int offset = 0;
        for (int i = 0; i < EDGE_VERTEX_COUNT; i++) {
            for (int j = 0; j < EDGE_VERTEX_COUNT; j++) {

                positions[offset * 3 + 0] = (float)j / ((float)EDGE_VERTEX_COUNT - 1) * SIZE;
                positions[offset * 3 + 1] = (float)i / ((float)EDGE_VERTEX_COUNT - 1) * SIZE;
                positions[offset * 3 + 2] = 0;  // hight is 0

                textureUVs[offset * 2 + 0] = (float)j / ((float)EDGE_VERTEX_COUNT - 1);
                textureUVs[offset * 2 + 1] = (float)i / ((float)EDGE_VERTEX_COUNT - 1);

                normals[offset * 3 + 0] = 0;
                normals[offset * 3 + 1] = 0;
                normals[offset * 3 + 2] = 1;

                offset++;
            }
        }
    }

    // Assign values to the indices array [6 * (edge_vcount - 1) * (edge_vcount - 1)]
    {
        int offset = 0;
        for (int gy = 0; gy < EDGE_VERTEX_COUNT - 1; gy++) {
            for (int gx = 0; gx < EDGE_VERTEX_COUNT - 1; gx++) {

                int topLeft = (gy * EDGE_VERTEX_COUNT) + gx;
                int topRight = topLeft + 1;
                int bottomLeft = ((gy + 1) * EDGE_VERTEX_COUNT) + gx;
                int bottomRight = bottomLeft + 1;

                indices[offset * 6 + 0] = topLeft;
                indices[offset * 6 + 1] = bottomLeft;
                indices[offset * 6 + 2] = topRight;
                indices[offset * 6 + 3] = topRight;
                indices[offset * 6 + 4] = bottomLeft;
                indices[offset * 6 + 5] = bottomRight;

                offset++;
            }
        }
    }

    // For multi-vbo model
    float* attributes[] = {
        positions,
        textureUVs,
        normals
    };
    return loader->loadStaticModel(&attributes, vertex_count, indices, indices_count);
};
