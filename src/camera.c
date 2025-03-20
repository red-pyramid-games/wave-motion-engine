#include "camera.h"

#include "graphics.h"

#include <glad/glad.h>
#include <cglm/affine-pre.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

#define vec2_zero (vec2) { 0.0f, 0.0f }

Camera* camera_init_default() {
    Camera* camera = malloc(sizeof(Camera));

    memcpy(camera->position, (vec3) { 0.0f, 0.0f, 10.0f }, sizeof(vec3)); 
    //memcpy(camera->background_color, (vec4) {0.149f, 0.302f, 0.557f, 1.0f}, sizeof(vec3));
    memcpy(camera->background_color, (vec4) {0.1f, 0.1f, 0.1f, 1.0f}, sizeof(vec3));
    memcpy(camera->clipping_plane, (vec2) { 0.1f, 100.0f }, sizeof(vec2));
    memcpy(camera->screen_size, (vec2) { DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT }, sizeof(vec2));

    camera->zoom = 1.0f;

    vec3 cam_front = { 0.0f, 0.0f, -1.0f };
    memcpy(camera->front, cam_front, sizeof(vec3));
    vec3 cam_up = { 0.0f, 1.0f, 0.0f };
    memcpy(camera->up, cam_up, sizeof(vec3));

    glm_perspective(
        glm_rad(45.0f), 
        camera->screen_size[0] / camera->screen_size[1], 
        0.1f, 
        100.0f, 
        camera->projection_matrix);

    return camera;
}

void camera_exit(Camera* camera) {
    if (camera == NULL) {
        return;
    }

    free(camera);
}

void camera_clear(Camera* camera) {
    glClearColor(
        camera->background_color[0], 
        camera->background_color[1], 
        camera->background_color[2], 
        camera->background_color[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void camera_update_model(Camera* camera, const unsigned int shader_id, vec3 position) {
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, position);
    shader_update_uniform4fv(shader_id, "model", model);
} 

void camera_update_view(Camera* camera,const unsigned int shader_id) {
    vec3 cam_center;
    glm_vec3_add(camera->position, camera->front, cam_center);
    glm_lookat(camera->position, cam_center, camera->up, camera->view_matrix);
    shader_update_uniform4fv(shader_id, "view", camera->view_matrix);        
}

void camera_update_projection(Camera* camera, const unsigned int shader_id) {
    glm_perspective(
        camera->zoom, 
        camera->screen_size[0] / camera->screen_size[1], 
        camera->clipping_plane[0], 
        camera->clipping_plane[1], 
        camera->projection_matrix);
    shader_update_uniform4fv(shader_id, "projection", camera->projection_matrix);
}

void camera_update(Camera* camera, const unsigned int shader_id, Transform* transform) {
    shader_use_program(shader_id);
    shader_update_uniform4fv(shader_id, "projection", camera->projection_matrix);
    shader_update_uniform4fv(shader_id, "view", camera->view_matrix);
    camera_update_view(camera, shader_id);
}

