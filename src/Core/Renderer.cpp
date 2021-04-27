#include "Renderer.h"

#define NEAR_PLANE (0.1f)
#define FAR_PLANE  (9999.0f)
#define FOV        (45.0f)

#define MOUSE_ROTATE_SPEED (0.005f)
#define     KEY_MOVE_SPEED (0.05f)
#define             FACTOR (1.0f)

gl_math::mat4 BaseRenderer::projection_matrix = gl_math::mat4(1.0f);
gl_math::mat4 BaseRenderer::view_matrix = gl_math::mat4(1.0f);

void BaseRenderer::calculateProjMatrix() {
    projection_matrix = gl_math::perspective(gl_math::radians(FOV), (float)WIN_WIDTH / WIN_HEIGHT, NEAR_PLANE, FAR_PLANE);
}

void Camera::input_update(WindowSystem &win) {
    // glfwGetTime is called only once for lastTime (the first time this function is called)
    //
    // static double lastTime = glfwGetTime();
    // float deltaTime; {
    //     // Compute time difference between current and last frame
    //     double currentTime = glfwGetTime();
    //     deltaTime = float(currentTime - lastTime);
    // }

    // Set cam's h/v angle according to cursor movement
    {
        // Get mouse position
        double xpos, ypos; {
            win.getCursorPos(&xpos, &ypos);
        }

        if (xpos != WIN_WIDTH / 2 || ypos != WIN_HEIGHT / 2) {
            // Reset mouse position for next frame
            win.setCursorPos(WIN_WIDTH / 2, WIN_HEIGHT / 2);

            // Compute new orientation
            hAngle += MOUSE_ROTATE_SPEED * (float)(xpos - WIN_WIDTH / 2);  // moveLeft --, moveRight ++
            vAngle += MOUSE_ROTATE_SPEED * (float)(WIN_HEIGHT / 2 - ypos); // moveUp ++, moveDown --

            calculateDirecRightUp();
        }
    }

    // Set cam's pos according to key input
    {
        // Move forward
        if (win.getKey( WIN_SYS_KEY_UP ) == WIN_SYS_PRESS) {
            position += direc * FACTOR * KEY_MOVE_SPEED;
        }
        // Move backward
        if (win.getKey( WIN_SYS_KEY_DOWN ) == WIN_SYS_PRESS) {
            position -= direc * FACTOR * KEY_MOVE_SPEED;
        }
        // Strafe right
        if (win.getKey( WIN_SYS_KEY_RIGHT ) == WIN_SYS_PRESS) {
            position += right * FACTOR * KEY_MOVE_SPEED;
        }
        // Strafe left
        if (win.getKey( WIN_SYS_KEY_LEFT ) == WIN_SYS_PRESS) {
            position -= right * FACTOR * KEY_MOVE_SPEED;
        }

        if (win.getKey( WIN_SYS_KEY_R ) == WIN_SYS_PRESS) {
            // float KEY_MOVE_SPEED = .01f;
            position.z += KEY_MOVE_SPEED * FACTOR;
        }
        if (win.getKey( WIN_SYS_KEY_F ) == WIN_SYS_PRESS) {
            // float KEY_MOVE_SPEED = .01f;
            position.z -= KEY_MOVE_SPEED * FACTOR;
        }

        if (win.getKey(WIN_SYS_KEY_V) == WIN_SYS_PRESS) {
            // float KEY_MOVE_SPEED = .01f;
            printf("cam  position: %f, %f, %f\n", position.x, position.y, position.z);
            printf("h angle      : %f\n", hAngle);
            printf("v angle      : %f\n\n", vAngle);
            printf("cam direction: %f, %f, %f\n", direc.x, direc.y, direc.z);
            printf("cam        up: %f, %f, %f\n\n\n", up.x, up.y, up.z);
        }
    }
}
