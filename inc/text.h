#ifndef TEXT_H
#define TEXT_H

#include "cglm/types.h"

#define ASCII_SIZE 128

typedef struct Character {
    unsigned int texture_id;
    ivec2 size;
    ivec2 bearing;
    unsigned int advance;
    char character;
} Character;

typedef struct Text {
    unsigned int vao;
    unsigned int vbo;
    Character character_set[128];
} Text;

Text* text_init();
void text_exit(Text* text);
void text_render(
    const Text* text, 
    const unsigned int program_id, 
    const char* string, 
    float x, 
    float y, 
    const float scale, 
    const vec3 color
);


#endif
