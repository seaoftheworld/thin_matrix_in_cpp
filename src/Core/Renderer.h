#define __USE_INLINE_METHODS__
#include "../WindowSystem/WindowSystem.h"  // To use WIN_WIDTH, WIN_HEIGHT, 'WindowSystem' class, and its input functions...

#include "Shader/StaticShader.h"
#include "Shader/SpecularShader.h"
#include "Shader/MultiLightsShader.h"

#include "Common/gl_header.h"
#include "Common/gl_math.h"
#include "Common/data.h"

#include <iostream>

class Camera {

public:
    Camera(gl_math::vec3 pos, float input_h = 0, float input_v = 0) {
        
        hAngle = input_h; vAngle = input_v;
        position = pos;

        calculateDirecRightUp();
    }

    // float getHAngle();
    // float getVAngle();
    gl_math::vec3 getPosition() {
        return position;
    }

    gl_math::vec3 getDirection() {
        return direc;
    }

    gl_math::vec3 getRight() {
        return right;
    }

    gl_math::vec3 getUp() {
        return up;
    }

    void setPositionAngle(float input_data[][5]) {
        if (input_data) {
            position.x = (*input_data)[0];
            position.y = (*input_data)[1];
            position.z = (*input_data)[2];

            hAngle = (*input_data)[3];
            vAngle = (*input_data)[4];

            calculateDirecRightUp();
        }
    }

    void setAngles() {
    }

    void input_update(WindowSystem &win);

private:
    void calculateDirecRightUp() {
        direc = gl_math::vec3(
            cos(vAngle) * sin(hAngle),
            cos(vAngle) * cos(hAngle),
            sin(vAngle)
        );

        right = gl_math::vec3(
            sin(3.14f / 2.0f - hAngle),
            cos(-3.14f / 2.0f - hAngle),
            0
        );   

        up = gl_math::cross(right, direc);
    }
    
    gl_math::vec3 position, direc, right, up;
    float hAngle, vAngle;
};

class Renderer {

public:
    static int init() {
        if (gl3wInit()) {
            return 0;
        }
        return 1;
    }

    // Renderer(StaticShader *shader) {
    Renderer() {
        std::cout << "  Renderer() info: " << std::endl;
        std::cout << "    Vendor:  " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "    Render:  " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;

        calculateProjMatrix();
    }

    void calculateProjMatrix();
    
    void calculateViewMatrix(const gl_math::vec3 &cam_pos, const gl_math::vec3 &cam_direc, const gl_math::vec3 &cam_up) {    
        view_matrix = gl_math::lookAt(
            cam_pos,             // Camera is here
            cam_pos + cam_direc, // and looks here : at the same position, plus "camDirection"
            cam_up               // Head is up (set to 0,-1,0 to look upside-down)
        );
    }

    // this is supposed to be called before rendering loop is entered,
    // shader shall be started / bound-to before calling this
    float *getProjMatrix() {
        return &projection_matrix[0][0];
    }
        // // void loadProjMatrixToShader(StaticShader *shader) {
        // void loadProjMatrixToShader(SpecularShader *shader) {
        //     if (shader) {
        //         shader->start();
        //         shader->loadProjMatrix(&projection_matrix[0][0]);
        //         shader->stop();
        //     }
        // }

    // this is supposed to be called in the rendering loop is,
    // after shader is bound to / started
    float *getViewMatrix() {
        return &view_matrix[0][0];
    }
        // // void loadViewMatrixToShader(StaticShader *shader) {
        // void loadViewMatrixToShader(SpecularShader *shader) {
        //     if (shader) {
        //         shader->loadViewMatrix(&view_matrix[0][0]);
        //     }
        // }

    void prepare() {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    // void render(Entity *entity, StaticShader *shader);
    void render(Entity *entity, BaseShader *shader, unsigned int str_rot);

private:
    gl_math::mat4 projection_matrix = gl_math::mat4(1.0f);
    gl_math::mat4 view_matrix = gl_math::mat4(1.0f);
};
