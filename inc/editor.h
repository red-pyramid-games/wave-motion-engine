#ifndef EDITOR_H
#define EDITOR_H

typedef struct GLFWwindow GLFWwindow;

typedef struct EditorComponent {
    char* name;
} EditorComponent;

typedef struct EditorComponentList {
    EditorComponent component;
    struct EditorComponentList* next;
} EditorComponentList;

typedef struct Editor {
    struct nk_glfw* glfw;
    struct nk_context* ctx;
    EditorComponentList* component_list;
    unsigned int num_components;
} Editor;

Editor* editor_init(GLFWwindow* window);
void editor_exit(Editor* editor);
void editor_render(Editor* editor);

static void editor_render_background_edit(Editor* editor);
static EditorComponentList* add_editor_component(EditorComponentList* node, const char* name);
static EditorComponentList* remove_editor_component(EditorComponentList* node);

#endif
