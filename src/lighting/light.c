#include "lighting/light.h"

#include <cglm/vec3.h>
#include <stdlib.h>

Light* light_init(vec3 ambient, vec3 diffuse, vec3 specular) {
    Light* light = malloc(sizeof(Light));
    glm_vec3_copy(ambient, light->ambient);
    glm_vec3_copy(diffuse, light->diffuse);
    glm_vec3_copy(specular, light->specular);
    return light;
}

void light_exit(Light* light) {
    free(light);
}

