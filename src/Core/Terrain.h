#include "Core/Common/data.h"
#include "Loader.h"

class Terrain {
    static const unsigned int EDGE_VERTEX_COUNT = 16;
    static const unsigned int SIZE = 10;
    
    static const unsigned int TOTAL_VERTEX_COUNT = EDGE_VERTEX_COUNT * EDGE_VERTEX_COUNT;

    StaticTexture *texture = NULL;
    StaticModel *model = NULL;
    
    int x_offset, y_offset;

    StaticModel *generateTerrainModel(Loader *loader);

public:
    StaticTexture *getTexture() {
        return texture;
    }

    StaticModel *getModel() {
        return model;
    }

    float getXOffset() {
        return (float)x_offset;
    }

    float getYOffset() {
        return (float)y_offset;
    }

    // Terrain(Loader *loader, std::string texture_path, unsigned int input_x, unsigned int input_y) {
    //     x_offset = input_x * SIZE;
    //     y_offset = input_y * SIZE;

    //     loader->loadStaticTextures(&texture_path, 1, &texture);
    //     model = generateTerrainModel(loader);
    // }

    Terrain(Loader *loader, StaticTexture *input_texture, int input_x, int input_y) {
        x_offset = input_x * SIZE;
        y_offset = input_y * SIZE;

        texture = input_texture;
        model = generateTerrainModel(loader);
    }
};
