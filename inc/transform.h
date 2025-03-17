#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cglm/types.h>

typedef struct Transform {
    vec3 position;
    vec3 rotation;
    vec3 scale;
} Transform;

Transform* transform_init_default();
void transform_exit(Transform* transform);

#endif
