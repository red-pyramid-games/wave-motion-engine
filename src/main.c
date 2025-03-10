#include <cglm/affine-pre.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include <string.h>
#include <assimp/cimport.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window, Camera* camera, float delta);
void process_mouse(GLFWwindow* window, Camera* camera);
void render_text(unsigned int program_id, const char* text, float x, float y, float scale, vec3 color);

const float WIDTH = 1920.0f;
const float HEIGHT = 1080.0f;

int main(void) {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Wave Motion Engine", NULL, NULL);
    if (!window) {
        printf("Error creating glfw window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Error loading glfw functions\n");
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader* texture_shader = shader_init(
        "../resources/shaders/texture_vs.glsl",
        "../resources/shaders/texture_fs.glsl");
    if (!texture_shader) {
        printf("Error creating shader program\n");
        return -1;
    }

    Texture* texture = texture_init("../resources/textures/container.jpg");
    if (!texture) {
        printf("Failed to load texture\n");
        return -1;
    }

    Shader* model_shader = shader_init(
        "../resources/shaders/light_basic_vs.glsl",
        "../resources/shaders/light_basic_fs.glsl");
    if (model_shader == NULL) {
        printf("Error creating shader program\n");
        return -1;
    }

    Camera* camera = camera_init_default();

    float prev = 0.0f;
    float now = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float delta = now - prev;
        prev = now;
        now = glfwGetTime();

        process_input(window, camera, delta);
        process_mouse(window, camera);
        
        camera_clear(camera);
        camera_update_model(camera, texture->transform->position);
        camera_update_view(camera);
        texture_render(texture, camera->shader_id);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_exit(texture_shader);
    glfwTerminate();

    return 0;
}

void process_input(GLFWwindow* window, Camera* camera, float delta) {
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

bool first_mouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float last_x = WIDTH / 2.0f;
float last_y = HEIGHT / 2.0f;

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

