#include "Terrain.h"
#include "Core/Common/gl_math.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // not implementation, implementation is in loader_tex.cpp

// MAKE SURE THAT YOUR CAMERA IS IN A POSITION THAT ALLOWS YOU TO SEE THE TERRAIN! MAKE SURE THE CAMERA'S Y
// POSITION IS ABOVE 0, AND MAKE SURE THAT THE TERRAIN IS INFRONT OF THE CAMERA (EITHER MOVE THE
// CAMERA BACK, ROTATE THE CAMERA AROUND, OR CHOOSE NEGATIVE GRIDX & GRIDZ VALUES WHEN CALLING THE TERRAIN
// CONSTRUCTOR).

// COPY AND PASTE INTO TERRAIN CLASS:

float Terrain::getHeight(unsigned int x, unsigned int y, unsigned char *height_map, int input_comp) {
    // printf("p input: 0x%x\n", height_map);

    if (x >= height_map_width || y >= height_map_height) {
        return 0;
    }

    unsigned int offset = x + (y * height_map_width);
    // printf("height_map_width: %d, offset: %d\n", height_map_width, offset);
    // unsigned int offset = x + (y * EDGE_VERTEX_COUNT);
    
    unsigned char *p = NULL; {
        // if (input_comp == 4) {
        //     p = height_map + offset * 4;
        // }
        // else if (input_comp == 3) {
        //     p = height_map + offset * 3;
        // }

        // p = height_map + offset;
        // printf("p: 0x%p\n", p);
        // p = height_map;
    }

    float height = 0.0f;

    // if (p) {
    //     // printf("aaaaaaa\n");
    //     // height = (p[0]) * (p[1]) * (p[2]);
    //     height = \
    //         height_map[offset] * \
    //         height_map[offset + 1] * \
    //         height_map[offset + 2];
    //     printf("rgb: %d, %d, %d   ", height_map[offset], height_map[offset + 1], height_map[offset + 2]);

    //     height -= MAX_HEIGHT_MAP_PIXEL_COLOR / 2.0f;
    //     // height += MAX_HEIGHT_MAP_PIXEL_COLOR / 2.0f;
    //     height /= MAX_HEIGHT_MAP_PIXEL_COLOR / 2.0f;
    //     height *= MAX_HEIGHT;
    // }

    // Even though the height_map_32_mod.png's format turns out tobe rgb, rgb, rgb ...
    // the comp for this image interpreted from stbi is 4 rather than 3 !!! ???
    // so offset * 3 has to be hardcoded (force 3, not 4) for correctness
    //
    // The original height_map_32.png's comp is 3 thought ...
    //
    if (height_map) {
        // printf("aaaaaaa\n");
        // height = (p[0]) * (p[1]) * (p[2]);
        height = \
            height_map[offset * 3 + 0] * \
            height_map[offset * 3 + 1] * \
            height_map[offset * 3 + 2];

        // printf("offset-%d ", offset);
        // printf("rgb: %d, %d, %d   ", \
        //     height_map[offset * 3 + 0], 
        //     height_map[offset * 3 + 1],
        //     height_map[offset * 3 + 2]);

        height -= MAX_HEIGHT_MAP_PIXEL_COLOR / 2.0f;
        // height += MAX_HEIGHT_MAP_PIXEL_COLOR / 2.0f;
        height /= MAX_HEIGHT_MAP_PIXEL_COLOR / 2.0f;
        height *= MAX_HEIGHT;

        // The current height-map picture is too dark, making the height lower than 0,
        // compensate the height by increasing very vertex's height.
        // height += 8.0f;
    }

    if (!height_calibration && x == 0 && y == 0) {
        height_calibration = 0.0f - height;
    }

    return height + height_calibration;
}

void Terrain::calculateNormal(unsigned int x, unsigned int y, unsigned char *height_map, float *result) {
// Vector3f calculateNormal(int x, int z, BufferedImage image){
    float heightL = (x == 0) ? (0) : ( getHeight(x - 1, y, height_map, 3) );
    float heightR = getHeight(x + 1, y, height_map, 3);
    float heightD = (y == 0) ? (0) : ( getHeight(x, y - 1, height_map, 3) );
    float heightU = getHeight(x, y + 1, height_map, 3);

    gl_math::vec3 temp_result; {
        temp_result[0] = heightL - heightR;
        temp_result[1] = heightD - heightU;
        temp_result[2] = 2.0f;
        
        temp_result = gl_math::normalize_vec3(temp_result);
    }

    if (result) {
        (result)[0] = temp_result[0];
        (result)[1] = temp_result[1];
        (result)[2] = temp_result[2];
        // (result)[0] = 0.0f;
        // (result)[1] = 0.0f;
        // (result)[2] = 1.0f;
    }
}

StaticModel *Terrain::generateTerrainModel(Loader *loader, const std::string &height_map_path) {

    // load the height and get info from the height map
    unsigned char *height_map_data = NULL;
    // unsigned int EDGE_VERTEX_COUNT = 0; 
    
    bool height_map_loaded = false; {
        // stbi_set_flip_vertically_on_load(1);

        printf("\n\n  __loading height map\n");
        height_map_data = stbi_load(
            height_map_path.c_str(), 
            &height_map_width, &height_map_height, &height_map_comp, 
            STBI_rgb
        );

        if (!height_map_data) {
            printf("terrain height_map: failed to read file: %s!\n\n", height_map_path.c_str());
        }
        else if (height_map_width <= 0 || height_map_height <= 0) {
            printf("terrain height_map: width or height is 0!\n\n");
        }
        else {
            printf("  __loading height success, comp is %d\n", height_map_comp);
            height_map_loaded = true;
        }

        // EDGE_VERTEX_COUNT is the same as the width of height-map on successful loading,
        // and is 16 if failed to load height-map
        // EDGE_VERTEX_COUNT = (height_map_loaded) ? (height_map_width) : (16);
        
        if (height_map_loaded) {
            // for (unsigned int i = 0; i < 32 * 32; i++) {
            //     // printf("    i: %d", i);
            //     // unsigned char result = height_map_data[i];
            //     printf("i-%3d ", i);
            //     printf("r: %3d ", height_map_data[i * 3 + 0]);
            //     printf("g: %3d ", height_map_data[i * 3 + 1]);
            //     printf("b: %3d ", height_map_data[i * 3 + 2]);
            //     printf("\n");
            // }

            printf("  height map w, h, comp: %d, %d, %d\n", 
                height_map_width, height_map_height, height_map_comp);

            // printf("  __ 1023 * 3 + 2\n");
            // unsigned char result = height_map_data[(31 + 31 * 32) * 3 + 2];

            // printf("press anything to continue ...\n\n"); {
            //     int dbg;
            //     scanf("%d", &dbg);
            // }

            // printf("\n\n\n\n");
            // printf("height_map: 0x%x\n", height_map_data);
        }
    }
    // printf("height_map: 0x%x\n", height_map_data);

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
            // printf("  __i: %d\n", i);
            for (int j = 0; j < EDGE_VERTEX_COUNT; j++) {

                textureUVs[offset * 2 + 0] = (float)j / ((float)EDGE_VERTEX_COUNT - 1);
                textureUVs[offset * 2 + 1] = (float)i / ((float)EDGE_VERTEX_COUNT - 1);

                // Normal effect not working ???
                // error in shader ???
                // in multi-texture ???
                // tobe debugged ...
                //
                // normals[offset * 3 + 0] = 0;
                // normals[offset * 3 + 1] = 0;
                // normals[offset * 3 + 2] = 1;
                calculateNormal(j, i, height_map_data, normals + offset * 3);

                // printf("(%d, %d) ", j, i);
                positions[offset * 3 + 0] = (float)j / ((float)EDGE_VERTEX_COUNT - 1) * SIZE;
                positions[offset * 3 + 1] = (float)i / ((float)EDGE_VERTEX_COUNT - 1) * SIZE;
                // positions[offset * 3 + 2] = 0;  // hight is 0
                // if (height_map) {
                //     positions[offset * 3 + 2] = getHeight(j, i, height_map_data, height_map_comp);
                // }
                // else {
                //     positions[offset * 3 + 2] = 0.0f;
                // }

                positions[offset * 3 + 2] = (height_map_loaded) ? \
                    ( getHeight(j, i, height_map_data, 3) ) : \
                    (0);
                // positions[offset * 3 + 2] = 0.0f;
                    // ( getHeight(j, i, height_map_data, height_map_comp) ) : \

                // printf("(%d, %d)b \n", j, i);

                offset++;
            }
            // printf("\n\n");
        }
    }

    // height-map data can be free-ed after the above step
    stbi_image_free(height_map_data);

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
