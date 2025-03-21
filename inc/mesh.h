#ifndef MESH_H
#define MESH_H

typedef struct Shader Shader;
typedef struct Buffer Buffer;

typedef struct Mesh{
    struct Vertex* vertices;
    unsigned int vertex_count;
    unsigned int* indices;
    unsigned int indices_count;
    struct Texture* textures;
    unsigned int texture_count;
    Buffer* buffer;
} Mesh;

Mesh* mesh_init(
    struct Vertex* vertices,
    unsigned int num_vertices,
    unsigned int* indices,
    unsigned int indices_count,
    struct Texture* textures,
    unsigned int texture_count);

void mesh_exit(Mesh* mesh);
void mesh_draw(Shader* shader, Mesh* mesh);

#endif
