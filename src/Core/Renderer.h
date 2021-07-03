#pragma once

#define __USE_INLINE_METHODS__
#include "../WindowSystem/WindowSystem.h"  // To use WIN_WIDTH, WIN_HEIGHT, 'WindowSystem' class, and its input functions...

#include "Common/gl_header.h"
#include "Common/gl_math.h"

#include <iostream>

class Camera {

    gl_math::vec3 position, direc, right, up;
    float hAngle, vAngle;

public:
    gl_math::vec3 getPosition() {
        return position;
    }

    gl_math::vec3 getDirection() {
        return direc;
    }

    gl_math::vec3 getUp() {
        return up;
    }

    void setHeight(float height) {
        position.z = height;
    }

    float getHeight() {
        return position.z;
    }

    void inverseVerticalAngle() {
        vAngle = -vAngle;
    }

    // float getHAngle();
    // float getVAngle();

    // gl_math::vec3 getRight() {
    //     return right;
    // }

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

public:
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

    void input_update(WindowSystem &win);

    // void setAngles() {
    // }

public:
    Camera(gl_math::vec3 pos, float input_h = 0, float input_v = 0) {
        
        hAngle = input_h; vAngle = input_v;
        position = pos;

        calculateDirecRightUp();
    }
};

// Base class for entity-renderer, terrain-renderer, skybox-renderer, gui-renderer
class BaseRenderer {

private:
    static gl_math::mat4 projection_matrix;
    static gl_math::mat4 view_matrix;

public:
    // Supposed to be called before any object is instanced
    static bool init() {
        if (gl3wInit()) {
            return false;
        }

        std::cout << "  HighLevelRenderer() info: " << std::endl;
        std::cout << "    Vendor:  " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "    Render:  " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;
        
        // clearEntities();
        calculateProjMatrix();  // only have to calculate once if window never resized
        return true;
    }

    // only have to calculate once if window never resized
    static void calculateProjMatrix();
    static void calculateViewMatrix(const gl_math::vec3 &cam_pos, const gl_math::vec3 &cam_direc, const gl_math::vec3 &cam_up) {
        view_matrix = gl_math::lookAt(
            cam_pos,             // Camera is here
            cam_pos + cam_direc, // and looks here : at the same position, plus "camDirection"
            cam_up               // Head is up (set to 0,-1,0 to look upside-down)
        );
    }

public:
    // this is supposed to be called before rendering loop is entered to set proj_mat for the shader,
    // shader shall be started / bound-to before calling this
    float *getProjMatrix() {
        return &projection_matrix[0][0];
    }

    // this is supposed to be called in the rendering loop to set the view_mat for the shader,
    // after shader is bound to / started
    float *getViewMatrix() {
        return &view_matrix[0][0];
    }

    // Tobe called by derived class in its constructor, after
    // virtual funcs are implemented according to
    // derived class' details
    virtual void allocShadersData() = 0;  // only have to be called once before the rendering loop
    virtual void freeShadersData() = 0;
    virtual bool ready() = 0;             // to check does are shaders successfully allocated 

public:
    BaseRenderer() {
        printf("  __ base-renderer constructor called.\n");
    }

    virtual ~BaseRenderer() {
        printf("  __ base-renderer destructor called.\n");
    }
};
