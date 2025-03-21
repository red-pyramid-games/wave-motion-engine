#ifndef POINT_LIGHT_GO_H
#define POINT_LIGHT_GO_H

typedef struct PointLightGO {
    struct Model* model;
    struct Shader* shader;
    struct Buffer* buffer;
    struct Transform* transform;
    struct PointLight* point_light_data;
} PointLightGO;

PointLightGO* point_light_go_init(const unsigned int idx);
void point_light_go_exit(PointLightGO* point_light_go);

void point_light_go_draw(PointLightGO* point_light_go);

#endif
