#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/types.h>

typedef struct Transform Transform;

typedef struct Camera {
    vec4 background_color;
    vec2 screen_size;
    vec3 position;
    float zoom;
    vec2 clipping_plane;
    mat4 model_matrix;
    mat4 view_matrix;
    mat4 projection_matrix;
    vec3 front;
    vec3 up;
} Camera;

Camera* camera_init_default();
void camera_exit(Camera* camera);
void camera_perspective(Camera* camera);
void camera_ortho(Camera* camera);
void camera_clear(Camera* camera);

void camera_update(Camera* camera, const unsigned int shader_id, Transform* transform);
void camera_update_model(Camera* camera, const unsigned int shader_id, vec3 position);
void camera_update_view(Camera* camera, const unsigned int shader_id);
void camera_update_projection(Camera* camera, const unsigned int shader_id);

#endif
