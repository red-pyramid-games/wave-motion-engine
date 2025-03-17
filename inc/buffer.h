#ifndef BUFFER_H
#define BUFFER_H

typedef struct Vertex Vertex;

typedef struct Buffer {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
} Buffer;

Buffer* buffer_position_init(
    const Vertex* vertices, 
    const unsigned int num_vertices, 
    const unsigned int* indices,
    const unsigned int num_indices);

// Position Normal buffer
Buffer* buffer_pn_init(
    const Vertex* vertices, 
    const unsigned int num_vertices, 
    const unsigned int* indices,
    const unsigned int num_indices);

// Position Normal Texture buffer
Buffer* buffer_pnt_init();
void buffer_exit(Buffer* buffer);

static Buffer* buffer_init(
    const Vertex* vertices, 
    const unsigned int num_vertices, 
    const unsigned int* indices,
    const unsigned int num_indices);

#endif
