#ifndef LIGHT_H
#define LIGHT_H

#include <cglm/types.h>

typedef struct Light {
    vec3 color;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float diffuse_intensity;
    float ambient_intensity;
} Light;

//typedef struct DirectionLight {
//    Light* light_base;
//
//    vec3 direction;
//} DirectionLight;
//
//typedef struct PointLight {
//    struct Light* light_base;
//
//    vec3 position;
//
//    float constant;
//    float linear;
//    float quadratic;
//} PointLight;
//
//typedef struct SpotLight {
//    struct PointLight* point_light;
//
//    float cutOff;
//    float outerCutOff;
//} SpotLight;

Light* light_init(vec3 color);
void light_exit(Light* light);

void direction_light_update(Light* light, const unsigned int shader_id);
void point_light_update(
    Light* light, 
    const unsigned int shader_id, 
    vec3 light_pos, 
    vec3 camera_pos, 
    const unsigned int idx);    
void spot_light_update(Light* light, const unsigned int shader_id, vec3 camera_pos, vec3 camera_front);
void light_update(Light* light, const unsigned int shader_id, vec3 light_pos, vec3 camera_pos);

#endif
