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

static void editor_render_top_menu(struct nk_context *ctx);
static void editor_render_components(struct nk_context* ctx);

#endif
