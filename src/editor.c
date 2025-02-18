#include "editor.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

Editor* editor_init(GLFWwindow* window) {
    Editor* editor = malloc(sizeof(Editor));
    editor->glfw = malloc(sizeof(struct nk_glfw));
    editor->ctx = malloc(sizeof(struct nk_context));
    editor->bg = malloc(sizeof(struct nk_colorf));

    editor->ctx = nk_glfw3_init(editor->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(editor->glfw, &atlas);
    nk_glfw3_font_stash_end(editor->glfw);

    editor->bg->r = 0.10f;
    editor->bg->g = 0.18f;
    editor->bg->b = 0.24f;
    editor->bg->a = 1.0f;

    return editor;
}

void editor_exit(Editor *editor) {
    if (editor == NULL) {
        return;
    }
    free (editor);
}

void editor_clear(Editor* editor) {
    glClearColor(editor->bg->r, editor->bg->g, editor->bg->b, editor->bg->a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void editor_render(Editor* editor) {
    nk_glfw3_new_frame(editor->glfw);

    /* GUI */
    editor_render_background_edit(editor);

    nk_end(editor->ctx);
    nk_glfw3_render(editor->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

static void editor_render_background_edit(Editor* editor) {
    nk_bool can_begin = nk_begin(
        editor->ctx, 
        "Background", 
        nk_rect(50, 50, 230, 250),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE
    ); 

    if (can_begin) {
        nk_layout_row_static(editor->ctx, 30, 80, 1);
        nk_label(editor->ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(editor->ctx, 25, 1);
        nk_bool begin_color = nk_combo_begin_color(
            editor->ctx, 
            nk_rgb_cf(*editor->bg), 
            nk_vec2(nk_widget_width(editor->ctx),
            400));

        if (begin_color) {
            nk_layout_row_dynamic(editor->ctx, 120, 1);
            *editor->bg = nk_color_picker(editor->ctx, *editor->bg, NK_RGBA);
            nk_layout_row_dynamic(editor->ctx, 25, 1);
            editor->bg->r = nk_propertyf(editor->ctx, "#R:", 0, editor->bg->r, 1.0f, 0.01f,0.005f);
            editor->bg->g = nk_propertyf(editor->ctx, "#G:", 0, editor->bg->g, 1.0f, 0.01f,0.005f);
            editor->bg->b = nk_propertyf(editor->ctx, "#B:", 0, editor->bg->b, 1.0f, 0.01f,0.005f);
            editor->bg->a = nk_propertyf(editor->ctx, "#A:", 0, editor->bg->a, 1.0f, 0.01f,0.005f);
            nk_combo_end(editor->ctx);
        }
    }
}
