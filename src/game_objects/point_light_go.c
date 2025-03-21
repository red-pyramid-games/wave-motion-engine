#include "game_objects/point_light_go.h"

#include "lighting/point_light.h"

#include "buffer.h"
#include "model.h"
#include "mesh.h"
#include "shader.h"
#include "transform.h"

#include <cglm/affine.h>
#include <glad/glad.h>
#include <stdlib.h>

PointLightGO* point_light_go_init(const unsigned int idx) {
    PointLightGO* point_light_go = malloc(sizeof(PointLightGO));

    point_light_go->model = model_init("../resources/models/sphere.obj");
    for (int i = 0; i < point_light_go->model->mesh_count; i++) {
        point_light_go->model->meshes[i].buffer = buffer_position_init(
            point_light_go->model->meshes[i].vertices,
            point_light_go->model->meshes[i].vertex_count,
            point_light_go->model->meshes[i].indices,
            point_light_go->model->meshes[i].indices_count);
    }

    point_light_go->shader = shader_init(
        "../resources/shaders/light_source_vs.glsl", 
        "../resources/shaders/light_source_fs.glsl");

    point_light_go->transform = transform_init_default();

    vec3 ambient = {0.2f, 0.2f, 0.2f};
    vec3 diffuse = {0.8f, 0.8f, 0.8f};
    vec3 specular = {1.0f, 1.0f, 1.0f};
    float constant = 1.0f;
    float linear = 0.09f;
    float quadradic = 0.032f;

    point_light_go->point_light_data = point_light_init(idx, ambient, diffuse, specular);

    glm_vec3_copy((vec3){1.2f, 1.0f, 2.0f}, point_light_go->transform->position);

    return point_light_go;
}

void point_light_go_exit(PointLightGO* point_light_go) {
    free(point_light_go);
}

void point_light_go_draw(PointLightGO* point_light_go) {
    for (int i = 0; i < point_light_go->model->mesh_count; i++) {
        glBindVertexArray(point_light_go->model->meshes[i].buffer->vao);

        glDrawElements(
            GL_TRIANGLES, 
            point_light_go->model->meshes[i].indices_count, 
            GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}
