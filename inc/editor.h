#ifndef EDITOR_H
#define EDITOR_H

typedef struct GLFWwindow GLFWwindow;

typedef struct Editor {
    struct nk_glfw* glfw;
    struct nk_context* ctx;
    struct nk_colorf* bg;
} Editor;

Editor* editor_init(GLFWwindow* window);
void editor_exit(Editor* editor);
void editor_clear(Editor* editor);
void editor_render(Editor* editor);

static void editor_render_background_edit(Editor* editor);

#endif
