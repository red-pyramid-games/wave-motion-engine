#include "material.h"

#include "shader.h"

#include <cglm/vec3.h>
#include <stdio.h>
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

MaterialTexture* material_texture_init_default() {
    MaterialTexture* material = malloc(sizeof(MaterialTexture));

    material->diffuse = 0;
    material->specular = 1;
    material->shininess = 32.0f;

    return material;
}

void material_texture_update(MaterialTexture* material, const unsigned int shader_id) {
    shader_update_int(shader_id, "material.diffuse", material->diffuse);
    shader_update_int(shader_id, "material.specular", material->specular);
    shader_update_float(shader_id, "material.shininess", material->shininess);
}
