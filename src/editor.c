#include "editor.h"
#include <stdio.h>

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

Editor* editor_init(GLFWwindow* window) {
    Editor* editor = malloc(sizeof(Editor));
    editor->glfw = malloc(sizeof(struct nk_glfw));
    editor->ctx = malloc(sizeof(struct nk_context));

    editor->ctx = nk_glfw3_init(editor->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(editor->glfw, &atlas);
    nk_glfw3_font_stash_end(editor->glfw);

    editor->event_queue = NULL;
    
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
    editor_process_events(editor);
    editor_render_top_menu(editor->ctx, editor->event_queue);
    EditorComponent* component = editor_render_components(
        editor->ctx, 
        &editor->component);
    editor_render_component_details(editor->ctx, component);
    nk_glfw3_render(editor->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

static void editor_render_top_menu(struct nk_context *ctx, EditorEventQueue* queue) {
    /* window flags */
    static nk_flags window_flags = NK_WINDOW_BORDER;

    if (nk_begin(ctx, "", nk_rect(0, 0, 200, 35), window_flags))
    {
        nk_menubar_begin(ctx);

        nk_layout_row_begin(ctx, NK_STATIC, 25, 3);
        nk_layout_row_push(ctx, 35);
        if (nk_menu_begin_label(ctx, "Add", NK_TEXT_LEFT, nk_vec2(120, 35)))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            if(nk_menu_item_label(ctx, "Transform", NK_TEXT_LEFT)) {
                editor_push_event(queue, CREATE_TRANSFORM); 
            }
            nk_menu_end(ctx);
        }
        nk_menubar_end(ctx);
    }
    nk_end(ctx);
}

static EditorComponent* editor_render_components(
    struct nk_context* ctx, 
    EditorComponent* component
) {
    EditorComponent* selected_component = NULL;
        static nk_flags window_flags = NK_WINDOW_TITLE |
            NK_WINDOW_BORDER |
            NK_WINDOW_SCALABLE |
            NK_WINDOW_MOVABLE |
            NK_WINDOW_MINIMIZABLE |
            NK_WINDOW_SCROLL_AUTO_HIDE;
    if (nk_begin(ctx, "Components", nk_rect(0, 35, 200, 200), window_flags)) {

    }
    nk_end(ctx);
    return selected_component;
}

static void editor_render_component_details(struct nk_context* ctx, EditorComponent* component) {
    if (component == NULL) {
        return;
    }
}

static void editor_push_event(EditorEventQueue* queue, EditorEventType type) {
    if (queue == NULL) {
        queue = malloc(sizeof(EditorEventQueue));
        queue->type = type;
        queue->next = NULL;
        return;
    }

    while (queue != NULL) {
        queue = queue->next;
    }

    queue = malloc(sizeof(EditorEventQueue));
    queue->type = type;
    queue->next = NULL;
}

static EditorEventType editor_pop_event(EditorEventQueue *queue) {
    EditorEventQueue* head = queue;
    EditorEventType type = head->type;

    queue = queue->next;
    free(head);

    return type;
}

static void editor_process_events(Editor *editor) {
    while (editor->event_queue != NULL) {
        EditorEventType type = editor_pop_event(editor->event_queue);
        switch (type) {
            case CREATE_TRANSFORM:
                printf("create transfarom\n");
                break;
        }
        editor->event_queue = editor->event_queue->next;
    }
}
