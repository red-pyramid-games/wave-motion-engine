#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct Texture {
    unsigned int id;
    unsigned int vao;
} Texture;

Texture* texture_init(const char* path);
void texture_exit(Texture* texture);
void texture_render(Texture* texture, const unsigned int pragram_id);

#endif
