#pragma once

#define WIN_WIDTH  (1200)
#define WIN_HEIGHT (600)

class WindowSystem {
public:
    static WindowSystem &instance() {
        static WindowSystem winSys;
        return winSys;
    }

    int start();
    void stop();

    double getTime();
    int getKey(int key);

    bool isValid();
    void pollEvents();
    void swapBuffers();


    void getCursorPos(double *x, double *y);
    void setCursorPos(double x, double y);

private:
    void *window;
    // bool valid;
};

// #if (1)
#ifdef __USE_INLINE_METHODS__
    #define GLFW_INCLUDE_NONE  // By default, glfw3.h includes <GL/gl.h> for gl1.1 support shipped by vs2019 without this line.
    #include <GLFW/glfw3.h>    // Several apis in gl1.1 header will overlap with mordern gl extented by gl3w and cause trouble.

    #define WIN_SYS_KEY_ESC (GLFW_KEY_ESCAPE)

    #define WIN_SYS_KEY_W (GLFW_KEY_W)
    #define WIN_SYS_KEY_S (GLFW_KEY_S)
    #define WIN_SYS_KEY_A (GLFW_KEY_A)
    #define WIN_SYS_KEY_D (GLFW_KEY_D)

    #define WIN_SYS_KEY_R (GLFW_KEY_R)
    #define WIN_SYS_KEY_F (GLFW_KEY_F)

    #define WIN_SYS_KEY_UP    (GLFW_KEY_UP)
    #define WIN_SYS_KEY_DOWN  (GLFW_KEY_DOWN)
    #define WIN_SYS_KEY_LEFT  (GLFW_KEY_LEFT)
    #define WIN_SYS_KEY_RIGHT (GLFW_KEY_RIGHT)
    #define WIN_SYS_KEY_V     (GLFW_KEY_V)

    #define WIN_SYS_PRESS     (GLFW_PRESS)

    inline double WindowSystem::getTime() {
        return glfwGetTime();
    }

    inline int WindowSystem::getKey(int key) {
        return glfwGetKey((GLFWwindow*)window, key);
    }

    inline bool WindowSystem::isValid() {
        if ( getKey(WIN_SYS_KEY_ESC) == WIN_SYS_PRESS || glfwWindowShouldClose((GLFWwindow*)window) ) {
            return 0;
        }
        else {
            return 1;
        }
    }

    inline void WindowSystem::pollEvents() {
        glfwPollEvents();
    }

    inline void WindowSystem::swapBuffers() {
        glfwSwapBuffers( (GLFWwindow *)window );
    }

    inline void WindowSystem::getCursorPos(double *x, double *y) {
        glfwGetCursorPos((GLFWwindow *)window, x, y);
    }

    inline void WindowSystem::setCursorPos(double x, double y) {
        glfwSetCursorPos((GLFWwindow*)window, x, y);
    }
#endif
