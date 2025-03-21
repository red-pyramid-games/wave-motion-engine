#include "lighting/spot_light.h"
#include "lighting/point_light.h"
#include "lighting/light.h"

#include "shader.h"

#include <cglm/util.h>
#include <math.h>

SpotLight* spot_light_init() {
    SpotLight* spot_light = malloc(sizeof(SpotLight));

    vec3 ambient = {0.2f, 0.2f, 0.2f};
    vec3 diffuse = {0.8f, 0.8f, 0.8f};
    vec3 specular = {1.0f, 1.0f, 1.0f};
    spot_light->light_base = point_light_init(-1, ambient, diffuse, specular);

    return spot_light;
}

void spot_light_exit(SpotLight* spot_light) {
    point_light_exit(spot_light->light_base);
    free(spot_light);
}

void spot_light_update(
    SpotLight *light, 
    const unsigned int shader_id, 
    float *camera_pos, 
    float *camera_front
) {
    shader_update_uniform3f(
        shader_id, 
        "spotLight.position", 
        camera_pos[0], 
        camera_pos[1], 
        camera_pos[2]);

    shader_update_uniform3f(
        shader_id, 
        "spotLight.direction", 
        camera_front[0], 
        camera_front[1], 
        camera_front[2]);

    shader_update_uniform3f(
        shader_id, 
        "spotLight.ambient", 
        light->light_base->light_base->ambient[0], 
        light->light_base->light_base->ambient[1], 
        light->light_base->light_base->ambient[2]);

    shader_update_uniform3f(
        shader_id, 
        "spotLight.diffuse", 
        light->light_base->light_base->diffuse[0], 
        light->light_base->light_base->diffuse[1], 
        light->light_base->light_base->diffuse[2]);

    shader_update_uniform3f(
        shader_id, 
        "spotLight.specular", 
        light->light_base->light_base->specular[0], 
        light->light_base->light_base->specular[1], 
        light->light_base->light_base->specular[2]);

    shader_update_float(shader_id, "spotLight.constant", light->light_base->constant);
    shader_update_float(shader_id, "spotLight.linear", light->light_base->linear);
    shader_update_float(shader_id, "spotLight.quadratic", light->light_base->quadradic);

    shader_update_float(shader_id, "spotLight.cutOff", cos(glm_rad(light->inner_angle)));
    shader_update_float(shader_id, "spotLight.outerCutOff", cos(glm_rad(light->outter_angle)));
}
