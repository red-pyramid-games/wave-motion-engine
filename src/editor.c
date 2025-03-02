#include "editor.h"
#include <stdio.h>
#include <dirent.h>

#include <cglm/vec3.h>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <limits.h> /* INT_MAX */
#include <time.h> /* struct tm, localtime */

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

typedef struct EditorComponentTransform {
    vec3 position;
    vec3 rotation;
    vec3 scale;
} EditorComponentTransform;

Editor* editor_init(GLFWwindow* window) {
    Editor* editor = malloc(sizeof(Editor));
    editor->glfw = malloc(sizeof(struct nk_glfw));
    editor->ctx = malloc(sizeof(struct nk_context));

    editor->ctx = nk_glfw3_init(editor->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(editor->glfw, &atlas);
    nk_glfw3_font_stash_end(editor->glfw);

    return editor;
}

void editor_exit(Editor *editor) {
    if (editor == NULL) {
        return;
    }
    free (editor);
}

void editor_render(Editor* editor) {
    nk_glfw3_new_frame(editor->glfw);
    editor_render_component_details(editor->ctx);
    nk_glfw3_render(editor->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void editor_render_component_details(struct nk_context* ctx) {
    nk_begin(ctx, "Details", nk_rect(0, 0, 500, 500), NK_WINDOW_TITLE);
    nk_layout_row_static(ctx, 30, 100, 2);
    if (nk_button_label(ctx, "Add")) {
        printf("Add Child Component\n");
    }
    if (nk_button_label(ctx, "Remove")) {
        printf("Remove Child Component\n");
    }
    if (nk_tree_push(ctx, NK_TREE_NODE, "Game Object", NK_MINIMIZED)) {
        editor_render_transform_component(ctx);
        editor_render_sprite_component(ctx);
        nk_tree_pop(ctx);
    }
    nk_end(ctx);
}

void editor_render_transform_component(struct nk_context* ctx) {
    if (nk_tree_push(ctx, NK_TREE_NODE, "Transform", NK_MINIMIZED)) {
        static const float ratio[] = {
            85, 15, 100, 15, 100, 15, 100,
        };
        static char field_buffer[64];
        static char text[9][64];
        static int text_len[9];

        nk_layout_row(ctx, NK_STATIC, 30, 100, ratio);
        nk_label(ctx, "Postition", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_label(ctx, "X:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[0], &text_len[0], 64, nk_filter_default);
        nk_label(ctx, "Y:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[1], &text_len[1], 64, nk_filter_default);
        nk_label(ctx, "Z:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[2], &text_len[2], 64, nk_filter_default);

        nk_layout_row(ctx, NK_STATIC, 30, 100, ratio);
        nk_label(ctx, "Rotation", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_label(ctx, "X:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[0], &text_len[0], 64, nk_filter_default);
        nk_label(ctx, "Y:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[1], &text_len[1], 64, nk_filter_default);
        nk_label(ctx, "Z:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[2], &text_len[2], 64, nk_filter_default);

        nk_layout_row(ctx, NK_STATIC, 30, 100, ratio);
        nk_label(ctx, "Scale", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_label(ctx, "X:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[0], &text_len[0], 64, nk_filter_default);
        nk_label(ctx, "Y:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[1], &text_len[1], 64, nk_filter_default);
        nk_label(ctx, "Z:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, text[2], &text_len[2], 64, nk_filter_default);
        nk_tree_pop(ctx);
    }
}

void editor_render_sprite_component(struct nk_context *ctx) {
    if (nk_tree_push(ctx, NK_TREE_NODE, "Sprite", NK_MINIMIZED)) {
        nk_layout_row_static(ctx, 30, 100, 2);
        nk_label(ctx, "Tetxure", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        static nk_bool selected;
        static int popup_active;
        static char* selected_image_name = "Select";
        if (nk_selectable_label(ctx, selected_image_name, NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE, &selected)) {
            popup_active = 1;
        }
        if (popup_active) {
            static struct nk_rect s = {20, 100, 220, 90};
            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", 0, s)) {
                nk_layout_row_dynamic(ctx, 25, 2);
                if (nk_button_label(ctx, "Cancel")) {
                    popup_active = 0;
                    selected = nk_false;
                    nk_popup_close(ctx);
                }
                DIR* d;
                struct dirent *dir;
                d = opendir("../resources/textures/");
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        if (dir->d_type == DT_REG) {
                            static nk_bool texture_selected;
                            nk_layout_row_static(ctx, 20, 100, 1);
                            if (nk_selectable_label(ctx, dir->d_name, NK_TEXT_ALIGN_LEFT, &texture_selected)) {
                                selected_image_name = dir->d_name;
                                popup_active = 0;
                                selected = nk_false;
                                nk_popup_close(ctx);
                            }
                        }
                    }
                    closedir(d);
                }
                else {
                    nk_label(ctx, "No files found", NK_TEXT_LEFT);
                } 
                nk_popup_end(ctx);
            }
        }
        nk_tree_pop(ctx);
    }
}
