#include "model.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

Model* model_init(const char* path) {
    return NULL;
}

void model_exit(Model* model) {

}

void Draw(Shader* shader, Model* model) {

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
