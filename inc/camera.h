#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/types.h>

typedef enum CameraType {
    ORTHOGRAPHIC,
    PERSPECTIVE, 
} CameraType;

typedef struct Camera {
    CameraType cam_projection;
    vec3 background_color;
    vec2 screen_size;
    vec3 position;
    float zoom;
    vec2 clipping_plane;
    unsigned int shader_id;
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
void camera_clear(vec3 clear_color);

static void camera_update_view(Camera* camera);
static void camera_update_projection(const unsigned int id, mat4 projection);

#endif
