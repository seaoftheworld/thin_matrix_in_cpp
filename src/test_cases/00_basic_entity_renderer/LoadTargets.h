#include "Core/AssimpLib.h"
#include "Core/Loader.h"

// #include "Core/Renderers/Terrain/Terrain.h"
// #include "Core/Renderers/Skybox/SkyboxRenderer.h"  // shall seperate skybox-data with skybox-renderer ???
// #include "WrappingRenderer.h"

class LoadTargets {
    Loader loader;

    Entity single_vbo_entity;
    Entity multi_vbo_entity;
    AssimpLib misa;

    void cleanUp() {
        single_vbo_entity.cleanUp();
        multi_vbo_entity.cleanUp();

        for (auto misa_entity = misa.entities.begin(); misa_entity != misa.entities.end(); misa_entity++) {
            misa_entity->cleanUp();
        }
        misa.cleanUp();

        loader.cleanUp();
    }

    void initSingleVboEntity();
    void initMultiVboEntity();
    void initMisa();

public:
    LoadTargets() {
        cleanUp();

        // init entities
        initSingleVboEntity();
        initMultiVboEntity();
        initMisa();
        printf("\n\n models/entities init done, press anything to continue ...\n\n"); {
            int dbg;
            scanf("%d", &dbg);
        }

        // init terrain
        // init skybox
        // init gui
    }
    virtual ~LoadTargets() {
        cleanUp();
    }

    Entity *getSingleVboEntity() {
        return &single_vbo_entity;
    }
    Entity *getMultiVboEntity() {
        return &multi_vbo_entity;
    }
    AssimpLib *getMisa() {
        return &misa;
    }
};
