#pragma once

// #include "Core/Common/data.h"
#include "Core/Loader.h"

class TerrainTexture {
public:
    void init(int input_tex_id) {
        texID = input_tex_id;
    }

    int getTextureID() {
        return texID;
    }
private:
    int texID = -1;
};

class TerrainTexturePack {
public:
    void init(const TerrainTexture &input_tex00, 
        const TerrainTexture &input_tex01, 
        const TerrainTexture &input_tex02, 
        const TerrainTexture &input_tex03) {

        tex00 = input_tex00;
        tex01 = input_tex01;
        tex02 = input_tex02;
        tex03 = input_tex03;
    }

    TerrainTexture *getTerrainTexture00() {
        return &tex00;
    }

    TerrainTexture *getTerrainTexture01() {
        return &tex01;
    }

    TerrainTexture *getTerrainTexture02() {
        return &tex02;
    }

    TerrainTexture *getTerrainTexture03() {
        return &tex03;
    }

private:
    TerrainTexture tex00;
    TerrainTexture tex01;
    TerrainTexture tex02;
    TerrainTexture tex03;
};

class Terrain {

    static const unsigned int MAX_HEIGHT_MAP_PIXEL_COLOR = 255 * 255 * 255;

    static const unsigned int EDGE_VERTEX_COUNT = 128;  // only 128 * 128 pixels on height map can be sampled now
    static const unsigned int TOTAL_VERTEX_COUNT = EDGE_VERTEX_COUNT * EDGE_VERTEX_COUNT;
    
    static const unsigned int SIZE = 20;
    static const unsigned int MAX_HEIGHT = SIZE * 2 / 3;
    // static const unsigned int MAX_HEIGHT = SIZE * 4 / 5;
    // static const unsigned int MAX_HEIGHT = SIZE * 5 / 4;

    float height_calibration = 0.0f;

    int height_map_width;
    int height_map_height;
    int height_map_comp;

    StaticTexture *texture = NULL;
    StaticModel *model = NULL;
    
    int x_offset, y_offset;

    TerrainTexturePack texturePack;
    TerrainTexture blendMap;

    StaticModel *generateTerrainModel(Loader* loader, const std::string &height_map_path);

    void calculateNormal(unsigned int x, unsigned int y, unsigned char *height_map, float *result);
    float getHeight(unsigned int x, unsigned int y, unsigned char* height_map, int intput_comp);

public:
    // StaticTexture *getTexture() {
    //     return texture;
    // }

    TerrainTexturePack *getTexturePack() {
        return &texturePack;
    }

    TerrainTexture *getBlendMap() {
        return &blendMap;
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

    // void init(const TerrainTexturePack &input_texture_pack, const TerrainTexture &input_blend_map) {
    //     texturePack = input_texture_pack;
    //     blendMap = input_blend_map;
    // }

    // Terrain(Loader *loader, int input_x, int input_y, StaticTexture *input_texture, const string &heightmap) {
    //     x_offset = input_x * SIZE;
    //     y_offset = input_y * SIZE;
    //     texture = input_texture;
    //     model = generateTerrainModel(loader, heightmap);
    // }

    void init(Loader *loader, int input_x, int input_y, 
        const TerrainTexturePack &input_texturePack, 
        const TerrainTexture &input_blendMap, 
        const string &heightmap) {

        x_offset = input_x * SIZE;
        y_offset = input_y * SIZE;

        texturePack = input_texturePack;
        blendMap = input_blendMap;    

        model = generateTerrainModel(loader, heightmap);
    }
};
