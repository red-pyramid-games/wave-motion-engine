#ifndef VERTEX_H
#define VERTEX_H

#include <cglm/types.h>

typedef struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 tex_coords;
} Vertex;

Vertex* vertex_init();
void vertex_exit(Vertex* vertex);

#endif
