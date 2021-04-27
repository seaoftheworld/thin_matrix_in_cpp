#include "LightsPositionsUpdate.h"

float LightsPositionsUpdate::initPosition[NUM_LIGHTS][Light::Position::max_pos] = {
    { 0.0f,  2.0f, 1.8f},
    {-2.0f,  0.0f, 1.8f},
    { 0.0f, -2.0f, 1.8f},
    { 2.0f,  0.0f, 1.8f}

    // { 0.0f + 2.0f,  2.0f + 2.0f, 1.8f},
    // {-2.0f + 2.0f,  0.0f + 2.0f, 1.8f},
    // { 0.0f + 2.0f, -2.0f + 2.0f, 1.8f},
    // { 2.0f + 2.0f,  0.0f + 2.0f, 1.8f}
};

LightsPositionsUpdate::LightsPositionsUpdate() {
    move_type = LightsPositionsUpdate::rotate;
    curr_rotz = 0.0f;

    for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
        for (unsigned int j = 0; j < Light::Position::max_pos; j++) {
            position[i][j] = initPosition[i][j];
        }
    }
}

void LightsPositionsUpdate::run() {
    float rot_z_step = 0.032f;
    float xy_step = 0.048f;
    
    // float rot_z_step = 0.016f;
    // float xy_step = 0.024f;

    // float offset = 3.14f / 5.0f + 3.14f / 6.0f;
    float offset = 3.14f / 4.0f;

    switch (move_type) {
        case rotate:
        {
            if (curr_rotz > 6.28f) {
                curr_rotz = 0.0f;
            }
            curr_rotz += rot_z_step;

            if (curr_rotz > 3.14f / 4.0f + offset && curr_rotz < 3.14f / 4.0f + offset + rot_z_step) {
                move_type = crate0_get_closer;
            }
            if (curr_rotz > 3.14f / 2.0f + 3.14f / 4.0f + offset && curr_rotz < 3.14f / 2.0f + 3.14f / 4.0f + offset + rot_z_step) {
                move_type = crate3_get_closer;
            }
            if (curr_rotz > 3.14f + 3.14f / 4.0f + offset && curr_rotz < 3.14f + 3.14f / 4.0f + offset + rot_z_step) {
                move_type = crate2_get_closer;
            }
            if (curr_rotz > 3.14f + 3.14f / 2.0f + 3.14f / 4.0f + offset && curr_rotz < 3.14f + 3.14f / 2.0f + 3.14f / 4.0f + offset + rot_z_step) {
                move_type = crate1_get_closer;
            }
        }
        break;

        case crate0_get_further:
        case crate0_get_closer:
        {
            // TODO: make these into a function
            unsigned short transform_idx = 0, 
                           value_idx = Entity::transform::y;

            float step = (move_type == crate0_get_closer) ? (-xy_step) : (xy_step);

            // for (unsigned int i = 0; i < crate->tranformsNum(); i++) {
            //     crate->increasePosition(i, 0.0f, step, 0.0f);
            // }
            for (unsigned int i = 0; i < NUM_LIGHTS; i++) {
                position[i][value_idx] += step;
            }
            // pos_value = (*(crate->getTransformValues(transform_idx)))[value_idx];
            float pos_value = position[transform_idx][value_idx];

            if (move_type == crate0_get_closer) {
                if ( pos_value < 1.0f ) {
                    // stop = true;
                    move_type = crate0_get_further;
                }
            }
            else {
                if ( pos_value > 2.0f ) {
                    // stop = true;
                    move_type = rotate;
                }
            }
        }
        break;

        case crate3_get_further:
        case crate3_get_closer:
        {
            unsigned short transform_idx = 3, 
                           value_idx = Entity::transform::x;

            float step = (move_type == crate3_get_closer) ? (-xy_step) : (xy_step);
            // crate->increasePosition(transform_idx, step, 0.0f, 0.0f);
            position[transform_idx][value_idx] += step;

            // pos_value = (*(crate->getTransformValues(transform_idx)))[value_idx];
            float pos_value = position[transform_idx][value_idx];

            if (move_type == crate3_get_closer) {
                if ( pos_value < 1.0f ) {
                    // stop = true;
                    move_type = crate3_get_further;
                }
            }
            else {
                if ( pos_value > 2.0f ) {
                    // stop = true;
                    move_type = rotate;
                }
            }
        }
        break;

        case crate2_get_further:
        case crate2_get_closer:
        {
            unsigned short transform_idx = 2, 
                           value_idx = Entity::transform::y;

            float step = (move_type == crate2_get_closer) ? (xy_step) : (-xy_step);
            // crate->increasePosition(transform_idx, 0.0, step, 0.0f);
            position[transform_idx][value_idx] += step;

            // pos_value = (*(crate->getTransformValues(transform_idx)))[value_idx];
            float pos_value = position[transform_idx][value_idx];

            if ( move_type == crate2_get_closer ) {
                if ( pos_value > -1.0f ) {
                    // stop = true;
                    move_type = crate2_get_further;
                }
            }
            else {
                if ( pos_value < -2.0f ) {
                    // stop = true;
                    move_type = rotate;
                }
            }
        }
        break;

        case crate1_get_further:
        case crate1_get_closer:
        {
            unsigned short transform_idx = 1, 
                           value_idx = Entity::transform::x;

            float step = (move_type == crate1_get_closer) ? (xy_step) : (-xy_step);
            // crate->increasePosition(transform_idx, step, 0.0, 0.0f);
            position[transform_idx][value_idx] += step;

            float pos_value = position[transform_idx][value_idx];

            if ( move_type == crate1_get_closer ) {
                if ( pos_value > -1.0f ) {
                    // stop = true;
                    move_type = crate1_get_further;
                }
            }
            else {
                if ( pos_value < -2.0f ) {
                    // stop = true;
                    move_type = rotate;
                }
            }
        }
        break;

        default:
        break;
    }
}
