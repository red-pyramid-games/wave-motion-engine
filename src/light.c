#include "light.h"
#include "shader.h"

#include <cglm/vec3.h>
#include <math.h>
#include <stdio.h>

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

void direction_light_update(Light *light, const unsigned int shader_id) {
    shader_update_uniform3f(shader_id, "dirLight.direction", -0.2f, -1.0f, -0.3f); 
    shader_update_uniform3f(shader_id, "dirLight.ambient", 0.05f, 0.05f, 0.05f); 
    shader_update_uniform3f(shader_id, "dirLight.diffuse", 0.4f, 0.4f, 0.4f); 
    shader_update_uniform3f(shader_id, "dirLight.specular", 0.5f, 0.5f, 0.5f); 
}

void point_light_update(Light* light, const unsigned int shader_id, vec3 light_pos, vec3 camera_pos, const unsigned int idx) {    
    vec3 ambient = {0.2f, 0.2f, 0.2f};
    vec3 diffuse = {0.8f, 0.8f, 0.8f};
    vec3 specular = {1.0f, 1.0f, 1.0f};
    float constant = 1.0f;
    float linear = 0.09f;
    float quadradic = 0.032f;

    char* light_position = "";
    char* light_ambient = "";
    char* light_diffuse = "";
    char* light_specular = "";
    char* light_constant = "";
    char* light_linear = "";
    char* light_quadradic = "";

    switch (idx) {
        case 0:
            light_position = "pointLights[0].position";
            light_ambient  = "pointLights[0].ambient";
            light_diffuse  = "pointLights[0].diffuse";
            light_specular = "pointLights[0].specular";
            light_constant = "pointLights[0].constant";
            light_linear   = "pointLights[0].linear";
            light_quadradic= "pointLights[0].quadratic";
            break;
        case 1:
            light_position  = "pointLights[1].position";
            light_ambient   = "pointLights[1].ambient";
            light_diffuse   = "pointLights[1].diffuse";
            light_specular  = "pointLights[1].specular";
            light_constant  = "pointLights[1].constant";
            light_linear    = "pointLights[1].linear";
            light_quadradic = "pointLights[1].quadratic";
            break;
        case 2:
            light_position  = "pointLights[2].position";
            light_ambient   = "pointLights[2].ambient";
            light_diffuse   = "pointLights[2].diffuse";
            light_specular  = "pointLights[2].specular";
            light_constant  = "pointLights[2].constant";
            light_linear    = "pointLights[2].linear";
            light_quadradic = "pointLights[2].quadratic";
            break;
        case 3:
            light_position  = "pointLights[3].position";
            light_ambient   = "pointLights[3].ambient";
            light_diffuse   = "pointLights[3].diffuse";
            light_specular  = "pointLights[3].specular";
            light_constant  = "pointLights[3].constant";
            light_linear    = "pointLights[3].linear";
            light_quadradic = "pointLights[3].quadratic";
            break;
    }

    shader_update_uniform3f(shader_id, light_position, light_pos[0], light_pos[1], light_pos[2]);
    shader_update_uniform3f(shader_id, light_ambient, ambient[0], ambient[1], ambient[2]);
    shader_update_uniform3f(shader_id, light_diffuse, diffuse[0], diffuse[1], diffuse[2]);
    shader_update_uniform3f(shader_id, light_specular, specular[0], specular[1], specular[2]);
    shader_update_float(shader_id, light_constant, constant);
    shader_update_float(shader_id, light_linear, linear);
    shader_update_float(shader_id, light_quadradic, quadradic);
}

void spot_light_update(Light *light, const unsigned int shader_id, float *camera_pos, float *camera_front) {
    shader_update_uniform3f(shader_id, "spotLight.position", camera_pos[0], camera_pos[1], camera_pos[2]);
    shader_update_uniform3f(shader_id, "spotLight.direction", camera_front[0], camera_front[1], camera_front[2]);
    shader_update_uniform3f(shader_id, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader_update_uniform3f(shader_id, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader_update_uniform3f(shader_id, "spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader_update_float(shader_id, "spotLight.constant", 1.0f);
    shader_update_float(shader_id, "spotLight.linear", 0.09f);
    shader_update_float(shader_id, "spotLight.quadratic", 0.032f);
    shader_update_float(shader_id, "spotLight.cutOff", cos(glm_rad(12.5f)));
    shader_update_float(shader_id, "spotLight.outerCutOff", cos(glm_rad(15.0f)));
}

void light_update(Light* light, const unsigned int shader_id, vec3 light_pos, vec3 camera_pos) {
    shader_update_uniform3f(shader_id, "light.position", light_pos[0], light_pos[1], light_pos[2]);
    shader_update_uniform3f(shader_id, "viewPos", camera_pos[0], camera_pos[1], camera_pos[2]);
    glm_vec3_scale(light->color, light->diffuse_intensity, light->diffuse);
    glm_vec3_scale(light->diffuse, light->ambient_intensity, light->ambient);                                                                               //

    shader_update_uniform3f(
        shader_id, 
        "light.ambient", 
        0.2f, 
        0.2f, 
        0.2f);

    shader_update_uniform3f(
        shader_id, 
        "light.diffuse", 
        0.5f, 
        0.5f, 
        0.5f);

    shader_update_uniform3f(
        shader_id, 
        "light.specular", 
        1.0f, 
        1.0f,
        1.0f);
}
