#ifndef EDITOR_H
#define EDITOR_H

typedef struct GLFWwindow GLFWwindow;
typedef struct Camera Camera;

typedef struct Editor {
    struct nk_glfw* glfw;
    struct nk_context* ctx;
} Editor;

Editor* editor_init(struct GLFWwindow* window);
void editor_exit(Editor* editor);
void editor_render(Editor* editor, Camera* camera);

void editor_render_menu_bar(Editor* editor, Camera* camera);
void editor_render_camera_details(Editor* editor, Camera* camera);
void editor_render_component_details(struct nk_context* ctx);
void editor_render_transform_component(struct nk_context* ctx);
void editor_render_sprite_component(struct nk_context* ctx);

#endif
