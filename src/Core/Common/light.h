#pragma once

class Light {
public:
    enum Position {x = 0, y, z, max_pos};
    enum Color {r = 0, g, b, max_color};
    enum Attenuation {a0 = 0, a1, a2, max_att};

    void setValues(float input_pos[][Position::max_pos], 
                   float input_color[][Color::max_color], 
                   float input_att[][Attenuation::max_att]) {
        if (input_pos) {
            for (unsigned char i = Position::x; i < Position::max_pos; i++) {
                position[i] = (*input_pos)[i];
            }
        }

        if (input_color) {
            for (unsigned char i = Color::r; i < Color::max_color; i++) {
                color[i] = (*input_color)[i];
            }
        }

        if (input_att) {
            for (unsigned char i = Attenuation::a0; i < Attenuation::max_att; i++) {
                attenuation[i] = (*input_att)[i];
            }
        }
    }

    void setPosition(float input_data[][max_pos]) {
        if (input_data) {
            for (unsigned int i = Position::x; i < Position::max_pos; i++) {
                position[i] = (*input_data)[i];
            }
        }
    }

    void setColor(float input_data[][max_color]) {
        if (input_data) {
            for (unsigned int i = Color::r; i < Color::max_color; i++) {
                color[i] = (*input_data)[i];
            }
        }
    }

    void setAttenuation(float input_data[][max_att]) {
        if (input_data) {
            for (unsigned int i = Attenuation::a0; i < Attenuation::max_att; i++) {
                attenuation[i] = (*input_data)[i];
            }
        }
    }

    const float *getPosition3fv() {
        return &position[0];
    }

    const float *getColor3fv() {
        return color;
    }

    const float *getAttenuation3fv() {
        return attenuation;
    }

private:
    float position[Position::max_pos];
    float color[Color::max_color];
    float attenuation[Attenuation::max_att];
};
