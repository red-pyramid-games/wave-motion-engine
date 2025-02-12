#include "shader.h"
#include <string.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

Shader* shader_init(const char* vertex_shader_path, const char* fragment_shader_path) {
    const char* vertex_shader_source = read_shader_from_file(vertex_shader_path); 
    unsigned int vertex_shader_id = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    if (!validate_compilation(vertex_shader_id, "VERTEX")) {
        return NULL;
    }

    const char* fragment_shader_source = read_shader_from_file(fragment_shader_path);
    unsigned int fragment_shader_id = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    if (!validate_compilation(fragment_shader_id, "FRAGMENT")) {
        return NULL;
    }

    unsigned int program_id = link_shaders(vertex_shader_id, fragment_shader_id);
    if (!validate_compilation(program_id, "PROGRAM")) {
        return NULL;
    }

    Shader* shader = malloc(sizeof(Shader));
    shader->id = program_id;
    return shader;
}

void shader_exit(Shader* shader) {
    if (shader == NULL) {
        return;
    }
    free(shader);
}

void shader_use_program(unsigned int program_id) {
    glUseProgram(program_id);
}

void shader_update_uniform3f(
    const unsigned int program_id, 
    const char* uniform_name, 
    const float x, 
    const float y, 
    const float z
) {
    //printf("%d\n", glGetUniformLocation(program_id, uniform_name));
    glUniform3f(glGetUniformLocation(program_id, uniform_name), x, y, z);
}

void shader_update_uniform4fv(
    const unsigned int program_id,
    const char* uniform_name,
    const mat4 mat
) {
    glUniform4fv(glGetUniformLocation(program_id, uniform_name), GL_FALSE, &mat[0][0]);
}

static char* read_shader_from_file(const char* path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Failed to open file for reading: %s\n", path);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    char* buf = (char*)malloc(file_size + 1);
    if (buf == NULL) {
        printf("Error allocating buffer for file reading\n");
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buf, 1, file_size, fp);
    if (bytes_read != file_size) {
        printf("Error reading from file\n");
        fclose(fp);
        free(buf);
        return NULL;
    }

    buf[file_size] = '\0';
    fclose(fp);

    return buf;
}

static unsigned int compile_shader(const char* source, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}

static unsigned int link_shaders(
    const unsigned int vertex_shader_id,
    const unsigned int fragment_shader_id
) {
    unsigned int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    return program_id;
}

static bool validate_compilation(const unsigned int shader_id, const char* type) {
    GLint success;
    GLchar log[1024];

    if (strcmp(type, "PROGRAM")) {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader_id, 1024, NULL, log);
            printf("ERROR:SHADER_COMPILEATION:%s:%s\n", type, log);
            return false;
        }
    } else {
        glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_id, 1024, NULL, log);
            printf("ERROR:PROGRAM_LINK:%s:%s\n", type, log);
            return false;
        }
    }

    return true;
}
