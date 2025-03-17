#include "material.h"

#include "shader.h"

#include <cglm/vec3.h>
#include <stdlib.h>

Material* material_init_default() {
    Material* material = malloc(sizeof(Material));

    material->color[0] = 0.5f;
    material->color[1] = 0.5f;
    material->color[2] = 0.5f;

    glm_vec3_one(material->ambient);
    glm_vec3_one(material->diffuse);

    material->specular[0] = 0.5f;
    material->specular[1] = 0.5f;
    material->specular[2] = 0.5f;

    material->shine = 32.0f;

    return material;
}

void material_exit(Material* material) {
    free(material);
}

void material_update(const Material *material, const unsigned int shader_id) {
    shader_update_uniform3f(
        shader_id, 
        "material.ambient", 
        material->ambient[0],
        material->ambient[1],
        material->ambient[2]);

    shader_update_uniform3f(
        shader_id, 
        "material.diffuse", 
        material->diffuse[0],
        material->diffuse[1],
        material->diffuse[2]);

    shader_update_uniform3f(
        shader_id, 
        "material.specular", 
        material->specular[0], 
        material->specular[1], 
        material->specular[2]); 

    shader_update_float(shader_id, "material.shininess", material->shine);
}

void obsolete_material_update(
    const unsigned int shader_id, 
    vec3 object_pos, 
    vec3 light_pos, 
    vec3 light_color, 
    vec3 camera_pos
) {
    shader_update_uniform3f(shader_id, "light.position", light_pos[0], light_pos[1], light_pos[2]);
    shader_update_uniform3f(shader_id, "viewPos", camera_pos[0], camera_pos[1], camera_pos[2]);

    // light properties
    vec3 diffuseColor; // decrease the influence
    glm_vec3_scale(light_color, 0.75f, diffuseColor);
    vec3 ambientColor;
    glm_vec3_scale(diffuseColor, 1.0f, ambientColor); // low influence
    shader_update_uniform3f(shader_id, "light.ambient", ambientColor[0], ambientColor[1], ambientColor[2]);
    shader_update_uniform3f(shader_id, "light.diffuse", diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    shader_update_uniform3f(shader_id, "light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    shader_update_uniform3f(shader_id, "material.ambient", 1.0f, 1.0f, 1.0f);
    shader_update_uniform3f(shader_id, "material.diffuse", 1.0f, 1.0f, 1.0f);
    shader_update_uniform3f(shader_id, "material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    shader_update_float(shader_id, "material.shininess", 32.0f);
}
