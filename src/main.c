#include "camera.h"
#include "cube.h"
#include "game_objects/point_light_go.h"
#include "lighting/direction_light.h"
#include "lighting/point_light.h"
#include "lighting/spot_light.h"
#include "material.h"
#include "mouse.h"
#include "shader.h"
#include "model.h"
#include "time.h"
#include "keyboard.h"
#include "graphics.h"
#include "transform.h"

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>

vec3 ambient = {0.2f, 0.2f, 0.2f};
vec3 diffuse = {0.8f, 0.8f, 0.8f};
vec3 specular = {1.0f, 1.0f, 1.0f};

bool main() {
    if (graphics_init() == false && window == 0) {
        return false;
    }

    Camera* camera = camera_init_default();
    Cube* cube = cube_init();
    DirectionLight* direction_light = direction_light_init((vec3) { 0.0f, 0.0f, 0.0f });
    PointLightGO* point_lights[4];
    point_lights[0] = point_light_go_init(0);
    point_lights[0]->transform->position[0] = 0.7f;
    point_lights[0]->transform->position[1] = 0.2f;
    point_lights[0]->transform->position[2] = 1.5f;

    point_lights[1] = point_light_go_init(1);
    point_lights[1]->transform->position[0] = 2.3f;
    point_lights[1]->transform->position[1] = -3.3f;
    point_lights[1]->transform->position[2] = -4.0f;

    point_lights[2] = point_light_go_init(2);
    point_lights[2]->transform->position[0] = -4.0f;
    point_lights[2]->transform->position[1] = 2.0f;
    point_lights[2]->transform->position[2] = -12.0f;

    point_lights[3] = point_light_go_init(3);
    point_lights[3]->transform->position[0] = 0.0f;
    point_lights[3]->transform->position[1] = 0.0f;
    point_lights[3]->transform->position[2] = -3.0f;

    SpotLight* spot_light = spot_light_init();

    float deltaTime = 0.0f;

    while (!graphics_window_should_close()) {
        deltaTime = get_delta_time();

        process_input(window, camera, deltaTime);
        process_mouse(window, camera);

        camera_clear(camera);
        camera_update(camera, cube->shader->id, cube->transform);

        direction_light_update(direction_light, cube->shader->id);
        for (int i = 0; i < 4; i++) {
            point_light_update(
                point_lights[i]->point_light_data,
                cube->shader->id, 
                point_lights[i]->transform->position, 
                camera->position);
        }
        spot_light_update(spot_light, cube->shader->id, camera->position, camera->front);
        material_texture_update(cube->material_texture, cube->shader->id);

        model_update(cube->shader->id, cube->transform);
        cube_draw(cube);

        // this shows the point light working a little better. seems like it's fairly weak with the
        // tutorial settings
        point_lights[0]->transform->position[0] = sin(glfwGetTime());
        for (int i = 0; i < 4; i++) {
            camera_update(camera, point_lights[i]->shader->id, point_lights[i]->transform);
            model_update(point_lights[i]->shader->id, point_lights[i]->transform);
            point_light_go_draw(point_lights[i]);
        }

        graphics_swap_buffers(window);
        graphics_poll_events();
    }

    cube_exit(cube);
    for (int i = 0; i < 4; i++) {
        point_light_go_exit(point_lights[i]);
    }
    camera_exit(camera);

    graphics_exit();

    return true;
}

