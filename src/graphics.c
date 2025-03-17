#include "graphics.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

GLFWwindow* window = NULL;

bool graphics_init() {
    if (window != NULL) {
        return false;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        DEFAULT_SCREEN_WIDTH, 
        DEFAULT_SCREEN_HEIGHT, 
        "LearnOpenGL", 
        NULL, 
        NULL);

    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void graphics_exit() {
    glfwTerminate();
}

bool graphics_window_should_close() {
    return glfwWindowShouldClose(window);
}

void graphics_swap_buffers(GLFWwindow *window) {
    glfwSwapBuffers(window);
}

void graphics_poll_events() {
    glfwPollEvents();
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

