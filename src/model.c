#include "model.h"

#include <assimp/cimport.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "vertex.h"

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

    model->mesh_count = 0;
    Mesh* meshes = NULL;
    process_node(&meshes, scene->mRootNode, scene, &model->mesh_count);
    model->meshes = malloc(sizeof(Mesh) * model->mesh_count);
    memcpy(model->meshes, meshes, sizeof(Mesh)* model->mesh_count);
    free(meshes);

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

void model_draw(Model* model, Shader* shader) {
}

void process_node(Mesh** meshes, aiNode *node, const aiScene *scene, unsigned int* total) {
    *meshes = malloc(sizeof(Mesh) * node->mNumMeshes);
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        *meshes[i] = proccess_mesh(mesh, scene);
        *total += 1;
    }

    for (int i = 0; i < node->mNumChildren; i++) {
        process_node(meshes, node->mChildren[i], scene, total);
        total++;
    }
}

Mesh proccess_mesh(aiMesh* mesh, const aiScene* scene) {
    Vertex* vertices = malloc(sizeof(Vertex) * mesh->mNumVertices);
    for (int i = 0; i < mesh->mNumVertices; i++) {
        vertices[i].position[0] = mesh->mVertices[i].x;
        vertices[i].position[1] = mesh->mVertices[i].y;
        vertices[i].position[2] = mesh->mVertices[i].z;

        if (mesh->mNormals) {
            vertices[i].normal[0] = mesh->mNormals[i].x;
            vertices[i].normal[1] = mesh->mNormals[i].y;
            vertices[i].normal[2] = mesh->mNormals[i].z;
        }

        if(mesh->mTextureCoords[0]) {
            vertices[i].tex_coords[0] = mesh->mTextureCoords[0][i].x;
            vertices[i].tex_coords[1] = mesh->mTextureCoords[0][i].y;
        }
    }

    // For now, we can assume that each face has 3 indices. AKA: GL_TRIANGLES
    unsigned int indices_count = mesh->mNumFaces * 3;
    unsigned int* indices = malloc(sizeof(unsigned int) * indices_count);
    int idx = 0;
    for (int i = 0; i < mesh->mNumFaces; i++) {
        for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
           indices[idx] = mesh->mFaces[i].mIndices[j];
           idx++;
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    unsigned int texture_count = 0;

    unsigned int num_diffuse_maps = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
    unsigned int num_specular_maps = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);

    texture_count += num_diffuse_maps + num_specular_maps;

    Texture* diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    Texture* specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    
    //unsigned int num_normal_maps = aiGetMaterialTextureCount(material, aiTextureType_HEIGHT);
    //Texture* normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");

    //unsigned int num_height_maps = aiGetMaterialTextureCount(material, aiTextureType_AMBIENT);
    //Texture* height_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");

    Texture* textures = malloc(sizeof(Texture) * texture_count);
    unsigned int offset = 0;
    if (diffuse_maps != NULL) {
        memcpy(textures, diffuse_maps, sizeof(Texture) * num_diffuse_maps);
        offset += num_diffuse_maps;
    }
    if (specular_maps != NULL) {
        memcpy(textures + offset, specular_maps, sizeof(Texture) * num_specular_maps);
        offset += num_specular_maps;
    }
    //if (normal_maps != NULL) {
    //    memcpy(processed_mesh.textures + offset, normal_maps, sizeof(Texture) * num_normal_maps);
    //    offset += num_normal_maps;
    //}
    //if (height_maps != NULL) {
    //    memcpy(processed_mesh.textures + offset, height_maps, sizeof(Texture) * num_height_maps);
    //    offset += num_height_maps;
    //}

    Mesh* processed_mesh = mesh_init(
        vertices, 
        mesh->mNumVertices, 
        indices, 
        indices_count, 
        textures, 
        texture_count);

    free(textures);

    return *processed_mesh;
}

Texture* load_material_textures(
    aiMaterial* material, 
    aiTextureType type, 
    const char* type_name
) {
    unsigned int texture_count = aiGetMaterialTextureCount(material, type);
    if (texture_count == 0) {
        return NULL;
    }

    Texture* textures = malloc(sizeof(Texture) * texture_count);
    for (int i = 0; i < texture_count; i++) {
        struct aiString str;
        aiGetMaterialTexture(material, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
        textures[i] = *texture_init(str.data);
    }
    
    return textures;
}

void model_update(const unsigned int shader_id, Transform* transform) {
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, transform->position);
    glm_rotate_x(model, transform->rotation[0], model);
    glm_rotate_y(model, transform->rotation[1], model);
    glm_rotate_z(model, transform->rotation[2], model);
    glm_scale(model, transform->scale);
    shader_update_uniform4fv(shader_id, "model", model);
}
