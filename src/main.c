#include <cglm/cam.h>
#include <cglm/types.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "editor.h"
#include "shader.h"
#include "text.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
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

    Shader* text_shader = shader_init(
        "../resources/shaders/text_vs.glsl",
        "../resources/shaders/text_fs.glsl");
    if (!text_shader) {
        printf("Error creating shader program\n");
        return -1;
    }

    mat4 projection;
    glm_mat4_identity(projection);
    glm_ortho(0, WIDTH, 0, HEIGHT, 0, 1, projection);
    shader_use_program(text_shader->id);
    shader_update_uniform4fv(text_shader->id, "projection", projection);

    Text* text = text_init();
    if (text == NULL) {
        printf("Error creating text set\n");
        return 0;
    }
    
    Editor* editor = editor_init(window);
    if (editor == NULL) {
        printf("Error creating editor ui\n");
        return -1;
    }
    
    Camera* camera = camera_init(WIDTH, HEIGHT, text_shader->id);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        editor_clear(editor);

        text_render(text, text_shader->id, "This is sample text", 1000.0f, 1000.0f, 1.0f, (vec3){1.0, 0.0f, 1.0f});
        texture_render(texture, texture_shader->id);
        editor_render(editor);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_exit(texture_shader);
    glfwTerminate();

    return 0;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

