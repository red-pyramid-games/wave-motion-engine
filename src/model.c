#include "model.h"

#include <assimp/cimport.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"
#include "texture.h"
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

void model_draw(Shader* shader, Model* model) {
    for (int i = 0; i < model->mesh_count; i++) {
        mesh_draw(shader, &model->meshes[i]);
    }
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
    Mesh processed_mesh;
    processed_mesh.vertices = malloc(sizeof(Vertex) * mesh->mNumVertices);
    processed_mesh.vertex_count = mesh->mNumVertices;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        processed_mesh.vertices[i].position[0] = mesh->mVertices[i].x;
        processed_mesh.vertices[i].position[1] = mesh->mVertices[i].y;
        processed_mesh.vertices[i].position[2] = mesh->mVertices[i].z;

        if (mesh->mNormals) {
            processed_mesh.vertices[i].normal[0] = mesh->mNormals[i].x;
            processed_mesh.vertices[i].normal[1] = mesh->mNormals[i].y;
            processed_mesh.vertices[i].normal[2] = mesh->mNormals[i].z;
        }

        if(mesh->mTextureCoords[0]) {
            processed_mesh.vertices[i].tex_coords[0] = mesh->mTextureCoords[0][i].x;
            processed_mesh.vertices[i].tex_coords[1] = mesh->mTextureCoords[0][i].y;
        }
    }

    // For now, we can assume that each face has 3 indices. AKA: GL_TRIANGLES
    processed_mesh.indices_count = mesh->mNumFaces * 3;
    processed_mesh.indices = malloc(sizeof(unsigned int) * processed_mesh.indices_count);
    int idx = 0;
    for (int i = 0; i < mesh->mNumFaces; i++) {
        for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
           processed_mesh.indices[idx] = mesh->mFaces[i].mIndices[j];
           idx++;
        }
    }

    processed_mesh.texture_count = 0;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    unsigned int num_diffuse_maps = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
    processed_mesh.texture_count += num_diffuse_maps;
    Texture* diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");

    unsigned int num_specular_maps = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
    processed_mesh.texture_count += num_specular_maps;
    Texture* specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    
    unsigned int num_normal_maps = aiGetMaterialTextureCount(material, aiTextureType_HEIGHT);
    processed_mesh.texture_count += num_normal_maps;
    Texture* normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");

    unsigned int num_height_maps = aiGetMaterialTextureCount(material, aiTextureType_AMBIENT);
    processed_mesh.texture_count += num_height_maps;
    Texture* height_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");

    processed_mesh.textures = malloc(sizeof(Texture) * processed_mesh.texture_count);
    unsigned int offset = 0;
    if (diffuse_maps != NULL) {
        memcpy(processed_mesh.textures, diffuse_maps, sizeof(Texture) * num_diffuse_maps);
        offset += num_diffuse_maps;
    }
    if (specular_maps != NULL) {
        memcpy(processed_mesh.textures + offset, specular_maps, sizeof(Texture) * num_specular_maps);
        offset += num_specular_maps;
    }
    if (normal_maps != NULL) {
        memcpy(processed_mesh.textures + offset, normal_maps, sizeof(Texture) * num_normal_maps);
        offset += num_normal_maps;
    }
    if (height_maps != NULL) {
        memcpy(processed_mesh.textures + offset, height_maps, sizeof(Texture) * num_height_maps);
        offset += num_height_maps;
    }

    return processed_mesh;
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

    Texture* texture = texture_init("../resources/textures/container.jpg");
    texture->type = malloc(sizeof(char) * strlen(type_name));
    memcpy(texture->type, type_name, sizeof(char) * strlen(type_name));
    //for (int i = 0; i < texture_count; i++) {
    //    struct aiString str;
    //    aiGetMaterialTexture(material, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
    //    bool skip = false;
    //    int textures_loaded = 0;
    //    for (int j = 0; j < textures_loaded; j++) {
    //        if (strcmp("", str.data) == 0) {
    //            skip = true;
    //            break;
    //        }
    //    }
    //    if (!skip) {
    //    }
    //}
    
    return texture;
}
