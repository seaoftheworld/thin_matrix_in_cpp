#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "data.h"  // To get S.R.T.(translate) matrix from a Entity::transform::max
// using namespace glm;

namespace gl_math {

    typedef glm::mat4 mat4;
    typedef glm::vec4 vec4;
    typedef glm::vec3 vec3;

    void print_mat4(const char *prefix, mat4 m);

    vec3 normalize_vec3(vec3 input);

    // Used gui-data and watertile-data as input, to generate model-matrix
    // for gui and water renderers.
    void create_transform_matrix(const float pos[][2], const float scale[][2], mat4 *result);
    void create_transform_matrix(const float pos[][3], float scale_x, float scale_y, mat4 *result);

    void model_matrix_SRT_Normal(const float transform[][Entity::transform::max], mat4 *result);
    void model_matrix_STR_Rot(const float transform[][Entity::transform::max], mat4 *result);
    void get_xyz_from_rotz(float input_pos[][3], float rot_z, float output_result[][3]);

    mat4 perspective(float fov, float aspect_ratio, float near, float far);

    // tan ???
    // void perspective(float fov, float aspect_ratio, float near, float far, mat4 *result) {
    // }

    // void rotate(float pitch, mat4 *result) {
    //     glm::rotate();
    // }

    inline float radians(float n) {
        return glm::radians(n);
    }

    inline vec3 cross(vec3 a, vec3 b) {
        return glm::cross(a, b);
    }

    inline mat4 lookAt(vec3 pos, vec3 pos_dir, vec3 up) {
        return glm::lookAt(pos, pos_dir, up);
    }

    inline mat4 mat4_multiply(mat4 a, mat4 b) {
        return (a * b);
    }

    inline mat4 mat4_multiply(mat4 a, mat4 b, mat4 c) {
        return (a * b * c);
    }
}
