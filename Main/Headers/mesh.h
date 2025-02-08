#ifndef mesh_h
#define mesh_h
#include "vertex.h"
#include "texture.h"
#include "material.h"

typedef struct{
    Vertex *vertices;
    unsigned int *indices;
    Texture *textures;
    unsigned int VAO;
    unsigned int  VBO;
    unsigned int EBO;
    int numVertices;
    int numIndices;
    int numTextures;
    mat4 model;
    mat4 view;
    mat4 projection;
}Mesh;

void init_mesh(Mesh *m);
#endif