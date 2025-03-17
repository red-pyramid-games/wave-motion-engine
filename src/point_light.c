#include "point_light.h"

#include "buffer.h"
#include "model.h"
#include "mesh.h"
#include "shader.h"
#include "transform.h"

#include <cglm/affine.h>
#include <glad/glad.h>
#include <stdlib.h>

PointLight* point_light_init() {
    PointLight* point_light = malloc(sizeof(PointLight));

    point_light->model = model_init("../resources/models/sphere.obj");
    for (int i = 0; i < point_light->model->mesh_count; i++) {
        point_light->model->meshes[i].buffer = buffer_position_init(
            point_light->model->meshes[i].vertices,
            point_light->model->meshes[i].vertex_count,
            point_light->model->meshes[i].indices,
            point_light->model->meshes[i].indices_count);
    }

    point_light->shader = shader_init(
        "../resources/shaders/light_source_vs.glsl", 
        "../resources/shaders/light_source_fs.glsl");

    point_light->transform = transform_init_default();

    glm_vec3_copy((vec3){1.2f, 1.0f, 2.0f}, point_light->transform->position);

    return point_light;
}

void point_light_exit(PointLight* point_light) {
    free(point_light);
}

void point_light_draw(PointLight* point_light) {
    for (int i = 0; i < point_light->model->mesh_count; i++) {
        glBindVertexArray(point_light->model->meshes[i].buffer->vao);

        glDrawElements(
            GL_TRIANGLES, 
            point_light->model->meshes[i].indices_count, 
            GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}
