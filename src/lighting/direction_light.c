#include "lighting/direction_light.h"
#include "lighting/light.h"
#include "shader.h"

#include <cglm/vec3.h>

DirectionLight* direction_light_init(vec3 direction) {
    DirectionLight* light = malloc(sizeof(DirectionLight));
    vec3 ambient = { 0.05f, 0.05f, 0.05f }; 
    vec3 diffuse = { 0.4f, 0.4f, 0.4f }; 
    vec3 specular = { 0.5f, 0.5f, 0.5f }; 
    light->light_base = light_init(ambient, diffuse, specular);
    glm_vec3_copy(direction, light->direction);

    return light;
}

void direction_light_exit(DirectionLight *light) {
    light_exit(light->light_base);
    free(light);
}

void direction_light_update(DirectionLight *light, const unsigned int shader_id) {
    shader_update_uniform3f(
        shader_id, 
        "dirLight.direction", 
        light->direction[0], 
        light->direction[1], 
        light->direction[2]); 

    shader_update_uniform3f(
        shader_id, 
        "dirLight.ambient", 
        light->light_base->ambient[0], 
        light->light_base->ambient[1], 
        light->light_base->ambient[2]); 

    shader_update_uniform3f(
        shader_id, 
        "dirLight.diffuse", 
        light->light_base->diffuse[0], 
        light->light_base->diffuse[1], 
        light->light_base->diffuse[2]); 

    shader_update_uniform3f(
        shader_id, 
        "dirLight.specular", 
        light->light_base->specular[0], 
        light->light_base->specular[1], 
        light->light_base->specular[2]); 
}
