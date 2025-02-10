#ifndef SHADER_H
#define SHADER_H

#include <cglm/types.h>
#include <stdbool.h>

typedef struct Shader {
    unsigned int id;
    unsigned int vao;
} Shader;

Shader* shader_init(
    const char* vertex_shader_path, 
    const char* fragment_shader_path);
void shader_exit(Shader* shader);
void shader_use_program(unsigned int program_id);
void shader_update_uniform3f(
    const unsigned int program_id, 
    const char* uniform_name, 
    const float x, 
    const float y, 
    const float z);
void shader_update_uniform4fv(
    const unsigned int program_id,
    const char* uniform_name,
    const mat4 mat);

static char* read_shader_from_file(const char* path);
static unsigned int compile_shader(const char* source, unsigned int type);
static unsigned int link_shaders(
    const unsigned int vertex_shader_id,
    const unsigned int fragment_shader_id);
static bool validate_compilation(const unsigned int shader_id, const char* type);

#endif
