#include "Core/Common/light.h"
#include "Core/Common/data.h"

#define NUM_LIGHTS (4)

struct XYZRotz {
    float pos[3];
    float rot;
};

//  p3f is a pointer to an array of 3-floats
// fp3f is a function returning a pointer to an array of 3-floats
typedef float (*p3f)[Light::Position::max_pos];
typedef p3f *fp3f();

class LightsPositionsUpdate {

public:
    enum MoveType {
        rotate = 0, 
        crate0_get_closer, crate0_get_further, 
        crate3_get_closer, crate3_get_further, 
        crate2_get_closer, crate2_get_further, 
        crate1_get_closer, crate1_get_further, 
        max_movetype
    };

    static float initPosition[NUM_LIGHTS][Light::Position::max_pos];

    LightsPositionsUpdate();
    void run();

    void getPosRotz(unsigned int input_idx, XYZRotz *buff) {
        unsigned int idx = (input_idx < NUM_LIGHTS) ? (input_idx) : (NUM_LIGHTS - 1);
        if (buff) {
            buff->pos[Light::Position::x] = position[idx][Light::Position::x];
            buff->pos[Light::Position::y] = position[idx][Light::Position::y];
            buff->pos[Light::Position::z] = position[idx][Light::Position::z];
            buff->rot = curr_rotz;
        }
    }

    MoveType getMoveType() {
        return move_type;
    }

    // float(*getPosition(unsigned int input_idx))[Light::Position::max_pos] {
    p3f getPosition(unsigned int input_idx) {
        unsigned int idx = (input_idx < NUM_LIGHTS) ? (input_idx) : (NUM_LIGHTS - 1);
        return &position[idx];
    }
    
private:
    MoveType move_type;

    float position[NUM_LIGHTS][Light::Position::max_pos];
    float curr_rotz;
};
