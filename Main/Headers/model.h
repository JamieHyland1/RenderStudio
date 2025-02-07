#ifndef model_h
#define model_h
#include "mesh.h"
#include "../../include/cglm/cglm.h"
#include "../../include/assimp/scene.h"
#include "../../include/assimp/types.h"
#include "../../include/assimp/defs.h"

#include "../../include/assimp/cimport.h"
#include "../../include/assimp/postprocess.h"
#include "../../include/assimp/material.h"

typedef struct{
    Mesh *meshes;
    char *path;
    int numMeshes;
    mat4 model;
    
}Model;

void load_model(Model *m, char *path);
void process_node(Model *model, struct aiNode* node, const struct aiScene* scene);
void process_mesh(Mesh *m,struct aiMesh* aimesh, const struct aiScene* scene, char* directory);
void load_material_textures(Mesh* mesh, struct aiMaterial* mat, enum aiTextureType type, char* typeName, char* directory);
void draw_model(Model *model);
#endif