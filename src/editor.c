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

    editor->ctx = nk_glfw3_init(editor->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(editor->glfw, &atlas);
    nk_glfw3_font_stash_end(editor->glfw);
    
    editor->component_list = malloc(sizeof(EditorComponentList));
    editor->component_list->component.name = "root";
    editor->component_list->next = NULL;
    editor->num_components = 1;

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

    /* GUI */
    editor_render_background_edit(editor);

    nk_glfw3_render(editor->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

static void editor_render_background_edit(Editor* editor) {
    nk_bool can_begin = nk_begin(
        editor->ctx, 
        "Components", 
        nk_rect(50, 50, 230, 250),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE
    ); 

    if (can_begin) {
        nk_layout_row_static(editor->ctx, 30, 80, 2);
        if (nk_button_label(editor->ctx, "Add")) {
            char* component_name = "test_component";
            if (editor->num_components == 0) {
                component_name = "root";
            }
            editor->component_list = add_editor_component(editor->component_list, component_name);
            editor->num_components++;
        }
        if (nk_button_label(editor->ctx, "Remove") && editor->num_components > 0) {
            editor->component_list = remove_editor_component(editor->component_list);
            editor->num_components--;
        }
        EditorComponentList* node = editor->component_list;
        nk_layout_row_static(editor->ctx, 220, 220, 1);
        if (nk_group_begin(editor->ctx, "List:", 0)) {
            static int selected[16];
            nk_layout_row_static(editor->ctx, 18, 100, 1);
            int i = 0;
            while(node != NULL) {
                nk_selectable_label(
                    editor->ctx,
                    node->component.name, 
                    NK_TEXT_CENTERED, 
                    &selected[i]);
                node = node->next; 
                i++;
            }
        }
        nk_group_end(editor->ctx);
    }

    nk_end(editor->ctx);
}

static EditorComponentList* add_editor_component(EditorComponentList* node, const char* name) {
    if (node == NULL) {
        node = malloc(sizeof(EditorComponentList));
        int length = strlen(name);
        node->component.name = malloc(sizeof(char) * length + 1);
        for (int i = 0; i < length; i++) {
            node->component.name[i] = name[i];
        }
        node->component.name[length] = '\0';
        node->next = NULL;
    } else {
        EditorComponentList* temp = node;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = malloc(sizeof(EditorComponentList));
        int length = strlen(name);
        temp->next->component.name = malloc(sizeof(char) * length + 1);
        for (int i = 0; i < length; i++) {
            temp->next->component.name[i] = name[i];
        }
        temp->next->component.name[length] = '\0';
        temp->next->next = NULL;
    }
    return node;
}

static EditorComponentList* remove_editor_component(EditorComponentList* node) {
    if (node == NULL) {
        return NULL;
    }

    if (node->next == NULL) {
        node->next = NULL;
        return NULL;
    }

    EditorComponentList* second_last = node;
    while(second_last->next->next != NULL) {
        second_last = second_last->next;
    }

    second_last->next = NULL;
    return node;
}

