#include "camera.h"

#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

#define vec2_zero (vec2) { 0.0f, 0.0f }

Camera* camera_init(float width, float height, const unsigned int shader_id) {
    Camera* camera = malloc(sizeof(Camera));
    camera->projection = ORTHOGRAPHIC;
    memcpy(camera->position, vec2_zero, sizeof(vec2)); 
    printf("cam position %f %f\n", camera->position[0], camera->position[1]);
    camera->shader_id = shader_id;
    return camera;
}
void camera_ortho(const unsigned int shader_id, const float width, const float height) {
    mat4 projection;
    glm_mat4_identity(projection);
    glm_ortho(0, width, 0, height, 0, 1, projection);
    shader_use_program(shader_id);
    shader_update_uniform4fv(shader_id, "projection", projection);
}
