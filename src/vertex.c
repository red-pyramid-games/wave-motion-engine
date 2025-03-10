#include "vertex.h"

#include <stdlib.h>

Vertex* vertex_init() {
    return NULL;
}

void vertex_exit(Vertex* vertex) {
    if (vertex == NULL) {
        return;
    }

    free(vertex);
}
