#include <cglm/cam.h>
#include <cglm/types.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void render_text(unsigned int program_id, const char* text, float x, float y, float scale, vec3 color);

const float WIDTH = 1920.0f;
const float HEIGHT = 1080.0f;

typedef struct Character {
    unsigned int texture_id;
    ivec2 size;
    ivec2 bearing;
    unsigned int advance;
    char character;
} Character;

Character characters[128];
unsigned int vao, vbo;

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

    mat4 projection;
    glm_ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f, projection);
    shader_use_program(text_shader->id);
    shader_update_uniform4fv(text_shader->id, "projection", projection);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("Failed to init freetype library\n");
        return -1;
    }

    const char* font_name = "../resources/fonts/LiberationSerif-Bold.ttf";
    FT_Face face;
    if (FT_New_Face(ft, font_name, 0, &face)) {
        printf("Failed to load font name\n");
        return -1;
    } else {
        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (int i = 0; i < 128; i++) {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                printf("Failed to load glyph\n");
                continue;
            }
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
            Character character = {
                texture,
                {face->glyph->bitmap.width, face->glyph->bitmap.rows},
                {face->glyph->bitmap_left, face->glyph->bitmap_top},
                face->glyph->advance.x,
                i,
            };
            characters[i] = character;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render_text(text_shader->id, "This is sample text", WIDTH / 2, HEIGHT / 2, 1.0f, (vec3){0.5, 0.8f, 0.2f});
        //texture_render(texture, texture_shader->id);

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

void render_text(unsigned int program_id, const char* text, float x, float y, float scale, vec3 color) {
    shader_use_program(program_id);
    shader_update_uniform3f(program_id, "textColor", color[0], color[1], color[2]);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    int length = strlen(text);
    for (int i = 0; i < length; i++) {
        Character ch;
        ch.character = -1;
        for (int j = 0; j < 128; j++) {
            if (text[i] == characters[j].character) {
                ch = characters[j];
            }
        }

        if (ch.character > -1) {
            float xpos = x + ch.bearing[0] * scale;
            float ypos = y - (ch.size[1] - ch.bearing[1]) * scale;

            float w = ch.size[0] * scale;
            float h = ch.size[1] * scale;

            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.texture_id);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (ch.advance >> 6) * scale;
        }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
