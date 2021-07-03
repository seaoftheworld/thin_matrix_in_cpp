#include "gl_math.h"

#include "stdio.h"

namespace gl_math {

    void print_mat4(const char* prefix, mat4 m) {

        unsigned char i, j;
        // const float* p = (const float*)glm::value_ptr(m);
        const float* p = (const float*)&m[0][0];

        for (i = 0; i < 4; i++) {
            printf("%s[%d, 0~3]: ", prefix, i);
            for (j = 0; j < 4; j++) {
                printf("%f, ", p[i * 4 + j]);
            }
            printf("\n");
        }
        printf("\n\n");
    }

    vec3 normalize_vec3(vec3 input) {
        return glm::normalize(input);
    }

    // matrix = T * R * S calculation for normal model's matrix
    void model_matrix_SRT_Normal(const float transform[][Entity::transform::max], mat4 *result) {

        #define TRANS_INFO(x) ( (*transform)[Entity::transform::x] )

        mat4 translate = glm::translate(
            mat4(1.0f),
            vec3(TRANS_INFO(x), TRANS_INFO(y), TRANS_INFO(z)) );

        mat4   rotate_x = glm::rotate(mat4(1.0f), TRANS_INFO(rot_x), vec3(1.0f, 0.0f, 0.0f));
        mat4   rotate_xy = glm::rotate(rotate_x, TRANS_INFO(rot_y), vec3(0.0f, 1.0f, 0.0f));
        mat4   rotate_xyz = glm::rotate(rotate_xy, TRANS_INFO(rot_z), vec3(0.0f, 0.0f, 1.0f));
        // mat4   rotate_z = glm::rotate(mat4(1.0f), TRANS_INFO(rot_z), vec3(0.0f, 0.0f, 1.0f));
        
        mat4 scale = glm::scale(
            mat4(1.0f), 
            vec3(TRANS_INFO(scale), TRANS_INFO(scale), TRANS_INFO(scale)) );
        
        *result = translate * rotate_xyz * scale;
    }

    // matrix = R * T * S calculation to make a model rotate about x/y/z axis
    void model_matrix_STR_Rot(const float transform[][Entity::transform::max], mat4 *result) {

        #define TRANS_INFO(x) ( (*transform)[Entity::transform::x] )

        mat4 translate = glm::translate(
            mat4(1.0f),
            vec3(TRANS_INFO(x), TRANS_INFO(y), TRANS_INFO(z)) );

        mat4   rotate_x = glm::rotate(mat4(1.0f), TRANS_INFO(rot_x), vec3(1.0f, 0.0f, 0.0f));
        mat4   rotate_xy = glm::rotate(rotate_x, TRANS_INFO(rot_y), vec3(0.0f, 1.0f, 0.0f));
        mat4   rotate_xyz = glm::rotate(rotate_xy, TRANS_INFO(rot_z), vec3(0.0f, 0.0f, 1.0f));
        // mat4   rotate_z = glm::rotate(mat4(1.0f), TRANS_INFO(rot_z), vec3(0.0f, 0.0f, 1.0f));
        
        mat4 scale = glm::scale(
            mat4(1.0f), 
            vec3(TRANS_INFO(scale), TRANS_INFO(scale), TRANS_INFO(scale)) );
        
        *result = rotate_xyz * translate * scale;
    }

    void create_transform_matrix(const float input_pos[][2], const float input_scale[][2], mat4 *result) {

        // #define TRANS_INFO(x) ( (*transform)[Entity::transform::x] )

        mat4 translate = glm::translate(
            mat4(1.0f),
            vec3((*input_pos)[0], (*input_pos)[1], 0.0f)
        );

            // mat4   rotate_x = glm::rotate(mat4(1.0f), TRANS_INFO(rot_x), vec3(1.0f, 0.0f, 0.0f));
            // mat4   rotate_xy = glm::rotate(rotate_x, TRANS_INFO(rot_y), vec3(0.0f, 1.0f, 0.0f));
            // mat4   rotate_xyz = glm::rotate(rotate_xy, TRANS_INFO(rot_z), vec3(0.0f, 0.0f, 1.0f));
            // // mat4   rotate_z = glm::rotate(mat4(1.0f), TRANS_INFO(rot_z), vec3(0.0f, 0.0f, 1.0f));
        
        mat4 scale = glm::scale(
            mat4(1.0f), 
            vec3((*input_scale)[0], (*input_scale)[1], 1.0f) );
        
        // *result = translate * rotate_xyz * scale;
        *result = translate * scale;
    }

    void create_transform_matrix(const float input_pos[][3], float scale_x, float scale_y, mat4 *result) {

        mat4 translate = glm::translate(
            mat4(1.0f), vec3((*input_pos)[0], (*input_pos)[1], (*input_pos)[2])
        );

        mat4 scale = glm::scale(
            mat4(1.0f), vec3(scale_x, scale_y, 1.0f) );

        // *result = translate * rotate_xyz * scale;
        // *result = translate * rotate_xyz * scale;
        *result = translate * scale;
    }

    // Supposed tobe used to get the positions of 4 lights according to 4 transforms of crates.
    // Crates are rendered with STR model-matrix, not normal SRT matrix,
    // so even though a crate is rendered and spins around axis-z,
    // the xyz of this crate fixed, only rotz changes
    void get_xyz_from_rotz(float input_pos[][3], float rot_z, float output_result[][3]) {

        if (!input_pos) {
            return;
        }
        
        mat4 translate = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f) );

        mat4   rotate_x = glm::rotate(mat4(1.0f), 0.0f, vec3(1.0f, 0.0f, 0.0f));
        mat4   rotate_xy = glm::rotate(rotate_x, 0.0f, vec3(0.0f, 1.0f, 0.0f));
        mat4   rotate_xyz = glm::rotate(rotate_xy, rot_z, vec3(0.0f, 0.0f, 1.0f));
        // mat4   rotate_xyz = glm::rotate(rotate_xy, glm::radians(rot_z), vec3(0.0f, 0.0f, 1.0f));
        
        mat4 scale = glm::scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) );
        
        mat4 srt = translate * rotate_xyz * scale;
            // printf("xyz_rot_z() input pos: %f, %f, %f\n", (*input_pos)[0], (*input_pos)[1], (*input_pos)[2]);
            // printf("xyz_rot_z() input z  : %f\n", rot_z);

        vec4 result = srt * vec4((*input_pos)[0], (*input_pos)[1], (*input_pos)[2], 1.0f);
            // printf("xyz_rot_z() result   : %f, %f, %f\n", result.x, result.y, result.z);

        if (output_result) {
            (*output_result)[0] = result.x;
            (*output_result)[1] = result.y;
            (*output_result)[2] = result.z;
        }
    }

    mat4 perspective(float fov, float aspect_ratio, float near_plane, float far_plane) {
        return glm::perspective(fov, aspect_ratio, near_plane, far_plane);
    }
}
