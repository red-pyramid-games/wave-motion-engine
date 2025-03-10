#include "basic_buffer.h"

#include <glad/glad.h>

#include <stdlib.h>

BasicBuffer* basic_buffer_init(const float* vertices, const unsigned int* indices) {
    BasicBuffer* buffer = malloc(sizeof(BasicBuffer));

    glGenVertexArrays(1, &buffer->vao);
    glGenBuffers(1, &buffer->vbo);
    glGenBuffers(1, &buffer->ebo);

    glBindVertexArray(buffer->vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* Keep these for when you need to get them for other buffers  */
    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    // texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    return buffer;
}

void basic_buffer_exit(BasicBuffer *buffer) {
    if (buffer == NULL) {
        return;
    }

    free(buffer);
}
