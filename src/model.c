#include "model.h"

#include <assimp/cimport.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"

Model* model_init(char* path) {
    Model* model = malloc(sizeof(Model));
    const C_STRUCT aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
    bool scene_invalid = 
        scene == NULL || 
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        scene->mRootNode == NULL;
    if (scene_invalid) {
        printf("Error reading assimp scene from file: %s\n", path);
        return NULL;
    }
    
    char str[strlen(path)];
    strcpy(str, path);
    char *ptr = strrchr(str, '/');
    if (ptr) {
        *ptr = '\0';
    }
    model->mesh_directory = malloc(sizeof(char) * strlen(str));
    strcpy(model->mesh_directory, str);
    return model;
}

void model_exit(Model* model) {
    if (model == NULL) {
        return;
    }
    for (int i = 0; i < model->mesh_count; i++) {
        mesh_exit(&model->meshes[i]);
    }
    free(model->mesh_directory);
    free(model);
}

void Draw(Shader* shader, Model* model) {
    for (int i = 0; i < model->mesh_count; i++) {
        mesh_draw(shader, &model->meshes[i]);
    }
}

void process_node(aiNode *node, aiScene *scene) {

}

Mesh* proccess_mesh(aiMesh* mesh, aiScene* scene);
Texture* load_material_textures(
    aiMaterial* material, 
    aiTextureType type, 
    const char* type_name
) {
    return NULL;
}
