#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef struct GLFWwindow GLFWwindow;
typedef struct Camera Camera;

void process_input(GLFWwindow* window, Camera* camera, float delta);

#endif
