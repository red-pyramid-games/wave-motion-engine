#include "lighting/point_light.h"
#include "lighting/light.h"

#include "shader.h"

#include <stdlib.h>

#define INDEX_POSITION 12

static char light_position [] = "pointLights[0].position";
static char light_ambient  [] = "pointLights[0].ambient";
static char light_diffuse  [] = "pointLights[0].diffuse";
static char light_specular [] = "pointLights[0].specular";
static char light_constant [] = "pointLights[0].constant";
static char light_linear   [] = "pointLights[0].linear";
static char light_quadradic[] = "pointLights[0].quadratic";

PointLight* point_light_init(
    const unsigned int idx,
    vec3 ambient,
    vec3 diffuse,
    vec3 specular
) {
    PointLight* light = malloc(sizeof(PointLight));
    light->id = idx;
    light->light_base = light_init(ambient, diffuse, specular);
    light->constant = 1.0f;
    light->linear = 0.09f;
    light->quadradic = 0.032f;

    if (light->id >= 0) {
        light_position [INDEX_POSITION] = light->id + '0'; 
        light_ambient  [INDEX_POSITION] = light->id + '0'; 
        light_diffuse  [INDEX_POSITION] = light->id + '0'; 
        light_specular [INDEX_POSITION] = light->id + '0'; 
        light_constant [INDEX_POSITION] = light->id + '0'; 
        light_linear   [INDEX_POSITION] = light->id + '0'; 
        light_quadradic[INDEX_POSITION] = light->id + '0'; 
    }

    return light;
}

void point_light_exit(PointLight* light) {
    light_exit(light->light_base);
    free(light);
}

void point_light_update(
    PointLight* light, 
    const unsigned int shader_id, 
    vec3 light_pos, 
    vec3 camera_pos 
) {    
    shader_update_uniform3f(shader_id, light_position, light_pos[0], light_pos[1], light_pos[2]);

    shader_update_uniform3f(
        shader_id, 
        light_ambient, 
        light->light_base->ambient[0], 
        light->light_base->ambient[1], 
        light->light_base->ambient[2]);

    shader_update_uniform3f(
        shader_id, 
        light_diffuse, 
        light->light_base->diffuse[0], 
        light->light_base->diffuse[1], 
        light->light_base->diffuse[2]);

    shader_update_uniform3f(
        shader_id, 
        light_specular, 
        light->light_base->specular[0], 
        light->light_base->specular[1], 
        light->light_base->specular[2]);

    shader_update_float(shader_id, light_constant, light->constant);
    shader_update_float(shader_id, light_linear, light->linear);
    shader_update_float(shader_id, light_quadradic, light->quadradic);
}
