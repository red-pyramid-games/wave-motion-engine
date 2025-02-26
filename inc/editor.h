#ifndef EDITOR_H
#define EDITOR_H

typedef struct GLFWwindow GLFWwindow;

typedef enum EditorEventType {
    CREATE_TRANSFORM, 
} EditorEventType;

typedef struct EditorEventQueue {
    EditorEventType type;
    struct EditorEventQueue* next;
} EditorEventQueue;

typedef struct EditorComponent {
    char* name;
} EditorComponent;

typedef struct Editor {
    struct nk_glfw* glfw;
    struct nk_context* ctx;
    EditorComponent component;
    EditorEventQueue* event_queue;
} Editor;

Editor* editor_init(GLFWwindow* window);
void editor_exit(Editor* editor);
void editor_render(Editor* editor);

static void editor_render_top_menu(struct nk_context *ctx, EditorEventQueue* queue);
static EditorComponent* editor_render_components(struct nk_context* ctx, EditorComponent* component);
static void editor_render_component_details(struct nk_context* ctx, EditorComponent* component);

static void editor_push_event(EditorEventQueue* queue, EditorEventType type);
static EditorEventType editor_pop_event(EditorEventQueue* queue);
static void editor_process_events(Editor* editor);

#endif
