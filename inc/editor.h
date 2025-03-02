#ifndef EDITOR_H
#define EDITOR_H

typedef struct GLFWwindow GLFWwindow;

typedef struct Editor {
    struct nk_glfw* glfw;
    struct nk_context* ctx;
} Editor;

Editor* editor_init(GLFWwindow* window);
void editor_exit(Editor* editor);
void editor_render(Editor* editor);

void editor_render_component_details(struct nk_context* ctx);
void editor_render_transform_component(struct nk_context* ctx);
void editor_render_sprite_component(struct nk_context* ctx);

#endif
