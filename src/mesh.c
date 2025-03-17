#include "mesh.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "buffer.h"
#include "shader.h"
#include "texture.h"    
#include "vertex.h"

Mesh* mesh_init(
    struct Vertex* vertices,
    unsigned int num_vertices,
    unsigned int* indices,
    unsigned int indices_count,
    struct Texture* textures,
    unsigned int texture_count
) {
    Mesh* mesh = malloc(sizeof(Mesh));
    mesh->vertices = malloc(sizeof(Vertex) * num_vertices);
    mesh->vertex_count = num_vertices;
    memcpy(mesh->vertices, vertices, sizeof(Vertex) * num_vertices);
    mesh->indices = malloc(sizeof(unsigned int) * indices_count);
    mesh->indices_count = indices_count;
    memcpy(mesh->indices, indices, sizeof(unsigned int) * indices_count);

    //mesh->textures = malloc(sizeof(Texture) * texture_count);
    //mesh->texture_count = texture_count;
    //memcpy(mesh->textures, textures, sizeof(Texture) * texture_count);

    //glGenVertexArrays(1, &mesh->vao);
    //glGenBuffers(1, &mesh->vbo);
    //glGenBuffers(1, &mesh->ebo);

    //glBindVertexArray(mesh->vao);

    //glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    //glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count, mesh->vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_count, mesh->indices, GL_STATIC_DRAW);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //glEnableVertexAttribArray(1);
    // texture coord attribute
    
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    //glEnableVertexAttribArray(2);

    return mesh;
}

void mesh_exit(Mesh* mesh) {
    if (mesh == NULL) {
        return;
    }
    vertex_exit(mesh->vertices);
    texture_exit(mesh->textures);

    free(mesh->indices);
    free(mesh);
}

void mesh_draw(Shader* shader, Mesh* mesh) {
    //unsigned int diffuse_nr = 1;
    //unsigned int specular_nr = 1;
    //for (int i = 0; i < mesh->texture_count; i++) {
    //    glActiveTexture(GL_TEXTURE0 + i);
    //    char* number = malloc(sizeof(char) * 8);
    //    char* name = mesh->textures[i].type;
    //    if (strcmp(name, "texture_diffuse") == 0) {
    //        sprintf(number, "%d", diffuse_nr);
    //    }
    //    else if (strcmp(name, "texture_specular") == 0) {
    //        sprintf(number, "%d", specular_nr);
    //    }

    //    char* material = malloc(sizeof(char) * 9 * strlen(number));
    //    strcat(material, "material.");
    //    strcat(material, number);
    //    shader_update_int(shader->id, material, i);
    //    glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    //}
    //glActiveTexture(GL_TEXTURE0);

    //glBindVertexArray(mesh->vao);
    //glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}
