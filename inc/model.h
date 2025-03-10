#ifndef MODEL_H
#define MODEL_H

typedef struct Shader Shader;
typedef struct Mesh Mesh;
typedef struct Texture Texture;

typedef struct aiNode aiNode;
typedef struct aiScene aiScene;
typedef struct aiMesh aiMesh;
typedef struct aiMaterial aiMaterial;
typedef enum aiTextureType aiTextureType;

typedef struct Model {
    Mesh* meshes;
    unsigned int mesh_count;
    char* mesh_directory;
} Model;

Model* model_init(const char* path);
void model_exit(Model* model);

void Draw(Shader* shader, Model* model);

void process_node(aiNode *node, aiScene *scene);

Mesh* proccess_mesh(aiMesh* mesh, aiScene* scene);
Texture* load_material_textures(
    aiMaterial* material, 
    enum aiTextureType type, 
    const char* type_name
);

#endif
