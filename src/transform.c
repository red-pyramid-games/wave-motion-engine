#include "transform.h"

#include <cglm/vec3.h>
#include <stdlib.h>

Transform* transform_init_default() {
    Transform* transform = malloc(sizeof(Transform));
    glm_vec3_zero(transform->position);
    glm_vec3_zero(transform->rotation);
    glm_vec3_one(transform->scale);

    return transform;
}

void transform_exit(Transform* transform) {
    if (transform == NULL) {
        return;
    }

    free(transform);
}
