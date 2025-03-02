#include "editor.h"
#include <cglm/affine-pre.h>
#include <cglm/mat4.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window, Camera* camera, float delta);
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

    Camera* camera = camera_init_default();
    Editor* editor = editor_init(window);
    if (editor == NULL) {
        printf("Error creating editor gui\n");
        return -1;
    }

    float prev = 0.0f;
    float now = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float delta = now - prev;
        prev = now;
        now = glfwGetTime();

        process_input(window, camera, delta);
        
        camera_clear(camera);
        camera_update_model(camera, texture->transform->position);
        camera_update_view(camera);
        editor_render_camera_details(editor, camera);
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

    if (glfwGetKey(window, GLFW_KEY_W)) {
        camera->position[1] += 5.0f * delta; 
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        camera->position[1] -= 5.0f * delta;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        camera->position[0] -= 5.0f * delta;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        camera->position[0] += 5.0f * delta;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

