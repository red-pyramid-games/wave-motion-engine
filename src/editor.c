#include "editor.h"
#include "camera.h"
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h> 

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

Editor* editor_init(struct GLFWwindow* window) {
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

void editor_render(Editor* editor, Camera* camera) {
    nk_glfw3_new_frame(editor->glfw);
    editor_render_menu_bar(editor, camera);
    editor_render_camera_details(editor, camera);
    nk_glfw3_render(editor->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void editor_render_menu_bar(Editor* editor, Camera* camera) {
    if (nk_begin(editor->ctx, "Overview", nk_rect(0, 0, 400, 100), 0)) {
        /* menubar */
        enum menu_states {MENU_DEFAULT, MENU_WINDOWS};
        static nk_size mprog = 60;
        static int mslider = 10;
        static nk_bool mcheck = nk_true;
        nk_menubar_begin(editor->ctx);

        /* menu #1 */
        nk_layout_row_begin(editor->ctx, NK_STATIC, 25, 5);
        nk_layout_row_push(editor->ctx, 45);
        if (nk_menu_begin_label(editor->ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200))) {
            static size_t prog = 40;
            static int slider = 10;
            static nk_bool check = nk_true;
            nk_layout_row_dynamic(editor->ctx, 25, 1);
            nk_menu_item_label(editor->ctx, "Hide", NK_TEXT_LEFT);
            nk_menu_item_label(editor->ctx, "About", NK_TEXT_LEFT);
            nk_progress(editor->ctx, &prog, 100, NK_MODIFIABLE);
            nk_slider_int(editor->ctx, 0, &slider, 16, 1);
            nk_checkbox_label(editor->ctx, "check", &check);
            nk_menu_end(editor->ctx);
        }
        /* menu #2 */
        nk_layout_row_push(editor->ctx, 60);
        if (nk_menu_begin_label(editor->ctx, "ADVANCED", NK_TEXT_LEFT, nk_vec2(200, 600))) {
            enum menu_state {MENU_NONE,MENU_FILE, MENU_EDIT,MENU_VIEW,MENU_CHART};
            static enum menu_state menu_state = MENU_NONE;
            enum nk_collapse_states state;

            state = (menu_state == MENU_FILE) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(editor->ctx, NK_TREE_TAB, "FILE", &state)) {
                menu_state = MENU_FILE;
                nk_menu_item_label(editor->ctx, "New", NK_TEXT_LEFT);
                if (nk_menu_item_label(editor->ctx, "Open", NK_TEXT_LEFT)) {
                    FILE *fp = fopen("../save_file.txt", "r");
                    if (fp == NULL) {
                        printf("Failed to open file for reading: %s\n", "../save_file.txt");
                        return;
                    }

                    fseek(fp, 0, SEEK_END);
                    long file_size = ftell(fp);
                    rewind(fp);

                    char* buf = (char*)malloc(file_size + 1);
                    if (buf == NULL) {
                        printf("Error allocating buffer for file reading\n");
                        fclose(fp);
                        return;
                    }

                    size_t bytes_read = fread(buf, 1, file_size, fp);
                    if (bytes_read != file_size) {
                        printf("Error reading from file\n");
                        fclose(fp);
                        free(buf);
                        return;
                    }

                    buf[file_size] = '\0';

                    const char* tok;
                    int i = 0;
                    for (tok = strtok(buf, ":"); tok && *tok; tok = strtok(NULL, ":\n")) {
                        if(strcmp(tok, "bg") != 0) {
                            camera->background_color[i] = atof(tok);
                            i++;
                        }
                    }

                    fclose(fp);
                    free(buf);  
                }
                if (nk_menu_item_label(editor->ctx, "Save", NK_TEXT_LEFT)) {
                    FILE* fp;
                    if ((fp = fopen("../save_file.txt", "w"))) {
                        fprintf(fp, "bg:");
                        fprintf(fp, "%f:", camera->background_color[0]);
                        fprintf(fp, "%f:", camera->background_color[1]);
                        fprintf(fp, "%f:", camera->background_color[2]);
                        fprintf(fp, "%f", camera->background_color[3]);
                        fclose(fp);
                    }
                }
                nk_menu_item_label(editor->ctx, "Close", NK_TEXT_LEFT);
                nk_menu_item_label(editor->ctx, "Exit", NK_TEXT_LEFT);
                nk_tree_pop(editor->ctx);
            } else menu_state = (menu_state == MENU_FILE) ? MENU_NONE: menu_state;

            state = (menu_state == MENU_EDIT) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(editor->ctx, NK_TREE_TAB, "EDIT", &state)) {
                menu_state = MENU_EDIT;
                nk_menu_item_label(editor->ctx, "Copy", NK_TEXT_LEFT);
                nk_menu_item_label(editor->ctx, "Delete", NK_TEXT_LEFT);
                nk_menu_item_label(editor->ctx, "Cut", NK_TEXT_LEFT);
                nk_menu_item_label(editor->ctx, "Paste", NK_TEXT_LEFT);
                nk_tree_pop(editor->ctx);
            } else menu_state = (menu_state == MENU_EDIT) ? MENU_NONE: menu_state;

            state = (menu_state == MENU_VIEW) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(editor->ctx, NK_TREE_TAB, "VIEW", &state)) {
                menu_state = MENU_VIEW;
                nk_menu_item_label(editor->ctx, "About", NK_TEXT_LEFT);
                nk_menu_item_label(editor->ctx, "Options", NK_TEXT_LEFT);
                nk_menu_item_label(editor->ctx, "Customize", NK_TEXT_LEFT);
                nk_tree_pop(editor->ctx);
            } else menu_state = (menu_state == MENU_VIEW) ? MENU_NONE: menu_state;

            state = (menu_state == MENU_CHART) ? NK_MAXIMIZED: NK_MINIMIZED;
            if (nk_tree_state_push(editor->ctx, NK_TREE_TAB, "CHART", &state)) {
                size_t i = 0;
                const float values[]={26.0f,13.0f,30.0f,15.0f,25.0f,10.0f,20.0f,40.0f,12.0f,8.0f,22.0f,28.0f};
                menu_state = MENU_CHART;
                nk_layout_row_dynamic(editor->ctx, 150, 1);
                nk_chart_begin(editor->ctx, NK_CHART_COLUMN, NK_LEN(values), 0, 50);
                for (i = 0; i < NK_LEN(values); ++i)
                    nk_chart_push(editor->ctx, values[i]);
                nk_chart_end(editor->ctx);
                nk_tree_pop(editor->ctx);
            } else menu_state = (menu_state == MENU_CHART) ? MENU_NONE: menu_state;
            nk_menu_end(editor->ctx);
        }
        nk_menubar_end(editor->ctx);
    }
    nk_end(editor->ctx);
}

void editor_render_camera_details(Editor* editor, Camera* camera) {
    enum nk_panel_flags flags = NK_WINDOW_TITLE | NK_WINDOW_MOVABLE;
    enum nk_text_align text_flags = NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE;

    nk_begin(editor->ctx, "Camera", nk_rect(0, 100, 500, 500), flags);

    nk_layout_row_dynamic(editor->ctx, 25, 3);
    camera->position[0] = nk_propertyf(editor->ctx, "X:", -5.0f, camera->position[0], 5.0f, 0.01f, 0.005f);
    camera->position[1] = nk_propertyf(editor->ctx, "Y:", -5.0f, camera->position[1], 5.0f, 0.01f, 0.005f);
    camera->position[2] = nk_propertyf(editor->ctx, "Z:", -5.0f, camera->position[2], 5.0f, 0.01f, 0.005f);

    nk_layout_row_dynamic(editor->ctx, 20, 1);
    nk_label(editor->ctx, "background:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(editor->ctx, 25, 1);

    struct nk_colorf background_color = {
        camera->background_color[0],
        camera->background_color[1],
        camera->background_color[2],
        camera->background_color[3],
    };
    nk_bool begin_color = nk_combo_begin_color(
        editor->ctx,
        nk_rgb_cf(background_color),
        nk_vec2(nk_widget_width(editor->ctx), 400)
    );

    if (begin_color) {
        nk_layout_row_dynamic(editor->ctx, 120, 1);
        background_color = nk_color_picker(editor->ctx, background_color, NK_RGBA);
        nk_layout_row_dynamic(editor->ctx, 25, 1);
        camera->background_color[0] = nk_propertyf(editor->ctx, "#R:", 0, background_color.r, 1.0f, 0.01f,0.005f);
        camera->background_color[1] = nk_propertyf(editor->ctx, "#G:", 0, background_color.g, 1.0f, 0.01f,0.005f);
        camera->background_color[2] = nk_propertyf(editor->ctx, "#B:", 0, background_color.b, 1.0f, 0.01f,0.005f);
        camera->background_color[3] = nk_propertyf(editor->ctx, "#A:", 0, background_color.a, 1.0f, 0.01f,0.005f);
        nk_combo_end(editor->ctx);
    }

    nk_end(editor->ctx);
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
        static char position_text[3][64];
        static int position_text_len[3];

        nk_layout_row(ctx, NK_STATIC, 30, 100, ratio);
        nk_label(ctx, "Postition", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_label(ctx, "X:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, position_text[0], &position_text_len[0], 64, nk_filter_default);
        nk_label(ctx, "Y:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, position_text[1], &position_text_len[1], 64, nk_filter_default);
        nk_label(ctx, "Z:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, position_text[2], &position_text_len[2], 64, nk_filter_default);

        static char rotation_text[3][64];
        static int rotation_text_len[3];
        nk_layout_row(ctx, NK_STATIC, 30, 100, ratio);
        nk_label(ctx, "Rotation", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_label(ctx, "X:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, rotation_text[0], &rotation_text_len[0], 64, nk_filter_default);
        nk_label(ctx, "Y:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, rotation_text[1], &rotation_text_len[1], 64, nk_filter_default);
        nk_label(ctx, "Z:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, rotation_text[2], &rotation_text_len[2], 64, nk_filter_default);

        static char scale_text[3][64];
        static int scale_text_len[3];
        nk_layout_row(ctx, NK_STATIC, 30, 100, ratio);
        nk_label(ctx, "Scale", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_label(ctx, "X:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, scale_text[0], &scale_text_len[0], 64, nk_filter_default);
        nk_label(ctx, "Y:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, scale_text[1], &scale_text_len[1], 64, nk_filter_default);
        nk_label(ctx, "Z:", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);
        nk_edit_string(ctx, NK_EDIT_SIMPLE, scale_text[2], &scale_text_len[2], 64, nk_filter_default);
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
                static nk_bool none_selected;
                nk_layout_row_static(ctx, 20, 100, 1);
                if (nk_selectable_label(ctx, " ", NK_TEXT_ALIGN_LEFT, &none_selected)) {
                    selected_image_name = "Select";
                    popup_active = 0;
                    selected = nk_false;
                    nk_popup_close(ctx);
                }
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
                nk_popup_end(ctx);
            }
        }
        nk_tree_pop(ctx);
    }
}
