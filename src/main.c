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

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>

bool main() {
    if (graphics_init() == false && window == 0) {
        return false;
    }

    Camera* camera = camera_init_default();
    Cube* new_cube = cube_init();
    PointLight* point_lights[4];
    point_lights[0] = point_light_init();
    point_lights[0]->transform->position[0] = 0.7f;
    point_lights[0]->transform->position[1] = 0.2f;
    point_lights[0]->transform->position[2] = 1.5f;

    point_lights[1] = point_light_init();
    point_lights[1]->transform->position[0] = 2.3f;
    point_lights[1]->transform->position[1] = -3.3f;
    point_lights[1]->transform->position[2] = -4.0f;

    point_lights[2] = point_light_init();
    point_lights[2]->transform->position[0] = -4.0f;
    point_lights[2]->transform->position[1] = 2.0f;
    point_lights[2]->transform->position[2] = -12.0f;

    point_lights[3] = point_light_init();
    point_lights[3]->transform->position[0] = 0.0f;
    point_lights[3]->transform->position[1] = 0.0f;
    point_lights[3]->transform->position[2] = -3.0f;

    float deltaTime = 0.0f;

    while (!graphics_window_should_close()) {
        deltaTime = get_delta_time();

        process_input(window, camera, deltaTime);
        process_mouse(window, camera);

        camera_clear(camera);

        camera_update(camera, new_cube->shader->id, new_cube->transform);
        shader_update_uniform3f(new_cube->shader->id, "viewPos", camera->position[0], camera->position[1], camera->position[2]);
        direction_light_update(0, new_cube->shader->id);
        for (int i = 0; i < 4; i++) {
            point_light_update(
                    new_cube->light, 
                    new_cube->shader->id, 
                    point_lights[i]->transform->position, 
                    camera->position,
                    i);
        }
        spot_light_update(0, new_cube->shader->id, camera->position, camera->front);
        //material_update(new_cube->material, new_cube->shader->id);
        material_texture_update(new_cube->material_texture, new_cube->shader->id);
        // this shows the point light working a little better. seems like it's fairly weak with the
        // tutorial settings
        point_lights[0]->transform->position[0] = sin(glfwGetTime());
        model_update(new_cube->shader->id, new_cube->transform);
        cube_draw(new_cube);

        for (int i = 0; i < 4; i++) {
            camera_update(camera, point_lights[i]->shader->id, point_lights[i]->transform);
            model_update(point_lights[i]->shader->id, point_lights[i]->transform);
            point_light_draw(point_lights[i]);
        }

        graphics_swap_buffers(window);
        graphics_poll_events();
    }

    cube_exit(new_cube);
        for (int i = 0; i < 4; i++) {
            point_light_exit(point_lights[i]);
        }
    camera_exit(camera);

    graphics_exit();

    return true;
}

