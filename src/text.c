#include "text.h"

#include "cglm/types.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "freetype/freetype.h"
#include FT_FREETYPE_H  

#include "shader.h"

Text* text_init() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("Error initializing freetype library\n");
        return NULL;
    }

    FT_Face face;
    if (FT_New_Face(ft, "../resources/fonts/MesloLGS NF Regular.ttf", 0, &face)) {
        printf("Error reading font\n");
        return NULL;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
        printf("Error loading char: X\n");
        return NULL;
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

    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Text* text = malloc(sizeof(Text));
    text->vao = vao;
    text->vbo = vbo;
    text->texture_id = texture;

    text->size = malloc(sizeof(ivec2));
    *text->size[0] = face->glyph->bitmap.width;
    *text->size[1] = face->glyph->bitmap.rows;

    text->bearing = malloc(sizeof(ivec2));
    *text->bearing[0] = face->glyph->bitmap_left;
    *text->bearing[1] = face->glyph->bitmap_top;

    text->advance = face->glyph->advance.x;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return text;
}

void text_exti(Text* text) {
    if (text == NULL) {
        return;
    }

    if (text->size) {
        free(text->size);
    }

    if (text->bearing) {
        free(text->bearing);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteVertexArrays(1, &text->vao);
    glDeleteBuffers(1, &text->vbo);

    free(text);
}

void text_render(
    const Text* text, 
    const unsigned int program_id, 
    const char* string, 
    const float x, 
    const float y, 
    const float scale, 
    const vec3 color
) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader_use_program(program_id);
    shader_update_uniform3f(program_id, "text_color", color[0], color[1], color[2]);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text->vao);


    float xpos = x + *text->bearing[0] * scale;
    float ypos = y - (*text->size[1] - *text->bearing[1]) * scale;

    float w = *text->size[0] * scale;
    float h = *text->size[1] * scale;
    
    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },            
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f } 
    };

    glBindTexture(GL_TEXTURE_2D, text->texture_id);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, text->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
