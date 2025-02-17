#include "text.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "freetype/freetype.h"
#include FT_FREETYPE_H  

#include "shader.h"

static Character characters[128];

Text* text_init() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("Failed to init freetype library\n");
        return NULL;
    }

    const char* font_name = "../resources/fonts/LiberationSerif-Bold.ttf";
    FT_Face face;
    if (FT_New_Face(ft, font_name, 0, &face)) {
        printf("Failed to load font name\n");
        return NULL;
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

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Text* text = malloc(sizeof(Text));
    text->vao = vao;
    text->vbo = vbo;
    memcpy(text->character_set, characters, sizeof(Character) * ASCII_SIZE);
    return text;
}

void text_exit(Text* text) {
    if (text == NULL) {
        return;
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
    float x, 
    float y, 
    const float scale, 
    const vec3 color
) {
    glEnable(GL_BLEND);
    shader_use_program(program_id);
    shader_update_uniform3f(program_id, "textColor", color[0], color[1], color[2]);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text->vao);

    int length = strlen(string);
    for (int i = 0; i < length; i++) {
        Character ch;
        ch.character = -1;
        for (int j = 0; j < 128; j++) {
            if (string[i] == text->character_set[j].character) {
                ch = text->character_set[j];
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
            glBindBuffer(GL_ARRAY_BUFFER, text->vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (ch.advance >> 6) * scale;
        }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

