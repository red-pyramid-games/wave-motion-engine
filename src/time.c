#include "time.h"

#include <GLFW/glfw3.h>

float get_delta_time() {
    static float lastFrame = 0.0f;

    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    return deltaTime;
}
