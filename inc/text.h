#ifndef TEXT_H
#define TEXT_H

#include "cglm/types.h"

typedef struct Text {
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture_id;
    ivec2* size;
    ivec2* bearing;
    unsigned int advance;
} Text;

Text* text_init();
void text_exit(Text* text);
void text_render(
    const Text* text, 
    const unsigned int program_id, 
    const char* string, 
    const float x, 
    const float y, 
    const float scale, 
    const vec3 color
);


#endif
