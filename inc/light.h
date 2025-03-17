#ifndef LIGHT_H
#define LIGHT_H

#include <cglm/types.h>

typedef struct Light {
    vec3 color;
    vec3 ambient;
    float ambient_intensity;
    vec3 diffuse;
    float diffuse_intensity;
    vec3 specular;
} Light;

Light* light_init(vec3 color);
void light_exit(Light* light);

void light_update(Light* light, const unsigned int shader_id, vec3 light_pos, vec3 camera_pos);    

#endif
