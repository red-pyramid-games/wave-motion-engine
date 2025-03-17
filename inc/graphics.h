#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

#define DEFAULT_SCREEN_WIDTH 1920.0f 
#define DEFAULT_SCREEN_HEIGHT 1080.0f

typedef struct GLFWwindow GLFWwindow;

extern GLFWwindow* window;

bool graphics_init();
void graphics_exit();

bool graphics_window_should_close();
void graphics_swap_buffers(GLFWwindow* window);
void graphics_poll_events();

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif
