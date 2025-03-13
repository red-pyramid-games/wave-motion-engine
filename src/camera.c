#include "camera.h"

#include <glad/glad.h>
#include <cglm/affine-pre.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

#define vec2_zero (vec2) { 0.0f, 0.0f }

static const float DEFAULT_SCREEN_WIDTH = 1920.0f; 
static const float DEFAULT_SCREEN_HEIGHT = 1080.0f;

Camera* camera_init_default() {
    Shader* camera_shader = shader_init(
        "../resources/shaders/ortho_basic_vs.glsl",
        "../resources/shaders/ortho_basic_fs.glsl");
    if (camera_shader == NULL) {
        printf("Error creating shader for camera\n");
        return NULL;
    }

    Camera* camera = malloc(sizeof(Camera));

    memcpy(camera->position, (vec3) { 0.0f, 0.0f, 3.0f }, sizeof(vec3)); 
    memcpy(camera->background_color, (vec4) {1.0f, 1.0f, 1.0f, 1.0f}, sizeof(vec3));
    memcpy(camera->clipping_plane, (vec2) { 0.1f, 100.0f }, sizeof(vec2));
    memcpy(camera->screen_size, (vec2) { DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT }, sizeof(vec2));

    camera->cam_projection = PERSPECTIVE;
    camera->zoom = 1.0f;
    camera->shader_id = camera_shader->id;

    vec3 cam_front = { 0.0f, 0.0f, -1.0f };
    memcpy(camera->front, cam_front, sizeof(vec3));
    vec3 cam_up = { 0.0f, 1.0f, 0.0f };
    memcpy(camera->up, cam_up, sizeof(vec3));

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3) {0.0f, 0.0f, 0.0f});
    shader_update_uniform4fv(camera->shader_id, "model", model);

    mat4 view;
    glm_mat4_identity(view);
    memcpy(camera->view_matrix, view, sizeof(mat4));
    camera_update_view(camera);

    mat4 projection;
    glm_mat4_identity(projection);
    memcpy(camera->projection_matrix, projection, sizeof(mat4));

    camera_perspective(camera);

    return camera;
}

void camera_exit(Camera* camera) {
    if (camera == NULL) {
        return;
    }

    free(camera);
}

void camera_ortho(Camera* camera) {
    glm_ortho(0, camera->screen_size[0], 0, camera->screen_size[1], 0, 1, camera->projection_matrix);
    camera_update_projection(camera);
}

void camera_perspective(Camera* camera) {
    glm_perspective(
        camera->zoom, 
        camera->screen_size[0] / camera->screen_size[1], 
        camera->clipping_plane[0], 
        camera->clipping_plane[1], 
        camera->projection_matrix);
    camera_update_projection(camera);
}

void camera_clear(Camera* camera) {
    glClearColor(
        camera->background_color[0], 
        camera->background_color[1], 
        camera->background_color[2], 
        camera->background_color[3]);

    glClear(GL_COLOR_BUFFER_BIT);
}

void camera_update_model(Camera* camera, vec3 position) {
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, position);
    shader_update_uniform4fv(camera->shader_id, "model", model);
} 

void camera_update_view(Camera* camera) {
    vec3 cam_center;
    glm_vec3_add(camera->position, camera->front, cam_center);
    glm_lookat(camera->position, cam_center, camera->up, camera->view_matrix);
    shader_update_uniform4fv(camera->shader_id, "view", camera->view_matrix);        
}

void camera_update_projection(Camera* camera) {
    shader_use_program(camera->shader_id);
    shader_update_uniform4fv(camera->shader_id, "projection", camera->projection_matrix);
}

