#ifndef BASIC_BUFFER_H 
#define BASIC_BUFFER_H 

typedef struct BasicBuffer {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
} BasicBuffer;

BasicBuffer* basic_buffer_init(const float* vertices, const unsigned int* indices);
void basic_buffer_exit(BasicBuffer* buffers);

#endif
