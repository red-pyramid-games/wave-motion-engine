#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

typedef struct Camera Camera;

typedef struct PointLight {
    struct Model* model;
    struct Shader* shader;
    struct Buffer* buffer;
    struct Transform* transform;
} PointLight;

PointLight* point_light_init();
void point_light_exit(PointLight* point_light);

void point_light_draw(PointLight* point_light);

#endif 
