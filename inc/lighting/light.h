#ifndef LIGHT_H
#define LIGHT_H

#include <cglm/types.h>

typedef struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} Light;

Light* light_init(vec3 ambient, vec3 diffuse, vec3 specular);
void light_exit(Light* light);

#endif
