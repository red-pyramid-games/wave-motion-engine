#include "buffer.h"

#include "vertex.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 

Buffer* buffer_position_init(
    const Vertex* vertices, 
    const unsigned int num_vertices, 
    const unsigned int* indices,
    const unsigned int num_indices
) {
    Buffer* buffer = malloc(sizeof(Buffer));

    glGenVertexArrays(1, &buffer->vao);
    glGenBuffers(1, &buffer->vbo);
    glGenBuffers(1, &buffer->ebo);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, 
        sizeof(Vertex) * num_vertices, 
        &vertices[0], 
        GL_STATIC_DRAW);

    glBindVertexArray(buffer->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(unsigned int) * num_indices, 
        &indices[0], 
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(
        0, // index 
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(Vertex), //stride
        (void*)offsetof(Vertex, position) // pointer
    );
    glEnableVertexAttribArray(0);

    return buffer;
}

Buffer* buffer_pn_init(
    const Vertex* vertices, 
    const unsigned int num_vertices, 
    const unsigned int* indices,
    const unsigned int num_indices
) {
    Buffer* buffer = malloc(sizeof(Buffer));

    glGenVertexArrays(1, &buffer->vao);
    glGenBuffers(1, &buffer->vbo);
    glGenBuffers(1, &buffer->ebo);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, 
        sizeof(Vertex) * num_vertices, 
        &vertices[0], 
        GL_STATIC_DRAW);

    glBindVertexArray(buffer->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(unsigned int) * num_indices, 
        &indices[0], 
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(
        0, // index 
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(Vertex), //stride
        (void*)offsetof(Vertex, position) // pointer
    );
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(
        1, // index 
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(Vertex), //stride
        (void*)offsetof(Vertex, normal) // pointer
    );
    glEnableVertexAttribArray(1);
    
    return buffer;
}

Buffer* buffer_pntexture_init(
    const Vertex* vertices, 
    const unsigned int num_vertices, 
    const unsigned int* indices,
    const unsigned int num_indices
) {
    Buffer* buffer = malloc(sizeof(Buffer));
    
    glGenVertexArrays(1, &buffer->vao);
    glGenBuffers(1, &buffer->vbo);
    glGenBuffers(1, &buffer->ebo);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, 
        sizeof(Vertex) * num_vertices, 
        &vertices[0], 
        GL_STATIC_DRAW);

    glBindVertexArray(buffer->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(unsigned int) * num_indices, 
        &indices[0], 
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(
        0, // index 
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(Vertex), //stride
        (void*)offsetof(Vertex, position) // pointer
    );
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(
        1, // index 
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(Vertex), //stride
        (void*)offsetof(Vertex, normal) // pointer
    );
    glEnableVertexAttribArray(1);
    
    // texture coords attribute
    glVertexAttribPointer(
        2, // index 
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized
        sizeof(Vertex), //stride
        (void*)offsetof(Vertex, tex_coords) // pointer
    );
    glEnableVertexAttribArray(2);
    return buffer;
}

void buffer_exit(Buffer* buffer) {
    if (buffer == NULL) {
        return;
    }

    glDeleteVertexArrays(1, &buffer->vao);
    glDeleteBuffers(1, &buffer->vbo);
    glDeleteBuffers(1, &buffer->ebo);

    free(buffer);
}

