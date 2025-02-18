#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/types.h>

typedef enum CameraType {
    ORTHOGRAPHIC,
    PERSPECTIVE, 
} CameraType;

typedef struct Camera {
    CameraType projection;
    vec3 background;
    vec2 screen_size;
    vec2 position;
    float zoom;
    vec2 clipping_plane;
    unsigned int shader_id;
} Camera;

Camera* camera_init(float screen_width, float screen_height, const unsigned int shader_id);

#endif
