#ifndef MATERIAL_H
#define MATERIAL_H

#include <cglm/types.h>

typedef struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
} Material;

Material* material_init_default();
void material_exit(Material* material);

void material_update(const Material* material, const unsigned int shader_id);

#endif
