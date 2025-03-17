#include "light.h"
#include "shader.h"

#include <cglm/vec3.h>

Light* light_init(vec3 color) {
    Light* light = malloc(sizeof(Light));

    glm_vec3_copy(color, light->color);

    light->diffuse_intensity = 0.75f;
    light->ambient_intensity = 1.0f;
    
    glm_vec3_one(light->specular);

    return light;
}

void light_exit(Light* light) {
    free(light);
}

void light_update(Light* light, const unsigned int shader_id, vec3 light_pos, vec3 camera_pos) {    
    shader_update_uniform3f(shader_id, "light.position", light_pos[0], light_pos[1], light_pos[2]);
    shader_update_uniform3f(shader_id, "viewPos", camera_pos[0], camera_pos[1], camera_pos[2]);

    glm_vec3_scale(light->color, light->diffuse_intensity, light->diffuse);
    glm_vec3_scale(light->diffuse, light->ambient_intensity, light->ambient);                                                                               //

    shader_update_uniform3f(
        shader_id, 
        "light.ambient", 
        light->ambient[0], 
        light->ambient[1], 
        light->ambient[2]);

    shader_update_uniform3f(
        shader_id, 
        "light.diffuse", 
        light->diffuse[0], 
        light->diffuse[1], 
        light->diffuse[2]);

    shader_update_uniform3f(
        shader_id, 
        "light.specular", 
        light->specular[0], 
        light->specular[1],
        light->specular[2]);
}
