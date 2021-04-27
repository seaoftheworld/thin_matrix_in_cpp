#pragma once

#include "Core/AssimpLib.h"
#include "Core/Loader.h"


// #include "Core/Renderers/Terrain/Terrain.h"
// #include "Core/Renderers/Skybox/SkyboxRenderer.h"  // TODO: shall seperate skybox-data with skybox-renderer
// #include "Core/Renderers/Gui/Gui.h"                //       like it is for the other renderers

#include "WrappingRenderer.h"  // to include data types for terrain, skybox, and gui

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
    GuiType00 gui;

    void cleanUp() {
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

    void initSingleVboEntity();
    void initMultiVboEntity();
    void initCrate();
    void initMisa();
    void initRock();

    void initTerrain();
    void initSkybox();
    void initGui();

public:
    static float misa_offset_x;
    static float misa_offset_y;

    LoadTargets_03() {
        cleanUp();

        // init entities
        initSingleVboEntity();
        initMultiVboEntity();

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
        initGui();
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
    GuiType00 *getGui() {
        return &gui;
    }
};
