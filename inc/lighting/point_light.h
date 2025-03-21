#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <cglm/types.h>
typedef struct PointLight {
    struct Light* light_base;

    float constant; 
    float linear ; 
    float quadradic;

    unsigned int id;
} PointLight;

PointLight* point_light_init(
    const unsigned int idx,
    vec3 ambient,
    vec3 diffuse,
    vec3 specular);

void point_light_exit(PointLight* point_light);

void point_light_update(
    PointLight* light, 
    const unsigned int shader_id, 
    vec3 light_pos, 
    vec3 camera_pos); 

#endif
