#include "Core/AssimpLib.h"
#include "Core/Loader.h"

#include "LightsPositionsUpdate.h"

class LoadTargets_02 {
    Loader loader;

    AssimpLib crate;
    AssimpLib misa;

    void cleanUp() {

        for (auto misa_entity = misa.entities.begin(); misa_entity != misa.entities.end(); misa_entity++) {
            misa_entity->cleanUp();
        }
        misa.cleanUp();

        for (auto crate_entity = crate.entities.begin(); crate_entity != crate.entities.end(); crate_entity++) {
            crate_entity->cleanUp();
        }
        crate.cleanUp();

        loader.cleanUp();
    }

    void initCrate();
    void initMisa();

public:
    LoadTargets_02() {
        cleanUp();

        // init entities
        initCrate();
        initMisa();
        printf("\n\n models/entities init done, press anything to continue ...\n\n"); {
            int dbg;
            scanf("%d", &dbg);
        }

        // init terrain
        // init skybox
        // init gui
    }
    virtual ~LoadTargets_02() {
        cleanUp();
    }

    AssimpLib *getMisa() {
        return &misa;
    }
    AssimpLib *getCrate() {
        return &crate;
    }
};
