#include "cube.h"

#include "buffer.h"
#include "light.h"
#include "mesh.h"
#include "material.h"
#include "model.h"
#include "shader.h"
#include "transform.h"
#include "texture.h"    

#include <cglm/vec3.h>
#include <glad/glad.h>
#include <stdlib.h>

Cube* cube_init() {
    Cube* cube = malloc(sizeof(Cube));

    cube->model = model_init("../resources/models/cube.obj");
    for (int i = 0; i < cube->model->mesh_count; i++) {
        cube->model->meshes[i].buffer = buffer_pntexture_init(
            cube->model->meshes[i].vertices,
            cube->model->meshes[i].vertex_count,
            cube->model->meshes[i].indices,
            cube->model->meshes[i].indices_count);
    }

    cube->shader = shader_init(
        "../resources/shaders/lighting_dps_vs.glsl", 
        "../resources/shaders/lighting_dps_fs.glsl");

    cube->transform = transform_init_default();

    glm_vec3_copy((vec3){0.5f, 0.5f, 0.5f}, cube->color);

    cube->material = material_init_default();
    cube->material_texture = material_texture_init_default();
    cube->light = light_init(cube->color);

    return cube;
}

void cube_exit(Cube* cube) {
    free (cube);
}

void cube_draw(Cube* cube) {
    for (int i = 0; i < cube->model->mesh_count; i++) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube->model->meshes[i].textures[0].id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cube->model->meshes[i].textures[1].id);
        glBindVertexArray(cube->model->meshes[i].buffer->vao);

        glDrawElements(
            GL_TRIANGLES, 
            cube->model->meshes[i].indices_count, 
            GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}
