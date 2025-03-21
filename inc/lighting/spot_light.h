#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

typedef struct SpotLight {
    struct PointLight* light_base;

    float inner_angle;
    float outter_angle;
} SpotLight;

SpotLight* spot_light_init();
void spot_light_exit(SpotLight* spot_light);

void spot_light_update(
    SpotLight *light, 
    const unsigned int shader_id, 
    float *camera_pos, 
    float *camera_front);

#endif
