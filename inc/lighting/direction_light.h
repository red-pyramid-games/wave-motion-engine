#ifndef DIRECTION_LIGHT_H
#define DIRECTION_LIGHT_H

#include <cglm/types.h>

typedef struct DirectionLight {
    struct Light* light_base;
    vec3 direction;
} DirectionLight;

DirectionLight* direction_light_init(vec3 direction);
void direction_light_exit(DirectionLight* light);

void direction_light_update(DirectionLight* light, const unsigned int shader_id);

#endif
