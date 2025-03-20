#include "texture.h"
#include "shader.h"
#include "transform.h"

#include <glad/glad.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture* texture_init(const char* path) {
   unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        printf("Texture failed to load at path: %s\n",path);
        stbi_image_free(data);
    }

    Texture* texture = malloc(sizeof(Texture));
    texture->id = texture_id;
    return texture;
}

void texture_exit(Texture* texture) {
    if (!texture) {
        return;
    }

    transform_exit(texture->transform);

    free(texture);
}

void texture_render(Texture* texture, unsigned int program_id) {
    glDisable(GL_CULL_FACE);
    
    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // render container
    shader_use_program(program_id);
    glBindVertexArray(texture->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glEnable(GL_CULL_FACE);
}
