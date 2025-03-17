#include "mouse.h"

#include "camera.h"
#include "graphics.h"

#include <GLFW/glfw3.h>
#include <cglm/util.h>
#include <cglm/vec3.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

static bool first_mouse = true;
static float yaw = -90.0f;
static float pitch = 0.0f;
static float last_x = DEFAULT_SCREEN_WIDTH / 2.0f;
static float last_y = DEFAULT_SCREEN_HEIGHT / 2.0f;

void process_mouse(GLFWwindow* window, Camera* camera) {
    double xpos_in;
    double ypos_in;
    glfwGetCursorPos(window, &xpos_in, &ypos_in);
    float xpos = (float)xpos_in;
    float ypos = (float)ypos_in;

    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse= false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top
    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 front;
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_normalize(front);
    memcpy(camera->front, front, sizeof(vec3));
}
