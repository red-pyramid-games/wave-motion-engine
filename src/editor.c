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
    editor->component_list->next = NULL;
    editor->component_list->count = 0;

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
        nk_layout_row_static(editor->ctx, 30, 80, 1);
        nk_label(editor->ctx, "Componets:", NK_TEXT_LEFT);
        //TODO: Create add and delete buttons for component list
        EditorComponentList* head = editor->component_list;
        while (head != NULL) {
            // TODO: draw the component inside this window
            head = head->next; 
        }
        nk_layout_row_dynamic(editor->ctx, 25, 1);
    }

    nk_end(editor->ctx);
}

static void add_editor_component(EditorComponentList* node, const char* name) {
    if (node->next != NULL) {
        return;
    }    

    node->next = malloc(sizeof(EditorComponentList));
    strcpy(node->next->component.name, name);
    node->next->count = node->count + 1;
    node->next->next = NULL;
}

