#ifndef EDITOR_H
#define EDITOR_H

typedef struct GLFWwindow GLFWwindow;

typedef struct EditorComponent {
    char* name;
} EditorComponent;

typedef struct EditorComponentList {
    EditorComponent component;
    struct EditorComponentList* next;
    unsigned int count;
} EditorComponentList;

typedef struct Editor {
    struct nk_glfw* glfw;
    struct nk_context* ctx;
    EditorComponentList* component_list;
} Editor;

Editor* editor_init(GLFWwindow* window);
void editor_exit(Editor* editor);
void editor_render(Editor* editor);

static void editor_render_background_edit(Editor* editor);
static void add_editor_component(EditorComponentList* node, const char* name);

#endif
