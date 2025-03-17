#ifndef MODEL_H
#define MODEL_H

typedef struct Shader Shader;
typedef struct Mesh Mesh;
typedef struct Texture Texture;
typedef struct Transform Transform;

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

Model* model_init(char* path);
void model_exit(Model* model);

void model_draw(Model* model, Shader* shader);
void process_node(Mesh** meshes, aiNode *node, const aiScene *scene, unsigned int* total);
Mesh proccess_mesh(aiMesh* mesh, const aiScene* scene);
unsigned int get_total_textures(aiMaterial* material);

Texture* load_material_textures(
    aiMaterial* material, 
    enum aiTextureType type, 
    const char* type_name);

void model_update(const unsigned int shader_id, Transform* transform);

#endif
