#pragma once

#include "Core/AssimpLib.h"
#include "Core/Loader.h"


// #include "Core/Renderers/Terrain/Terrain.h"
// #include "Core/Renderers/Skybox/SkyboxRenderer.h"  // TODO: shall seperate skybox-data with skybox-renderer
// #include "Core/Renderers/Gui/Gui.h"                //       like it is for the other renderers
// #include "Core/Renderers/Water/WaterTile.h"

#include "WrappingRenderer.h"  // to include data types for terrain, skybox, gui, and water

#include "test_cases/02_multi_lighting_entity_renderer/LightsPositionsUpdate.h"

class LoadTargets_03 {
    Loader loader;

    Entity single_vbo_entity;
    Entity multi_vbo_entity;

    AssimpLib crate;
    AssimpLib misa;
    AssimpLib rock;

    Terrain terrain;
    Skybox  skybox;
    GuiType00 gui_00;
    GuiType00 gui_01;

    WaterFrameBuffers waterFbos;
    WaterTile waterTile[4];
    unsigned int water_dudvTexture = 0;
    unsigned int water_normalTexture = 0;

    void initData() {
        single_vbo_entity.cleanUp();
        multi_vbo_entity.cleanUp();

        for (auto misa_entity = misa.entities.begin(); misa_entity != misa.entities.end(); misa_entity++) {
            misa_entity->cleanUp();
        }
        misa.cleanUp();

        for (auto crate_entity = crate.entities.begin(); crate_entity != crate.entities.end(); crate_entity++) {
            crate_entity->cleanUp();
        }
        crate.cleanUp();

        for (auto rock_entity = rock.entities.begin(); rock_entity != rock.entities.end(); rock_entity++) {
            rock_entity->cleanUp();
        }
        rock.cleanUp();

        loader.cleanUp();
    }
    void cleanUp() {
        initData();
        waterFbos.cleanUp();  // this shall not be called in constructor
    }

    void initSingleVboEntity();
    void initMultiVboEntity();
    void initCrate();
    void initMisa();
    void initRock();

    void initTerrain();
    void initSkybox();
    void initGui();
    void initWaterTiles();

    void initWaterTextures();

public:
    static float misa_offset_x;
    static float misa_offset_y;

    LoadTargets_03() {
        initData();

        // init entities
        // initSingleVboEntity();
        // initMultiVboEntity();

        initCrate();
        initMisa();
        initRock();

        printf("\n\n models/entities init done, press anything to continue ...\n\n"); {
            int dbg;
            scanf("%d", &dbg);
        }

        // init terrain
        initTerrain();

        // init skybox
        initSkybox();

        // init gui
        // initGui();

        // init water
        initWaterTextures();
        initWaterTiles();
    }
    virtual ~LoadTargets_03() {
        cleanUp();
    }

    Entity *getSingleVboEntity() {
        return &single_vbo_entity;
    }
    Entity *getMultiVboEntity() {
        return &multi_vbo_entity;
    }
    AssimpLib *getCrate() {
        return &crate;
    }
    AssimpLib *getMisa() {
        return &misa;
    }
    AssimpLib *getRock() {
        return &rock;
    }

    Terrain *getTerrain() {
        return &terrain;
    }
    Skybox *getSkybox() {
        return &skybox;
    }

    GuiType00 *getGui00() {
        return &gui_00;
    }
    GuiType00* getGui01() {
        return &gui_01;
    }

    WaterFrameBuffers *getWaterFbos() {
        return &waterFbos;
    }
    WaterTile *getWaterTiles() {
        return waterTile;
    }
    unsigned int getWaterDudvTexture() {
        return water_dudvTexture;
    }
    unsigned int getWaterNormalTexture() {
        return water_normalTexture;
    }
};
