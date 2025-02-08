#ifndef model_h
#define model_h

#include "../../include/cglm/cglm.h"
#include "../../include/assimp/scene.h"
#include "../../include/assimp/types.h"
#include "../../include/assimp/defs.h"

#include "../../include/assimp/cimport.h"
#include "../../include/assimp/postprocess.h"
#include "../../include/assimp/material.h"
#include "material.h"
#include "mesh.h"

typedef struct{
    Mesh *meshes;
    char *path;
    int numMeshes;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    mat4 model_matrix;
    mat4 view_matrix;
    mat4 projection_matrix;
    Material material;
    
}Model;

void load_model(Model *m, char *path);
void process_node(Model *model, struct aiNode* node, const struct aiScene* scene);
void process_mesh(Mesh *m,struct aiMesh* aimesh, const struct aiScene* scene, char* directory);
void load_material_textures(Mesh* mesh, struct aiMaterial* mat, enum aiTextureType type, char* typeName, char* directory);
void update_model(Model *model);
void draw_model(Model *model);
void free_model(Model *model);
#endif