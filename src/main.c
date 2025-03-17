#include "camera.h"
#include "cube.h"
#include "light.h"
#include "material.h"
#include "mouse.h"
#include "point_light.h"
#include "shader.h"
#include "model.h"
#include "time.h"
#include "transform.h"
#include "keyboard.h"
#include "graphics.h"

#include <stdbool.h>

bool main() {
    if (graphics_init() == false && window == 0) {
        return false;
    }

    Camera* camera = camera_init_default();
    Cube* new_cube = cube_init();
    PointLight* point_light = point_light_init();

    float deltaTime = 0.0f;

    while (!graphics_window_should_close()) {
        deltaTime = get_delta_time();

        process_input(window, camera, deltaTime);
        process_mouse(window, camera);

        camera_clear(camera);

        camera_update(camera, new_cube->shader->id, new_cube->transform);
        light_update(
            new_cube->light, 
            new_cube->shader->id, 
            point_light->transform->position, 
            camera->position);
        material_update(new_cube->material, new_cube->shader->id);
        model_update(new_cube->shader->id, new_cube->transform);
        cube_draw(new_cube);

        camera_update(camera, point_light->shader->id, point_light->transform);
        model_update(point_light->shader->id, point_light->transform);
        point_light_draw(point_light);

        graphics_swap_buffers(window);
        graphics_poll_events();
    }

    cube_exit(new_cube);
    point_light_exit(point_light);
    camera_exit(camera);

    graphics_exit();

    return true;
}

