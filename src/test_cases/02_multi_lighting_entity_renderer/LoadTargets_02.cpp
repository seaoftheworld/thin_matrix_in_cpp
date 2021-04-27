#include "LoadTargets_02.h"

void LoadTargets_02::initMisa() {

    float values[Entity::transform::max] = {
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                1.0f
    };
    Transform transform(&values);
    Transform *transforms = &transform;

    // TODO: improve meshes' hierachy
    misa.loadModel("data/models/misa/misa_yForward_zUp.obj", loader, &transforms, 1);
}

void LoadTargets_02::initCrate() {

    // Modify the 4 init-transforms of the crate-entity
    // in the following lines:
    float crate_transform_values[NUM_LIGHTS][Entity::transform::max];

    for (unsigned int i = 0; i < NUM_LIGHTS; i++) {

        const float crate_scale = 0.12f;

        crate_transform_values[i][Entity::transform::x] = LightsPositionsUpdate::initPosition[i][Light::Position::x];
        crate_transform_values[i][Entity::transform::y] = LightsPositionsUpdate::initPosition[i][Light::Position::y];
        crate_transform_values[i][Entity::transform::z] = LightsPositionsUpdate::initPosition[i][Light::Position::z];

        crate_transform_values[i][Entity::transform::rot_x] = 0.0f;
        crate_transform_values[i][Entity::transform::rot_y] = 0.0f;
        crate_transform_values[i][Entity::transform::rot_z] = 0.0f;

        crate_transform_values[i][Entity::transform::scale] = crate_scale;
    }

    Transform crate_transforms[NUM_LIGHTS]; {
        for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
            crate_transforms[i].values[Transform::x] = crate_transform_values[i][Entity::transform::x];
            crate_transforms[i].values[Transform::y] = crate_transform_values[i][Entity::transform::y];
            crate_transforms[i].values[Transform::z] = crate_transform_values[i][Entity::transform::z];
            crate_transforms[i].values[Transform::rot_x] = crate_transform_values[i][Entity::transform::rot_x];
            crate_transforms[i].values[Transform::rot_y] = crate_transform_values[i][Entity::transform::rot_y];
            crate_transforms[i].values[Transform::rot_z] = crate_transform_values[i][Entity::transform::rot_z];
            crate_transforms[i].values[Transform::scale] = crate_transform_values[i][Entity::transform::scale];
        }
    }

    Transform *p_crate_transforms[NUM_LIGHTS]; {
        for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
            p_crate_transforms[i] = &crate_transforms[i];
        }
    };

    crate.loadModel("data/models/crate/Crate1.obj", loader, p_crate_transforms, NUM_LIGHTS);

    // Doesnt work before shader is bound/opened/started .... !!!
    // multiLightsShader.loadLights(&light_pos[0], &light_color[0], 4);

    // printf("4 lights inited and crate model loaded, press anything to continue ...\n\n"); {
    //     int dbg;
    //     scanf("%d", &dbg);
    // }
}
