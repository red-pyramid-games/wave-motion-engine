#ifndef CUBE_H
#define CUBE_H

#include <cglm/types.h>

typedef struct Camera Camera;

typedef struct Cube {
    struct Model* model;
    struct Shader* shader;
    struct Material* material;
    struct Light* light;
    struct Transform* transform;
    vec3 color;
} Cube;

Cube* cube_init();
void cube_exit(Cube* cube);

void cube_draw(Cube* cube);

#endif
