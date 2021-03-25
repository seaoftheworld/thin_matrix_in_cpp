#define __NOT_USE_INLINE_METHODS__
#include "WindowSystem.h"

#define GLFW_INCLUDE_NONE  // By default, glfw3.h includes <GL/gl.h> for gl1.1 support shipped by vs2019 without this line.
#include <GLFW/glfw3.h>    // Several apis in gl1.1 header will overlap with mordern gl extented by gl3w and cause trouble.

#include <iostream>

int WindowSystem::start() {
	std::cout << "00. Init GLFW for OpenGL Window and Context" << std::endl;
    if (!glfwInit()) {
        std::cout << "  glfwInit() failed !" << std::endl;

        glfwTerminate();
        return -2;
    }

    GLFWwindow *glfwWindow = NULL;
    glfwWindow = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "this is a test", NULL, NULL);
    if (!glfwWindow) {
        std::cout << "  glfwCreateWindow() failed !" << std::endl;

        // glfwDestroyWindow(glfwWindow);
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(glfwWindow, 100, 100);
    glfwMakeContextCurrent(glfwWindow);
    window = glfwWindow;
    // valid = true;
    return 0;
}

void WindowSystem::stop() {
	if (window) {
		glfwDestroyWindow((GLFWwindow *)window);
	}
	glfwTerminate();
}
