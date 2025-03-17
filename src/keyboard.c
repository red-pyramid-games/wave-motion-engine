#include "keyboard.h"

#include "camera.h"

#include <cglm/affine.h>
#include <cglm/vec3.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

void process_input(GLFWwindow *window, Camera *camera, float delta) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    static const float camera_speed = 5.0f;
    vec3 right;
    glm_vec3_cross(camera->front, camera->up, right);
    glm_normalize(right);
    glm_vec3_scale(right, (camera_speed * delta), right);

    vec3 front_scaled;
    glm_vec3_scale(camera->front, (camera_speed * delta), front_scaled);

    if (glfwGetKey(window, GLFW_KEY_W)) {
        glm_vec3_add(camera->position, front_scaled, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        glm_vec3_sub(camera->position, front_scaled, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        glm_vec3_sub(camera->position, right, camera->position);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        glm_vec3_add(camera->position, right, camera->position);
    }
}
