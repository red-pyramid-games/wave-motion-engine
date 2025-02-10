#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct Texture {
    unsigned int id;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
} Texture;

Texture* texture_init(const char* path);
void texture_exit(Texture* texture);

#endif
