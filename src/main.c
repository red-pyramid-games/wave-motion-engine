#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/cam.h>
#include <stdbool.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "editor.h"
#include "shader.h"
#include "text.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

const float WIDTH = 1920.0f;
const float HEIGHT = 1080.0f;

int
main(void) {
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

    Shader* text_shader = shader_init(
        "../resources/shaders/text_vs.glsl", 
        "../resources/shaders/text_fs.glsl");
    if (!text_shader) {
        printf("Error creating shader program\n");
        return -1;
    }

    Shader* texture_shader = shader_init(
        "../resources/shaders/texture_vs.glsl",
        "../resources/shaders/texture_fs.glsl");
    if (!texture_shader) {
        printf("Error creating shader program\n");
        return -1;
    }

    Text* text = text_init();
    if(text == NULL) {
        printf("Error creating text\n");
        return -1;
    }

    Editor* editor = editor_init(window);

    Texture* texture = texture_init("../resources/textures/crate.jpeg");
    if (!texture) {
        printf("Failed to create texture\n");
        return -1;
    }

    //mat4 projection;
    //glm_ortho(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 0.1f, projection);
    //shader_update_uniform4fv(text_shader->id, "projection", projection);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        editor_clear(editor);

        glBindTexture(GL_TEXTURE_2D, texture->id);
        shader_use_program(texture_shader->id);
        glBindVertexArray(texture->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        vec3 color = { 0.0f, 0.0f, 0.0f };
        text_render(text, text_shader->id, "X", WIDTH / 2.0f, HEIGHT / 2.0f, 1, color);

        editor_render(editor);  

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    texture_exit(texture);
    editor_exit(editor);
    shader_exit(text_shader);
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
