#include "transform.h"

#include <stdlib.h>
#include <string.h>

Transform* transform_init_default() {
    Transform* transform = malloc(sizeof(Transform));
    memcpy(transform->position, (vec3) {0.0f, 0.0f, 0.0f}, sizeof(vec3));

    return transform;
}

void transform_exit(Transform* transform) {
    if (transform == NULL) {
        return;
    }

    free(transform);
}
